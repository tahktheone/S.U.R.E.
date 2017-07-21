#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#include <SureDefines.h>
#define GPU
#include <SureGPUData.h>

double3 PenVec(double3 V1)
{
    double3 V2;
    double x = V1.x>0?V1.x:-V1.x;
    double y = V1.y>0?V1.y:-V1.y;
    double z = V1.z>0?V1.z:-V1.z;
    int typ = x < y ? x < z ?   0   :   2   : y < z ? 1 : 2 ;
//                          * -x<z- * -x>z- *
//                  * ----------x<y-------- * -- x>y -------*
    if(typ==0)
    {
        V2.x = 0;
        V2.y = V1.z;
        V2.z = -V1.y;
    };
    if(typ==1)
    {
        V2.x = V1.z;
        V2.y = 0;
        V2.z = -V1.x;
    };
    if(typ==2)
    {
        V2.x = V1.y;
        V2.y = -V1.x;
        V2.z = 0;
    };
    return V2;
};

bool CollideRaySphered(double3 tp,double3 tv,double3 o,double r, double2 *t, bool *in,double* id)
{
    double3 vtc = tp-o; //VecToCenter
    double B = dot(vtc,tv);
    double C = dot(vtc,vtc)-r*r;
    double D = B*B-C;
    if(D>=0){
        double kd = sqrt(D);
        double t1 = -B+kd;
        double t2 = -B-kd;
        t->x = t1<t2 ? t1 : t2;
        t->y = t1>t2 ? t1 : t2;
        if(t->x>*id) return false; // отсеиваем случаи когда ближайшая точка сферы дальше чем текущий intersect_dist
        if(t->y<SURE_R_DELTA) return false; // отсеиваем случаи когда сфера "сзади"
        *in = t->x<SURE_R_DELTA ? true : false; // определяем мы внутри или снаружи
        if(*in&&t->y>*id) return false; // отсеиваем случай когда мы внутри но дальняя стенка дальше id
        *id = *in ? t->y : t->x; //если внутри -- id = дальняя стенка, если снаружи ближняя
        return true;
    }else{
        return false;
    };
}

__kernel void Trace(__global float* rgbmatrix,
                   __global float* Randomf,
                   __global struct SureGPUData* GPUData,
                   __global struct SureDrawable* Drawables)
{
	// координаты обрабатываемой точки
	int x = get_global_id(0)*SURE_SCALE;
    int y = get_global_id(1)*SURE_SCALE;

    // Вспомогательные переменные для поиска пересечений
    double2 t;
    bool in;

    // поиск столкновения
    global struct SureDrawable* lv_cur;
    double3 cp; // collision point
    double3 cn; // collision normal
    double id; // intersect dist
    global struct SureDrawable* cur;
    global struct SureDrawable* col;
    global struct SureDrawable* lv_dr;
    double col_refr;
    uchar3 col_rgb;
    double col_transp;
    int col_dt; // тип рандомизации
    double col_ds; // sigma рандомизации
    double col_dm; // матожидание рандомизации
    //Вспомогательные переменные для трассировки
    double tl;
    int ri;
    double3 fade;
    double3 rgb;
    // Инициализируем рандом
    uint xx = x+y;
    uint yy = y;
    uint r = xx*yy+xx+yy*xx+yy;
    while(r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
    double3 dZ = GPUData->cam_vec;
    double3 dY = -GPUData->cam_upvec;
    double3 dX = cross(dZ,dY);
    size_t mx = get_global_size(0)*SURE_SCALE;
    size_t my = get_global_size(1)*SURE_SCALE;
    double kx = ((double)x-(double)mx/2.0)/(double)mx;
    double ky = ((double)y-(double)my/2.0)/(double)mx;
    double3 tv = dZ+dX*kx+dY*ky;
    double3 tp = GPUData->cam_x;
    tv = normalize(tv);
    cur = &Drawables[0];
    rgb.x = 0; rgb.y = 0; rgb.z = 0;
    fade.x = 1; fade.y = 1; fade.z = 1;
    tl = 0; ri = 0; in = false;

    while((tl<SURE_R_MAXDISTANCE)&&((fade.x+fade.y+fade.z)>SURE_R_FADELIMIT)&&(ri<SURE_R_MAXITERS))
    {
        ++ri;
        lv_cur = cur;
        id = SURE_R_MAXDISTANCE;
        col = 0;
        for(int i=1;i<GPUData->m_drawables;++i)
        {
            lv_dr = &Drawables[i];
            switch(lv_dr->type)
            {
                case SURE_DR_SPHERE:
                {
                    if(CollideRaySphered(tp,tv,lv_dr->X,lv_dr->lx,&t,&in,&id))
                    {
                       if(lv_dr->sided||!in)
                       {
                           col = lv_dr;
                           cur = lv_cur;
                       }else{
                           col = &Drawables[0];
                           cur = lv_dr;
                       };
                        RT_SETCOL;
                        cp = tp + tv*id;
                        if(in)
                        {
                            cn = lv_dr->X - cp;
                        }else{
                            cn = cp - lv_dr->X;
                        };
                        cn = normalize(cn);
                    }; // if(CollideRaySphered)
                    break;
                }; // Сфера

                case SURE_DR_SQUARE:
                {
                    double3 vtp = tp-lv_dr->X;
                    double dist = dot(lv_dr->oz,vtp);                // Расстояние до плоскости
                    if(dist<SURE_R_DELTA&&dist>-SURE_R_DELTA)break;  // Вплотную -- игнорируем
                    double dir = dot(lv_dr->oz,tv);                  // проекция луча на нормаль
                    double t = - dist/dir;                           // Расстояние до точки пересечения
                    if(t<SURE_R_DELTA||t>id)break;                   // Слишком близко или дальше чем уже найденное пересечение
                    double3 l_cp = tp+tv*t;                       // точка пересечение
                    double3 l_vtcp = lv_dr->X - l_cp;             // вектор к точке пересечения
                    double lx = dot(lv_dr->ox,l_vtcp);               // Локальные координаты
                    double ly = dot(lv_dr->oy,l_vtcp);               // Локальные координаты
                    if(lx>lv_dr->lx||lx<-lv_dr->lx||ly>lv_dr->ly||ly<-lv_dr->ly)break; // Нет попадания в область
                    id = t;
                    cp = l_cp;
                    if(dir>0.0) // с внутренней стороны
                    {
                        if(lv_dr->sided)
                        { // материал двухсторонний
                            col = lv_dr;
                            cur = lv_cur;
                        }else{  // материал односторонний
                            col = &Drawables[0];
                            cur = lv_dr;
                        };
                        RT_SETCOL;
                        cn = -lv_dr->oz;
                    }else{ // с внешней стороны
                        col = lv_dr;
                        cur = lv_cur;
                        RT_SETCOL;
                        cn = lv_dr->oz;
                    };
                    break;
                }; // Квадрат
                default:
                    break;
            }; // switch(lv_dr->type)
        };// Цикл по Drawables

        if(col==0)break; // луч улетел вникуда

        if(col->radiance>0)
        {
            rgb.x += fade.x*col->rgb.x;
            rgb.y += fade.y*col->rgb.y;
            rgb.z += fade.z*col->rgb.z;
            break;
        };
        bool recheck = true;
        int c = 0;
        while(recheck&&++c<10)
        {
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
            double rr = Randomf[r];
            recheck = false;
            if(rr<col_transp)
            { // сработала прозрачность -- считаем преломление

            }else{ // не сработала прозрачность -- считаем отражение
                double3 ndy = normalize(PenVec(cn)); // ndy -- верктор пенрердикулярный нормали
                double3 ndx = cross(ndy,cn); // ndx -- второй пенпердикулярный вектор
                double3 cnr;
                if(col_dt==SURE_D_EQUAL) // Равномерное распеределение
                {
                    double dx = 2;
                    double dy = 2;
                    while((dx*dx+dy*dy)>1)
                    {
                        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                        double ra = Randomf[r];
                        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                        double rb = Randomf[r];
                        dx = ra*2.0-1.0;
                        dy = rb*2.0-1.0;
                    };
                    double dz = sqrt(1-dx*dx-dy*dy);
                    cnr = cn*dz+ndx*dx+ndy*dy;
                    cnr = normalize(cnr);
                }else if(col_dt==SURE_D_NORM) // Нормальное распределеие
                {
                    if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                    double ra = Randomf[r]*2*SURE_PI;
                    if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                    double rb = Randomf[r];
                    if(rb>0.5)
                        rb = sqrt(-2.0*log(1.0-rb));
                    else
                        rb = sqrt(-2.0*log(rb));
                    rb = rb-(2.30753+0.27061*rb)/(1.0+0.99229*rb+0.04481*rb*rb);
                    rb *= col_ds;
                    rb += col_dm;
                    cnr = cn*cos(rb)+ndx*cos(ra)*sin(rb)+ndy*sin(ra)*sin(rb);
                    cnr = normalize(cnr);
                }; // Нормальное распределеие
                double l = -2*dot(tv,cnr);
                double3 rv = cnr*l+tv;
                if(dot(rv,cn)>=0)
                { // Результат отражения направлен от объекта
                    tp = cp;
                    rv = normalize(rv);
                    tv = rv;
                    tl+=id;
                    fade.x *= col_rgb.x/255.0;
                    fade.y *= col_rgb.y/255.0;
                    fade.z *= col_rgb.z/255.0;
                }else{ // результат отражения направлен внутрь объекта
                    recheck = true;
                };
            }; // отражение/преломление
        }; // while recheck
    }; //WHILE Трассировка

    for(int sx=0;sx<SURE_SCALE;++sx)
    for(int sy=0;sy<SURE_SCALE;++sy)
    {
        if(GPUData->reset)
        {
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3] = rgb.x;
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3+1] = rgb.y;
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3+2] = rgb.z;
        }else{
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3] += rgb.x;
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3+1] += rgb.y;
            rgbmatrix[y*SURE_MAXRES_X*3*SURE_FAA+sy*SURE_MAXRES_X*3*SURE_FAA+x*3+sx*3+2] += rgb.z;
        };
    };
}
