#include "SurePhysThread.h"

#include <SureData.h>

SurePhysThread::~SurePhysThread()
{

}

void SurePhysThread::run()
{
    while(m_running)
    {
        clock_gettime(CLOCK_MONOTONIC,&framestart);

        if(!EngineData->paused)
        {

        /*
        tick++;
        if(tick>30)
        {
            int i = EngineData->m_objects;
            EngineData->objects[i].X.x = 40.0*((float)rand()/(float)RAND_MAX - 0.5); //Координаты центра
            EngineData->objects[i].X.y = 40.0*((float)rand()/(float)RAND_MAX - 0.5); //Координаты центра
            EngineData->objects[i].X.z = 10*((float)rand()/(float)RAND_MAX)+80.0; //Координаты центра
            EngineData->objects[i].ox.x = 1;
            EngineData->objects[i].ox.y = 0;
            EngineData->objects[i].ox.z = 0;
            EngineData->objects[i].oy.x = 0;
            EngineData->objects[i].oy.y = 1;
            EngineData->objects[i].oy.z = 0;
            EngineData->objects[i].oz.x = 0;
            EngineData->objects[i].oz.y = 0;
            EngineData->objects[i].oz.z = 1;
            EngineData->objects[i].lx = 3.0+(float)rand()/(float)RAND_MAX; // длина
            EngineData->objects[i].ly = 1.0; // ширина
            EngineData->objects[i].lz = 1.0; // высота
            EngineData->objects[i].lp = EngineData->objects[i].lx*1.8;
            EngineData->objects[i].type = SURE_OBJ_SPHERE;
            EngineData->objects[i].movable = true;
            EngineData->objects[i].collidable = true;
            EngineData->objects[i].drawable.X  = EngineData->objects[i].X;
            EngineData->objects[i].drawable.ox = EngineData->objects[i].ox;
            EngineData->objects[i].drawable.oy = EngineData->objects[i].oy;
            EngineData->objects[i].drawable.oz = EngineData->objects[i].oz;
            EngineData->objects[i].drawable.lx = EngineData->objects[i].lx;
            EngineData->objects[i].drawable.ly = EngineData->objects[i].ly;
            EngineData->objects[i].drawable.lz = EngineData->objects[i].lz;
            EngineData->objects[i].drawable.type = SURE_DR_SPHERE; // форма
            EngineData->objects[i].drawable.radiance = 0.0; // свечение
            EngineData->objects[i].drawable.transp = 1.1;//0.8+0.3*(float)rand()/(float)RAND_MAX; // прозрачность
            EngineData->objects[i].drawable.transp_i = 0.01+0.06*(float)rand()/(float)RAND_MAX; // прозрачность
            EngineData->objects[i].drawable.refr = 1.49; // Коэффициент преломления
            EngineData->objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
            EngineData->objects[i].drawable.dist_sigma = 0.02*(float)rand()/(float)RAND_MAX; // sigma рандомизации
            EngineData->objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
            EngineData->objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
            EngineData->objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX;// цвет
            EngineData->objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
            EngineData->objects[i].drawable.sided = false;
            EngineData->objects[i].initp4();
            EngineData->m_objects++;
            tick = 0;

            for(int iln=1;iln<=phase;++iln)
            {
                int lnk = EngineData->m_links;
                EngineData->links[lnk].o1 = &EngineData->objects[i];
                EngineData->links[lnk].o2 = &EngineData->objects[i-iln];
                EngineData->links[lnk].l = 10;
                EngineData->links[lnk].k = 0.08;
                EngineData->m_links++;
            };
            phase++;
            if(phase==4)phase=0;
        };
        */


            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o = &EngineData->objects[i];
                if(lv_o->movable)
                {
                    lv_o->next_tick();
                    lv_o->push(lv_o->X,my_double3{0,0,-1},0.01);
                };
            };

            for(int k = 0;k<1;++k)
            {

            for(int i = 0;i<EngineData->m_links;++i)
            {
                SureObject* o1 = EngineData->links[i].o1;
                SureObject* o2 = EngineData->links[i].o2;
                my_double3 pd = o1->X-o2->X;
                double d = (EngineData->links[i].l-__LENGTH(pd))*EngineData->links[i].k;
                pd = __NORMALIZE(pd);
                if(!o1->movable)
                {
                    o2->push(o2->X,pd,-d);
                }
                else if(!o2->movable)
                {
                    o1->push(o1->X,pd,d);
                }
                else{
                    o1->push(o1->X,pd,d*0.5);
                    o2->push(o2->X,pd,-d*0.5);
                }; // проверка на movable
            };


            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o = &EngineData->objects[i];
                if(lv_o->movable)
                {
                    lv_o->align_p4();
                };
            };

            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o1 = &EngineData->objects[i];
                if(lv_o1->collidable)
                {
                    for(int j = i+1;j<EngineData->m_objects;++j)
                    {
                        SureObject* lv_o2 = &EngineData->objects[j];
                        if((lv_o2->collidable)&&(lv_o2->movable||lv_o1->movable))
                        {
                            SureObject* o1;
                            SureObject* o2;
                            if(lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_SPHERE)
                            { // шарик с шариком
                                o1=lv_o1; o2=lv_o2;
                                my_double3 pd = o1->X-o2->X;
                                double d = __LENGTH(pd);
                                if(d<(o1->lx+o2->lx))
                                { // есть пересечение
                                    pd = __NORMALIZE(pd); // Penetration Direction
                                    my_double3 pp = (o1->X+o2->X)*0.5; // Penetration point
                                    double pl = o1->lx+o2->lx-d; // Penetration Length
                                    ObjCollide(o1,o2,pp,pd,pl);
                                };// есть пересечение
                            };  // шарик с шариком
                            if((lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE))
                            {   // шарик с плоскостью
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
                                // шарик с плоскостью
                                // o1 - шарик
                                // o2 - плоскость
                                my_double3 vtp = o2->X - o1->X; // вектор от центра к точке на плоскость
                                double dist = -dot(vtp,o2->oz); // расстояние от центра шара до плоскости
                                if(fabs(dist)<o1->lx)
                                { // есть пересечение
                                    double lx = -dot(vtp,o2->ox); // локальная x-координата центра шара
                                    double ly = -dot(vtp,o2->oy); // локальная y-координата центра шара
                                    if(fabs(lx)<o2->lx&&fabs(ly)<o2->ly)
                                    {// центр шара внутри прямоугольника
                                        my_double3 pd = o2->oz*dist; // Penetration Direction
                                        double pl = (o1->lx-fabs(dist)); // Penetration Length
                                        pd = __NORMALIZE(pd);
                                        my_double3 pp = o1->X-pd*(o1->lx-pl); // Penetration point
                                        ObjCollide(o1,o2,pp,pd,pl);
                                    }else{ // центр шара вне прямоугольника
                                        if(fabs(lx)<o2->lx)
                                        { // в пределах оси x -- столкновение с гранью y
                                            my_double3 pp;
                                            double cd = sqrt(dist*dist+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                                            if(cd<o1->lx)
                                            {
                                                if(ly<0)
                                                {
                                                        pp = o2->X+o2->ox*lx-o2->oy*o2->ly;
                                                }else
                                                {
                                                        pp = o2->X+o2->ox*lx+o2->oy*o2->ly;
                                                };
                                                double pl = o1->lx-cd;
                                                my_double3 pd = o1->X - pp;
                                                pd = __NORMALIZE(pd);
                                                ObjCollide(o1,o2,pp,pd,pl);
                                            };
                                        }else if(fabs(ly)<o2->ly){
                                        // в пределах оси y -- столкновение с гранью x
                                            my_double3 pp;
                                            double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx));
                                            if(cd<o1->lx)
                                            {
                                                if(lx<0)
                                                {
                                                        pp = o2->X+o2->oy*ly-o2->ox*o2->lx;
                                                }else
                                                {
                                                        pp = o2->X+o2->oy*ly+o2->ox*o2->lx;
                                                };
                                                double pl = o1->lx-cd;
                                                my_double3 pd = o1->X - pp;
                                                pd = __NORMALIZE(pd);
                                                ObjCollide(o1,o2,pp,pd,pl);
                                            };
                                        }else{
                                        // столкновение с углом?
                                            my_double3 pp;
                                            double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx)+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                                            if(cd<o1->lx)
                                            {
                                                pp = o2->X;
                                                if(lx<0)
                                                {
                                                    pp -= o2->ox*o2->lx;
                                                }else{
                                                    pp += o2->ox*o2->lx;
                                                };
                                                if(ly<0)
                                                {
                                                    pp -= o2->oy*o2->ly;
                                                }else{
                                                    pp += o2->oy*o2->ly;
                                                };
                                                double pl = o1->lx-cd;
                                                my_double3 pd = o1->X - pp;
                                                pd = __NORMALIZE(pd);
                                                ObjCollide(o1,o2,pp,pd,pl);
                                            };
                                        };
                                    }; // if(fabs(lx)<o2->lx&&fabs(ly)<o2->ly)
                                };// есть пересечение
                            }; // шарик с плоскостью
                            if(lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_MESH)
                            { // mesh'ы
                                o1=lv_o1; o2=lv_o2;
// ============================================================================================
// ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
// ============================================================================================

int mink_c = 0; // количество точек в разности минковского
my_double3 p1,p2; // точки из объектов, для получения разности
my_double3 minkowski[64*64];  // разность минкосвского
cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов

// для определения стартового симплекса:
int Q_maxx = -1;
int Q_maxy = -1;
int Q_maxz = -1;
float maxx = -SURE_R_MAXDISTANCE;
float maxy = -SURE_R_MAXDISTANCE;
float maxz = -SURE_R_MAXDISTANCE;

for(int i=0;i<lv_o1->vertex_count;++i)for(int j=0;j<lv_o2->vertex_count;++j)
{
    __GET_VERTEX(p1,lv_o1->vertex_start+i);
    __GET_VERTEX(p2,lv_o2->vertex_start+j);
    minkowski[i*lv_o2->vertex_count+j]=p1-p2;
    mink_c++;
    // Ищем точки с максимальными x,y,z

};

// стартовый тетраэдр -- {crs = дальняя точка в нправлении cross(max_x-max_y,max_z-max_y)}
// основание max_y, max_x, max_z
// max_y, max_z, crs
// max_z, max_x, crs
// max_x, max_y, crs

// ищем тетраэдр охватвающий 0,0




                            };// mesh'ы
                            if(lv_o1->type==SURE_OBJ_PLANE&&lv_o2->type==SURE_OBJ_PLANE)
                            { // квадратик с квадратиком
                                o1=lv_o1; o2=lv_o2;
// ============================================================================================
// ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
// ============================================================================================
my_double3 mesh1[8];
my_double3 mesh2[8];
int cover[64*64][3];
int tcover[64*64][3];

mesh1[0] = o1->X+o1->ox*o1->lx+o1->oy*o1->ly+o1->oz*o1->lz;
mesh1[1] = o1->X+o1->ox*o1->lx+o1->oy*o1->ly-o1->oz*o1->lz;
mesh1[2] = o1->X+o1->ox*o1->lx-o1->oy*o1->ly+o1->oz*o1->lz;
mesh1[3] = o1->X+o1->ox*o1->lx-o1->oy*o1->ly-o1->oz*o1->lz;
mesh1[4] = o1->X-o1->ox*o1->lx+o1->oy*o1->ly+o1->oz*o1->lz;
mesh1[5] = o1->X-o1->ox*o1->lx+o1->oy*o1->ly-o1->oz*o1->lz;
mesh1[6] = o1->X-o1->ox*o1->lx-o1->oy*o1->ly+o1->oz*o1->lz;
mesh1[7] = o1->X-o1->ox*o1->lx-o1->oy*o1->ly-o1->oz*o1->lz;

mesh2[0] = o2->X+o2->ox*o2->lx+o2->oy*o2->ly+o2->oz*o2->lz;
mesh2[1] = o2->X+o2->ox*o2->lx+o2->oy*o2->ly-o2->oz*o2->lz;
mesh2[2] = o2->X+o2->ox*o2->lx-o2->oy*o2->ly+o2->oz*o2->lz;
mesh2[3] = o2->X+o2->ox*o2->lx-o2->oy*o2->ly-o2->oz*o2->lz;
mesh2[4] = o2->X-o2->ox*o2->lx+o2->oy*o2->ly+o2->oz*o2->lz;
mesh2[5] = o2->X-o2->ox*o2->lx+o2->oy*o2->ly-o2->oz*o2->lz;
mesh2[6] = o2->X-o2->ox*o2->lx-o2->oy*o2->ly+o2->oz*o2->lz;
mesh2[7] = o2->X-o2->ox*o2->lx-o2->oy*o2->ly-o2->oz*o2->lz;

my_double3 minkowski[8*8];

for(int i=0;i<8;++i)for(int j=0;j<8;++j)
	minkowski[i*8+j]=mesh1[i]-mesh2[j];

int Q[4];
int QF[4];

// берем тэтраэдр из множества Минковского
Q[0]=0;
Q[1]=1;
Q[2]=2;
Q[3]=3;

bool fin = false; // индикатор завершения работы алгоритма

while(!fin)
{
    // проверяем, находится ли начало координат внутри тэтраэдра
    my_double3 n;
    my_double3 nf;
    my_double3 p;
    double dist;
    double distf;
    double dist_min = SURE_R_MAXDISTANCE;

    // нужно всех плоскостях тэтраэдра найти точку ближайшую к 0,0
    // плоскость 1, задаем номалью и точкой
    n = cross(minkowski[Q[1]]-minkowski[Q[0]],minkowski[Q[2]]-minkowski[Q[0]]);
    n = __NORMALIZE(n);
    p = minkowski[Q[0]];
    dist = -dot(p,n);
    if(fabs(dist)<dist_min)
    {
        dist_min = fabs(dist);
        QF[0] = Q[0];QF[1] = Q[1];QF[2] = Q[2];QF[3] = Q[3];
        nf = n;
        distf = dist;
    };

    // плоскость 2,
    n = cross(minkowski[Q[2]]-minkowski[Q[1]],minkowski[Q[3]]-minkowski[Q[1]]);
    n = __NORMALIZE(n);
    p = minkowski[Q[1]];
    dist = -dot(p,n);
    if(fabs(dist)<dist_min)
    {
        dist_min = fabs(dist);
        QF[0] = Q[1];QF[1] = Q[2];QF[2] = Q[3];QF[3] = Q[0];
        nf = n;
        distf = dist;
    };

    // плоскость 3
    n = cross(minkowski[Q[3]]-minkowski[Q[2]],minkowski[Q[0]]-minkowski[Q[2]]);
    n = __NORMALIZE(n);
    p = minkowski[Q[2]];
    dist = -dot(p,n);
    if(fabs(dist)<dist_min)
    {
        dist_min = fabs(dist);
        QF[0] = Q[2];QF[1] = Q[3];QF[2] = Q[0];QF[3] = Q[1];
        nf = n;
        distf = dist;
    };

    // плоскость 4
    n = cross(minkowski[Q[0]]-minkowski[Q[3]],minkowski[Q[1]]-minkowski[Q[3]]);
    n = __NORMALIZE(n);
    p = minkowski[Q[3]];
    dist = -dot(p,n);
    if(fabs(dist)<dist_min)
    {
        dist_min = fabs(dist);
        QF[0] = Q[3];QF[1] = Q[0];QF[2] = Q[1];QF[3] = Q[2];
        nf = n;
        distf = dist;
    };

    // QF[0..2] -- плоскость ближайшая к 0,0
    // nf -- нормаль
    // dist_min -- расстояние до 0,0.
    // QF[3] нужно для отсечения пройденных граней

    //выражаем точку 0,0 в координатах тэтраэдра
    // для этого решаем систему уравнений v1*x+v2*y+v3*z=vk где
    // v1 = Q1-Q0 v2 = Q2-Q0 v3 = Q3-Q0 vk = [0,0]-Q0
    my_double3 v1 = minkowski[QF[1]]-minkowski[QF[0]];
    my_double3 v2 = minkowski[QF[2]]-minkowski[QF[0]];
    my_double3 v3 = minkowski[QF[3]]-minkowski[QF[0]];

    double k1 = -minkowski[QF[0]].x;
    double k2 = -minkowski[QF[0]].y;
    double k3 = -minkowski[QF[0]].z;

    double mo = fabs(v1.x*v2.y*v3.z-v1.y*v2.z*v3.x-v2.x*v3.y*v1.z-v1.z*v2.y*v3.x+v1.y*v2.x*v3.z+v2.z*v3.y*v1.x);
    // определитель матрицы

    if(mo>SURE_P_DELTA)
    {   // векторы образуют базис
        // Нужно упорядочить векторы таким образом, чтобы на главной диагонали матрицы не было нулей
        my_double3 tX;
        if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // A B C
        {   tX=v1; v1=v2; v2=v3; v3=tX;
            if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B C A
            {   tX=v1; v1=v2; v2=v3; v3=tX;
                if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C A B
                {   tX=v1; v1=v2; v2=tX;
                    if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C B A
                    {   tX=v1; v1=v2; v2=v3; v3=tX;
                        if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B A C
                        { tX=v1; v1=v2; v2=v3;v3=tX; /* A C B */ };
                    };
                };
            };
        };

        // решаем методом Гаусса
        v2.x/=v1.x; v3.x/=v1.x; k1/=v1.x; v1.x=1;
        v2.y-=v2.x*v1.y; v3.y-=v3.x*v1.y; k2-=k1*v1.y; v1.y=0;
        v2.z-=v2.x*v1.z; v3.z-=v3.x*v1.z; k3-=k1*v1.z; v1.z=0;
        v3.y/=v2.y; k2/=v2.y; v2.y=1;
        v3.x-=v3.y*v2.x; k1-=k2*v2.x; v2.x=0;
        v3.z-=v3.y*v2.z; k3-=k2*v2.z; v2.z=0;
        k3/=v3.z; v3.z=1;
        k2-=k3*v3.y; v3.y=0;
        k1-=k3*v3.x; v3.x=0;

    }else
    { // определитель = 0
     // если векторы лежат на одной плоскости -- считаем, что в таком тэтраэдре не может лежать точка 0,0
     // фактически, так мы игнорируем практически нулевые "касания" плоскими гранями.
     // ну и хер с ними.
        k1 = -1.0;
        k2 = -1.0;
        k3 = -1.0;
    };

    if(((k1+k2+k3)<1.0)&&k1>0&&k2>0&&k3>0)
    { // точка 0,0 внутри тэтраэдра
    // QF[0..3] -- тэтраэдр, внутри которого находится точка 0,0

    std::cout << "Разность Минковского:\n";

    for(int kk = 0;kk<8*8;++kk)
    {
        std::cout << kk << ";" << minkowski[kk].x << ";" << minkowski[kk].y << ";" << minkowski[kk].z << "\n";
    };

    int cover_c = 0;

    // в cover складываем 4 грани:

    #define __ADD_TO_COVER(A,B,C) \
    if(dot(cross(minkowski[QF[B]]-minkowski[QF[A]],minkowski[QF[C]]-minkowski[QF[A]]),-minkowski[QF[A]])>0) \
    { cover[cover_c][0] = QF[A]; cover[cover_c][1] = QF[C]; cover[cover_c][2] = QF[B]; \
    }else { cover[cover_c][0] = QF[A]; cover[cover_c][1] = QF[B]; cover[cover_c][2] = QF[C]; }; \
    ++cover_c;

/*
    // проверяем, чтобы гнрань смотрела "наружу"
    if(dot(cross(minkowski[QF[1]]-minkowski[QF[0]],minkowski[QF[2]]-minkowski[QF[0]]),-minkowski[QF[0]])>0)
    { // точка "снаружи" -- грань инвертируем
        cover[cover_c][0] = QF[0];
        cover[cover_c][1] = QF[2];
        cover[cover_c][2] = QF[1];
    }else
    { // точка внутри -- добавляем грань как есть
        cover[cover_c][0] = QF[0];
        cover[cover_c][1] = QF[1];
        cover[cover_c][2] = QF[2];
    };
    ++cover_c;
*/
    __ADD_TO_COVER(0,1,2);
    __ADD_TO_COVER(0,1,3);
    __ADD_TO_COVER(0,2,3);
    __ADD_TO_COVER(1,2,3);

    std::cout << "Стартовый симплекс:\n";
    for(int cci=0;cci<cover_c;++cci)
    {
        std::cout << cci << ";" << minkowski[cover[cci][0]].x << ";" << minkowski[cover[cci][0]].y << ";" << minkowski[cover[cci][0]].z << "\n";
        std::cout << cci << ";" << minkowski[cover[cci][1]].x << ";" << minkowski[cover[cci][1]].y << ";" << minkowski[cover[cci][1]].z << "\n";
        std::cout << cci << ";" << minkowski[cover[cci][2]].x << ";" << minkowski[cover[cci][2]].y << ";" << minkowski[cover[cci][2]].z << "\n";
        std::cout << cci << ";" << minkowski[cover[cci][0]].x << ";" << minkowski[cover[cci][0]].y << ";" << minkowski[cover[cci][0]].z << "\n";
    };

    bool found = false;

    while(!found)
    {
        // ищем грань ближайшую к 0 0
        std::cout << "Ищем грань ближайшую к 0 0:\n";
        double dist_min = SURE_R_MAXDISTANCE;
        for(int ig=0;ig<cover_c;++ig)
        { // для каждой грани:
            std::cout << ig << "=";
            n = __NORMALIZE(cross(minkowski[cover[ig][1]]-minkowski[cover[ig][0]],
                                   minkowski[cover[ig][2]]-minkowski[cover[ig][0]]));
            double dist_00 = dot(n, minkowski[cover[ig][0]]);
            std::cout << dist_00 << ";";
            if(dist_00<dist_min)
            {
                dist_min = dist_00;
                nf = n;
                QF[0] = cover[ig][0];
                QF[1] = cover[ig][1];
                QF[2] = cover[ig][2];
            }
        };// для каждой грани

        std::cout << "min=" << dist_min << "\n";

        // ищем есть ли точка "дальше" этой грани от 0,0
        std::cout << "Ищем есть ли точка дальше этой грани от 0,0:\n";
        int QN = -1;
        n = cross(minkowski[QF[1]]-minkowski[QF[0]],minkowski[QF[2]]-minkowski[QF[0]]);
        double dd_max = dot(n,minkowski[QF[0]]) + SURE_P_DELTA;
        std::cout << "n,dist;" << n.x << ";" << n.y << ";" << n.z << ";" << dd_max << "\n";

        std::cout << "Проверяем каждую точку:\n";
        for(int i=0;i<8*8;++i)
        {
          bool incover = false;
          std::cout << i << ";";
          for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
            if(i==cover[ic][icp])
            {
                std::cout << "уже в оболочке:" << ic << ":" << icp;
                incover = true; ic = cover_c; icp = 3;
            };
          if(!incover)
          { // точка не должна буть уже в оболочке
            double dd = dot(n,minkowski[i]);
            std::cout << "d;" << dd << ";";
            if(dd>dd_max)
            { // нашли подходящий
              dd_max = dd + SURE_P_DELTA;
              QN = i;
              std::cout << "нашли;" << QN;
            };
          };
          std::cout << "\n";
        }; // перебор всех точке множества

        if(QN<0)
        {
            // если нет -- завершили цикл:
            std::cout << "Не нашли -- завершили цикл\n";
            found = true;
        }else
        { // если да -- берем эту точку. minkowski[QN]
            int tcover_c = 0;
            std::cout << "Нашли:\n";
            std::cout << "Проверяем все грани:\n";
            for(int icc=0;icc<cover_c;++icc)
            { // для всех граней cover
                std::cout << icc << ";" << minkowski[cover[icc][0]].x << ";" << minkowski[cover[icc][0]].y << ";" << minkowski[cover[icc][0]].z << "\n";
                std::cout << icc << ";" << minkowski[cover[icc][1]].x << ";" << minkowski[cover[icc][1]].y << ";" << minkowski[cover[icc][1]].z << "\n";
                std::cout << icc << ";" << minkowski[cover[icc][2]].x << ";" << minkowski[cover[icc][2]].y << ";" << minkowski[cover[icc][2]].z << "\n";
                n = cross(minkowski[cover[icc][1]]-minkowski[cover[icc][0]],
                          minkowski[cover[icc][2]]-minkowski[cover[icc][0]]);
                double d = dot(n,minkowski[QN]-minkowski[cover[icc][0]]);
                if(d<0)
                { // cover -> tcover все невидимые грани
                    std::cout << icc << " невидима:" << d << "; добавляем как № " << tcover_c << "\n";
                    tcover[tcover_c][0] = cover[icc][0];
                    tcover[tcover_c][1] = cover[icc][1];
                    tcover[tcover_c][2] = cover[icc][2];
                    tcover_c++;
                }else
                { // если грань видимая -- то для каждого ребра:
                    bool inside;
                    std::cout << icc << " видима:" << d << "анализируем: \n";
                    tcover[tcover_c][0] = cover[icc][0];
                    tcover[tcover_c][1] = cover[icc][1];
                    tcover[tcover_c][2] = QN;

                    std::cout << icc << ";" << minkowski[tcover[tcover_c][0]].x << ";" << minkowski[tcover[tcover_c][0]].y << ";" << minkowski[tcover[tcover_c][0]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][1]].x << ";" << minkowski[tcover[tcover_c][1]].y << ";" << minkowski[tcover[tcover_c][1]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][2]].x << ";" << minkowski[tcover[tcover_c][2]].y << ";" << minkowski[tcover[tcover_c][2]].z << "\n";

                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(minkowski[tcover[tcover_c][1]]-minkowski[tcover[tcover_c][0]],
                              minkowski[tcover[tcover_c][2]]-minkowski[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,minkowski[cover[ic][icp]]-minkowski[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      {
                      std::cout << icc << "внутренняя (" << d << ")для " << cover[ic][icp] << ";" << minkowski[cover[ic][icp]].x << ";" << minkowski[cover[ic][icp]].y << ";" << minkowski[cover[ic][icp]].z << "\n";
                      inside = false; ic = cover_c; icp = 3; };
                      };
                    // если все внутри -- грань в cover
                    if(inside)
                    {
                        std::cout << icc << "внешняя:" << d << "; добавляем как № " << tcover_c << "\n";
                        ++tcover_c;
                    };

                    tcover[tcover_c][0] = cover[icc][1];
                    tcover[tcover_c][1] = cover[icc][2];
                    tcover[tcover_c][2] = QN;
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][0]].x << ";" << minkowski[tcover[tcover_c][0]].y << ";" << minkowski[tcover[tcover_c][0]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][1]].x << ";" << minkowski[tcover[tcover_c][1]].y << ";" << minkowski[tcover[tcover_c][1]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][2]].x << ";" << minkowski[tcover[tcover_c][2]].y << ";" << minkowski[tcover[tcover_c][2]].z << "\n";

                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(minkowski[tcover[tcover_c][1]]-minkowski[tcover[tcover_c][0]],
                              minkowski[tcover[tcover_c][2]]-minkowski[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,minkowski[cover[ic][icp]]-minkowski[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      {
                      std::cout << icc << "внутренняя (" << d << ")для " << cover[ic][icp] << ";" << minkowski[cover[ic][icp]].x << ";" << minkowski[cover[ic][icp]].y << ";" << minkowski[cover[ic][icp]].z << "\n";
                      inside = false; ic = cover_c; icp = 3; };
                      };
                    // если все внутри -- грань в cover
                    if(inside)
                    {
                        std::cout << icc << "внешняя:" << d << "; добавляем как № " << tcover_c << "\n";
                        ++tcover_c;
                    };

                    tcover[tcover_c][0] = cover[icc][2];
                    tcover[tcover_c][1] = cover[icc][0];
                    tcover[tcover_c][2] = QN;
                    // строим грань с новой точкой и проверяем все точки cover
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][0]].x << ";" << minkowski[tcover[tcover_c][0]].y << ";" << minkowski[tcover[tcover_c][0]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][1]].x << ";" << minkowski[tcover[tcover_c][1]].y << ";" << minkowski[tcover[tcover_c][1]].z << "\n";
                    std::cout << icc << ";" << minkowski[tcover[tcover_c][2]].x << ";" << minkowski[tcover[tcover_c][2]].y << ";" << minkowski[tcover[tcover_c][2]].z << "\n";

                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(minkowski[tcover[tcover_c][1]]-minkowski[tcover[tcover_c][0]],
                              minkowski[tcover[tcover_c][2]]-minkowski[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,minkowski[cover[ic][icp]]-minkowski[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      {
                      std::cout << icc << "внутренняя (" << d << ")для " << cover[ic][icp] << ";" << minkowski[cover[ic][icp]].x << ";" << minkowski[cover[ic][icp]].y << ";" << minkowski[cover[ic][icp]].z << "\n";
                      inside = false; ic = cover_c; icp = 3; };
                      };
                    // если все внутри -- грань в cover
                    if(inside)
                    {
                        std::cout << icc << "внешняя:" << d << "; добавляем как № " << tcover_c << "\n";
                        ++tcover_c;
                    };

                }; // если грань видимая -- то для каждого ребра
            }; // для всех граней cover

            for(int ic = 0;ic<tcover_c;++ic)for(int icp=0;icp<3;++icp)
            {
                cover[ic][icp] = tcover[ic][icp];
            };
            cover_c = tcover_c;

            std::cout << "Получили симплекс:\n";
            for(int cci=0;cci<cover_c;++cci)
            {
                std::cout << cci << ";" << minkowski[cover[cci][0]].x << ";" << minkowski[cover[cci][0]].y << ";" << minkowski[cover[cci][0]].z << "\n";
                std::cout << cci << ";" << minkowski[cover[cci][1]].x << ";" << minkowski[cover[cci][1]].y << ";" << minkowski[cover[cci][1]].z << "\n";
                std::cout << cci << ";" << minkowski[cover[cci][2]].x << ";" << minkowski[cover[cci][2]].y << ";" << minkowski[cover[cci][2]].z << "\n";
                std::cout << cci << ";" << minkowski[cover[cci][0]].x << ";" << minkowski[cover[cci][0]].y << ";" << minkowski[cover[cci][0]].z << "\n";
            };

        }; // если нужно наращивать "оболочку"
    }; // while(!found)

    // Нужно найти глубину проникновения и точку контакта
    // p = 0,0 - n * dist;
    // p выражаем в базисе векторов грани.


    // QF[0] QF[1] QF[2]
    std::cout << "Искомая грань:\n";
    std::cout << minkowski[QF[0]].x << ";" << minkowski[QF[0]].y << ";" << minkowski[QF[0]].z << "\n";
    std::cout << minkowski[QF[1]].x << ";" << minkowski[QF[1]].y << ";" << minkowski[QF[1]].z << "\n";
    std::cout << minkowski[QF[2]].x << ";" << minkowski[QF[2]].y << ";" << minkowski[QF[2]].z << "\n";

    int mesh2_0 = QF[0] % 8;
    int mesh1_0 = ( QF[0] - QF[0] % 8 ) / 8;

    int mesh2_1 = QF[1] % 8;
    int mesh1_1 = ( QF[1] - QF[1] % 8 ) / 8;

    int mesh2_2 = QF[2] % 8;
    int mesh1_2 = ( QF[2] - QF[2] % 8 ) / 8;

    dist = dot(nf,minkowski[QF[0]]);
    std::cout << "dist=" << dist << "; n:\n";
    std::cout << nf.x << ";" << nf.y << ";" << nf.z << "\n";

    my_double3 pp = nf*dist;
    std::cout << "pp:\n";
    std::cout << pp.x << ";" << pp.y << ";" << pp.z << "\n";


    v1 = minkowski[QF[1]]-minkowski[QF[0]];
    v2 = minkowski[QF[2]]-minkowski[QF[0]];
    v3 = cross(v1,v2);

    k1 = pp.x-minkowski[QF[0]].x;
    k2 = pp.y-minkowski[QF[0]].y;
    k3 = pp.z-minkowski[QF[0]].z;

        my_double3 tX;
        if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // A B C
        {   tX=v1; v1=v2; v2=v3; v3=tX;
            if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B C A
            {   tX=v1; v1=v2; v2=v3; v3=tX;
                if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C A B
                {   tX=v1; v1=v2; v2=tX;
                    if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C B A
                    {   tX=v1; v1=v2; v2=v3; v3=tX;
                        if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B A C
                        { tX=v1; v1=v2; v2=v3;v3=tX; /* A C B */ };
                    };
                };
            };
        };

        // решаем методом Гаусса
        v2.x/=v1.x; v3.x/=v1.x; k1/=v1.x; v1.x=1;
        v2.y-=v2.x*v1.y; v3.y-=v3.x*v1.y; k2-=k1*v1.y; v1.y=0;
        v2.z-=v2.x*v1.z; v3.z-=v3.x*v1.z; k3-=k1*v1.z; v1.z=0;
        v3.y/=v2.y; k2/=v2.y; v2.y=1;
        v3.x-=v3.y*v2.x; k1-=k2*v2.x; v2.x=0;
        v3.z-=v3.y*v2.z; k3-=k2*v2.z; v2.z=0;
        k3/=v3.z; v3.z=1;
        k2-=k3*v3.y; v3.y=0;
        k1-=k3*v3.x; v3.x=0;

    if(fabs(k1)<SURE_P_DELTA)k1=0;
    if(fabs(k2)<SURE_P_DELTA)k2=0;
    if(fabs(k3)<SURE_P_DELTA)k3=0;

    std::cout << "Выразили вектор:\n";
    std::cout << k1 << ";" << k2 << ";" << k3 << "\n";

    dist_min = dist+SURE_P_DELTA;

    if((k1<0)||(k2<0)||(k3<0))
    {
        std::cout << "Не та грань :( Пробуем другие для d=" << dist_min << "\n";
        for(int ig=0;ig<cover_c;++ig)
        { // для каждой грани:
            std::cout << ig << "=";
            n = __NORMALIZE(cross(minkowski[cover[ig][1]]-minkowski[cover[ig][0]],
                                   minkowski[cover[ig][2]]-minkowski[cover[ig][0]]));
            double dist_00 = dot(n, minkowski[cover[ig][0]]);
            std::cout << dist_00 << ";";
            if(dist_00<=dist_min)
            {
                nf = n;
                QF[0] = cover[ig][0];
                QF[1] = cover[ig][1];
                QF[2] = cover[ig][2];
                std::cout << "пробуем:\n";

                v1 = minkowski[QF[1]]-minkowski[QF[0]];
                v2 = minkowski[QF[2]]-minkowski[QF[0]];
                v3 = cross(v1,v2);

                k1 = pp.x-minkowski[QF[0]].x;
                k2 = pp.y-minkowski[QF[0]].y;
                k3 = pp.z-minkowski[QF[0]].z;

                    my_double3 tX;
                    if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // A B C
                    {   tX=v1; v1=v2; v2=v3; v3=tX;
                        if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B C A
                        {   tX=v1; v1=v2; v2=v3; v3=tX;
                            if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C A B
                            {   tX=v1; v1=v2; v2=tX;
                                if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // C B A
                                {   tX=v1; v1=v2; v2=v3; v3=tX;
                                    if(fabs(v1.x)<SURE_P_DELTA||fabs(v2.y)<SURE_P_DELTA||fabs(v3.z)<SURE_P_DELTA) // B A C
                                    { tX=v1; v1=v2; v2=v3;v3=tX; /* A C B */ };
                                };
                            };
                        };
                    };

                    // решаем методом Гаусса
                    v2.x/=v1.x; v3.x/=v1.x; k1/=v1.x; v1.x=1;
                    v2.y-=v2.x*v1.y; v3.y-=v3.x*v1.y; k2-=k1*v1.y; v1.y=0;
                    v2.z-=v2.x*v1.z; v3.z-=v3.x*v1.z; k3-=k1*v1.z; v1.z=0;
                    v3.y/=v2.y; k2/=v2.y; v2.y=1;
                    v3.x-=v3.y*v2.x; k1-=k2*v2.x; v2.x=0;
                    v3.z-=v3.y*v2.z; k3-=k2*v2.z; v2.z=0;
                    k3/=v3.z; v3.z=1;
                    k2-=k3*v3.y; v3.y=0;
                    k1-=k3*v3.x; v3.x=0;

                if(fabs(k1)<SURE_P_DELTA)k1=0;
                if(fabs(k2)<SURE_P_DELTA)k2=0;
                if(fabs(k3)<SURE_P_DELTA)k3=0;

                std::cout << "Выразили вектор:\n";
                std::cout << k1 << ";" << k2 << ";" << k3 << "\n";
                if((k1>=0)&&(k2>=0)&&(k3>=0))
                {
                    std::cout << "Нашли!\n";
                    ig = cover_c;
                };
            }; // if(dist_00<=dist_min)
            std::cout << "\n";
        };// для каждой грани

    std::cout << "Искомая грань:\n";
    std::cout << minkowski[QF[0]].x << ";" << minkowski[QF[0]].y << ";" << minkowski[QF[0]].z << "\n";
    std::cout << minkowski[QF[1]].x << ";" << minkowski[QF[1]].y << ";" << minkowski[QF[1]].z << "\n";
    std::cout << minkowski[QF[2]].x << ";" << minkowski[QF[2]].y << ";" << minkowski[QF[2]].z << "\n";

    mesh2_0 = QF[0] % 8;
    mesh1_0 = ( QF[0] - QF[0] % 8 ) / 8;

    mesh2_1 = QF[1] % 8;
    mesh1_1 = ( QF[1] - QF[1] % 8 ) / 8;

    mesh2_2 = QF[2] % 8;
    mesh1_2 = ( QF[2] - QF[2] % 8 ) / 8;

    dist = dot(nf,minkowski[QF[0]]);
    std::cout << "dist=" << dist << "; n:\n";
    std::cout << nf.x << ";" << nf.y << ";" << nf.z << "\n";

    my_double3 pp = nf*dist;
    std::cout << "pp:\n";
    std::cout << pp.x << ";" << pp.y << ";" << pp.z << "\n";

    }; // не та грань


    std::cout << "Сторона объекта 1:\n";
    std::cout << "0;" << mesh1[mesh1_0].x << ";" << mesh1[mesh1_0].y << ";" << mesh1[mesh1_0].z << "\n";
    std::cout << "1;" << mesh1[mesh1_1].x << ";" << mesh1[mesh1_1].y << ";" << mesh1[mesh1_1].z << "\n";
    std::cout << "2;" << mesh1[mesh1_2].x << ";" << mesh1[mesh1_2].y << ";" << mesh1[mesh1_2].z << "\n";

    std::cout << "Сторона объекта 2:\n";
    std::cout << "0;" << mesh2[mesh2_0].x << ";" << mesh2[mesh2_0].y << ";" << mesh2[mesh2_0].z << "\n";
    std::cout << "1;" << mesh2[mesh2_1].x << ";" << mesh2[mesh2_1].y << ";" << mesh2[mesh2_1].z << "\n";
    std::cout << "2;" << mesh2[mesh2_2].x << ";" << mesh2[mesh2_2].y << ";" << mesh2[mesh2_2].z << "\n";

    my_double3 pp1 = mesh1[mesh1_0]+(mesh1[mesh1_1]-mesh1[mesh1_0])*k1 + (mesh1[mesh1_2]-mesh1[mesh1_0])*k2;
    my_double3 pp2 = mesh2[mesh2_0]+(mesh2[mesh2_1]-mesh2[mesh2_0])*k1 + (mesh2[mesh2_2]-mesh2[mesh2_0])*k2;

    my_double3 pd = -nf;
    double pl = dist;

    if((mesh1_0==mesh1_1)&&(mesh1_0==mesh1_2))
    {
        pp = mesh1[mesh1_0];
        ObjCollide(o1,o2,pp,pd,pl);
    }else if((mesh2_0==mesh2_1)&&(mesh2_0==mesh2_2))
    {
        pp = mesh2[mesh2_0];
        ObjCollide(o1,o2,pp,pd,pl);
    }else
    {
        pp = ( pp1 + pp2 ) * 0.5;
    };

    /*
    std::cout << "Разность Минковского:\n";

    for(int kk = 0;kk<8*8;++kk)
    {
        std::cout << kk << ";" << minkowski[kk].x << ";" << minkowski[kk].y << ";" << minkowski[kk].z << "\n";
    };
    */



    fin = true;

    }else
    {// точка 0,0 снаружи тэтраэдра

        if(distf<0){nf = -nf;dist=-distf;};
        // я точно знаю, что точка снаружи тэтраэдра, поэтому если dist<0 значит нужно развернуть нормаль.
        // нормаль направлена наружу

        double dd_max = -SURE_R_MAXDISTANCE;

        double dd;

        for(int d=0;d<4;++d)
        {
            dd = dot(nf,minkowski[QF[d]]);
            if(dd>dd_max){dd_max = dd;};
        };

        int QN = -1;

        for(int i=0;i<8*8;++i)
        {
          if(!((i==QF[0])||
               (i==QF[1])||
               (i==QF[2])||
               (i==QF[3])))
          { // не одна из найденных точек
            dd = dot(nf,minkowski[i]);
            if(dd>dd_max)
            { // нашли подходящий
              dd_max = dd;
              QN = i;
            };
          };
        }; // перебор всех точке множества

        if(QN==-1)
        {
          fin = true; // все. Не пересеклись. расстояние между ближайшими точками -- dist
        }else
        { // нашли новую точку
          Q[0] = QF[0];
          Q[1] = QF[1];
          Q[2] = QF[2];
          Q[3] = QN;
        };
    }; // точка 0,0 внутри/снаружи тэтраэдра
}; // !fin

// ============================================================================================
// ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
// ============================================================================================

                            }; // квадратик с квадратиком
                            if((lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_CUBE)||
                               (lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_CUBE))
                            {   // шарик с кубом
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_CUBE)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
                                // o1 - сфера
                                // o2 - куб



                            }; // шарик с кубом
                        }; // if(lv_o2->collidable&&lv_o2!=lv_o1)
                    }; //for(int j = 0;j<EngineData->m_objects;++j)
                }; // if(lv_o1->movable&&lv_o1->collidable)
            };//  for(int i = 0;i<EngineData->m_objects;++i)
            } // количество итераций

            EngineData->reset = true;
        }; // !paused
        // BEG Поворот и перемещение камеры:
        my_double3 dx = EngineData->cam_vec;
        my_double3 dz = EngineData->cam_upvec;
        my_double3 dy;
        dy = cross(dz,dx);

        EngineData->cam_x.x += dx.x*EngineData->cam_dx.x+dy.x*EngineData->cam_dx.y+dz.x*EngineData->cam_dx.z;
        EngineData->cam_x.y += dx.y*EngineData->cam_dx.x+dy.y*EngineData->cam_dx.y+dz.y*EngineData->cam_dx.z;
        EngineData->cam_x.z += dx.z*EngineData->cam_dx.x+dy.z*EngineData->cam_dx.y+dz.z*EngineData->cam_dx.z;

        if(EngineData->cam_dw.z!=0||EngineData->cam_dw.y!=0||EngineData->cam_dw.x!=0)
        {
            my_double3 dx1;
            my_double3 dz1;
            cl_double c0 = cos(EngineData->cam_dw.x);
            cl_double s0 = sin(EngineData->cam_dw.x);
            cl_double c1 = cos(EngineData->cam_dw.y);
            cl_double s1 = sin(EngineData->cam_dw.y);
            cl_double c2 = cos(EngineData->cam_dw.z);
            cl_double s2 = sin(EngineData->cam_dw.z);
            dx1 = dx*c2+dy*s2+dz*s1;
            dz1 = dz*c1-dx*(s1*c0)+dy*(c2*s0)-dx*(s2*s0);

            EngineData->cam_upvec = __NORMALIZE(dz1);
            EngineData->cam_vec = __NORMALIZE(dx1);

            EngineData->reset = true;
        };
        //EngineData->cam_dw[0] = 0;
        EngineData->cam_dw.y = 0;
        EngineData->cam_dw.z = 0;

        if(EngineData->cam_dx.x!=0||EngineData->cam_dx.y!=0||EngineData->cam_dx.z!=0)
            EngineData->reset = true;

        // END Поворот и перемещение камеры

        drawscene();// Данные -- в GPU
        clock_gettime(CLOCK_MONOTONIC,&frametime);
        EngineData->frametime = frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );
        if(EngineData->frametime<SURE_P_DELAY)
            msleep((int)(SURE_P_DELAY-EngineData->frametime));
    };
}

void SurePhysThread::drawscene()
{
    GPUData->cam_x = EngineData->cam_x;
    GPUData->cam_vec = EngineData->cam_vec;
    GPUData->cam_upvec = EngineData->cam_upvec;
    GPUData->xy_h = EngineData->xy_h;
    GPUData->r_maxiters = EngineData->r_iters;
    GPUData->r_rechecks = EngineData->r_rechecks;
    GPUData->r_backlight = EngineData->r_backlight;
    if(EngineData->reset)GPUData->toreset=true;
    EngineData->reset = false;

    int i = 0;
    GPUData->m_drawables = 0;

    // Воздух
    GPUData->Drawables[i].X.s[0] = 0; //Координаты центра
    GPUData->Drawables[i].X.s[1] = 0;
    GPUData->Drawables[i].X.s[2] = 0;
    GPUData->Drawables[i].ox.s[0] = 1; //Локальная ось x
    GPUData->Drawables[i].ox.s[1] = 0;
    GPUData->Drawables[i].ox.s[2] = 0;
    GPUData->Drawables[i].oy.s[0] = 0; //Локальная ось x
    GPUData->Drawables[i].oy.s[1] = 1;
    GPUData->Drawables[i].oy.s[2] = 0;
    GPUData->Drawables[i].oz.s[0] = 0; //Локальная ось x
    GPUData->Drawables[i].oz.s[1] = 0;
    GPUData->Drawables[i].oz.s[2] = 1;
    GPUData->Drawables[i].lx = 5.0; // длина
    GPUData->Drawables[i].ly = 1.0; // ширина
    GPUData->Drawables[i].lz = 1.0; // высота
    GPUData->Drawables[i].type = SURE_DR_NONE; // форма
    GPUData->Drawables[i].radiance = 0.0; // свечение
    GPUData->Drawables[i].transp = 1.5; // прозрачность
    GPUData->Drawables[i].transp_i = 1.1; // прозрачность
    GPUData->Drawables[i].refr = 1.0; // Коэффициент преломления
    GPUData->Drawables[i].dist_type = SURE_D_EQUAL; // тип рандомизации
    GPUData->Drawables[i].dist_sigma = 1.0; // sigma рандомизации
    GPUData->Drawables[i].dist_m = SURE_PI2; // матожидание рандомизации
    GPUData->Drawables[i].rgb.s[0] = 250; // цвет
    GPUData->Drawables[i].rgb.s[1] = 250;
    GPUData->Drawables[i].rgb.s[2] = 250;
    GPUData->Drawables[i].sided = true;
    GPUData->m_drawables++;

    for(int d = 0;d<EngineData->m_objects;++d)
    {
        switch(EngineData->objects[d].type){
            case SURE_OBJ_CUBE:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X + EngineData->objects[d].lx*EngineData->objects[d].ox;
                GPUData->Drawables[i].oz = EngineData->objects[d].ox;
                GPUData->Drawables[i].ox = EngineData->objects[d].oz;
                GPUData->Drawables[i].lx = EngineData->objects[d].lz;
                GPUData->m_drawables++;

                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X - EngineData->objects[d].lx*EngineData->objects[d].ox;
                GPUData->Drawables[i].oz = -EngineData->objects[d].ox;
                GPUData->Drawables[i].ox = EngineData->objects[d].oz;
                GPUData->Drawables[i].lx = EngineData->objects[d].lz;
                GPUData->m_drawables++;

                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X + EngineData->objects[d].ly*EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = EngineData->objects[d].oy;
                GPUData->Drawables[i].oy = EngineData->objects[d].oz;
                GPUData->Drawables[i].ly = EngineData->objects[d].lz;
                GPUData->m_drawables++;

                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X - EngineData->objects[d].ly*EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = -EngineData->objects[d].oy;
                GPUData->Drawables[i].oy = EngineData->objects[d].oz;
                GPUData->Drawables[i].ly = EngineData->objects[d].lz;
                GPUData->m_drawables++;

                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X + EngineData->objects[d].lz*EngineData->objects[d].oz;
                GPUData->m_drawables++;

                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X - EngineData->objects[d].lz*EngineData->objects[d].oz;
                GPUData->Drawables[i].oz = -EngineData->objects[d].oz;
                GPUData->m_drawables++;

                break;
            };
            case SURE_OBJ_SPHERE:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X;
                GPUData->Drawables[i].lx = 0.95*EngineData->objects[d].lx;
                GPUData->Drawables[i].ox = EngineData->objects[d].ox;
                GPUData->Drawables[i].oy = EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = EngineData->objects[d].oz;
                GPUData->m_drawables++;
                break;
            };
            /* case SURE_DR_MESH:
            {

                break;
            }; */
            default:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X;
                GPUData->Drawables[i].lx = EngineData->objects[d].lx;
                GPUData->Drawables[i].ox = EngineData->objects[d].ox;
                GPUData->Drawables[i].oy = EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = EngineData->objects[d].oz;
                GPUData->m_drawables++;
                break;
            };
            break;
        }; // case (type)
    };

}
