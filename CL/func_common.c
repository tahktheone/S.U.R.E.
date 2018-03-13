bool CollideRaySphered(__VTYPE3 tp,__VTYPE3 tv,__VTYPE3 o,__VTYPE r, __VTYPE2 *t, bool *in,__VTYPE* id)
{
    __VTYPE3 vtc = tp-o; //VecToCenter
    __VTYPE B = dot(vtc,tv);
    __VTYPE C = dot(vtc,vtc)-r*r;
    __VTYPE D = B*B-C;

    if(D>=0){
        __VTYPE kd = sqrt(D);
        __VTYPE t1 = -B+kd;
        __VTYPE t2 = -B-kd;
        t->x = __SURE_MIN(t1,t2);
        t->y = __SURE_MAX(t1,t2);
        if(t->x>*id) return false; // отсеиваем случаи когда ближайшая точка сферы дальше чем текущий intersect_dist
        if(t->y<(__VTYPE)SURE_R_DELTA) return false; // отсеиваем случаи когда сфера "сзади"
        *in = t->x<(__VTYPE)SURE_R_DELTA ? true : false; // определяем мы внутри или снаружи
        if(*in&&t->y>*id) return false; // отсеиваем случай когда мы внутри но дальняя стенка дальше id
        *id = *in ? t->y : t->x; //если внутри -- id = дальняя стенка, если снаружи ближняя
        return true;
    }else{
        return false;
    };
}

__VTYPE3 PenVec(__VTYPE3 V1)
{
    __VTYPE3 V2;
    __VTYPE x = fabs(V1.x);
    __VTYPE y = fabs(V1.y);
    __VTYPE z = fabs(V1.z);
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

__VTYPE3 DetermineTraceVector(int x,int y,__SURE_CONSTANT __SURE_STRUCT SureGPUData *GPUData,__SURE_GLOBAL float* Randomf,uint* rr)
{
    __VTYPE3 dZ = __FCONV3(GPUData->cam_vec);
    __VTYPE3 dY = -__FCONV3(GPUData->cam_upvec);
    __VTYPE3 dX = cross(dZ,dY);
    size_t mx = GPUData->m_amx;
    size_t my = GPUData->m_amy;
    uint r = *rr;
    __VTYPE kx;
    __VTYPE ky;
    if(GPUData->subp_rnd){
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rx = (Randomf[r]-0.5);
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE ry = (Randomf[r]-0.5);
        kx = GPUData->xy_h*((__VTYPE)x+rx-(__VTYPE)mx/2.0)/(__VTYPE)mx;
        ky = GPUData->xy_h*((__VTYPE)y+ry-(__VTYPE)my/2.0)/(__VTYPE)mx;
    }else{
        kx = GPUData->xy_h*((__VTYPE)x-(__VTYPE)mx/2.0)/(__VTYPE)mx;
        ky = GPUData->xy_h*((__VTYPE)y-(__VTYPE)my/2.0)/(__VTYPE)mx;
    };
    *rr = r;
    return dZ+kx*dX+ky*dY;
};

__VTYPE3 randomize(__VTYPE3 cn,int col_dt,__VTYPE col_ds,__VTYPE col_dm,uint* rr,__SURE_GLOBAL float* Randomf)
{
    #if SURE_RLEVEL<60
    return cn;
    #else
    __VTYPE3 cnr;
    __VTYPE3 ndy = __NORMALIZE(PenVec(cn)); // ndy -- верктор пенрердикулярный нормали
    __VTYPE3 ndx = cross(ndy,cn); // ndx -- второй пенпердикулярный вектор
    uint r = *rr;
    if(col_dt==SURE_D_EQUAL) // Равномерное распеределение
    {
        __VTYPE dx = 2;
        __VTYPE dy = 2;

        while((dx*dx+dy*dy)>1)
        {
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
            __VTYPE ra = Randomf[r];
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
            __VTYPE rb = Randomf[r];
            dx = ra*2.0-1.0;
            dy = rb*2.0-1.0;
        };
        __VTYPE dz = sqrt(1-dx*dx-dy*dy);
        cnr = cn*dz+ndx*dx+ndy*dy;
        cnr = __NORMALIZE(cnr);
    }else if(col_dt==SURE_D_NORM) // Нормальное распределеие
    {
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE ra = Randomf[r]*2*SURE_PI;
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rb = Randomf[r];
        rb = erfc(rb)*col_ds+col_dm; // функция ошибки
                          //  erfc(rb) это что-то типа такого:
                          //  if(rb>0.5)
                          //      rb = sqrt(-2.0*log(1.0-rb));
                          //  else
                          //      rb = sqrt(-2.0*log(rb));
                          //  rb = rb-(2.30753f+0.27061f*rb)/(1.0f+0.99229f*rb+0.04481f*rb*rb);
        cnr = cn*cos(rb)+ndx*cos(ra)*sin(rb)+ndy*sin(ra)*sin(rb);
        cnr = __NORMALIZE(cnr);
    }else // Без распределения
    {
        cnr = cn;
    }; //распределеие
    *rr = r;
    return cnr;
    #endif // SURE_RLEVEL
};

