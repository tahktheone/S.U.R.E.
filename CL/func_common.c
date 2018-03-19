bool RayAndSphereCollided(__VTYPE3 tp,__VTYPE3 tv,__VTYPE3 o,__VTYPE r, bool *in,__VTYPE* id)
{
    __VTYPE3 vtc = tp-o; //VecToCenter
    __VTYPE B = dot(vtc,tv);
    __VTYPE C = dot(vtc,vtc)-r*r;
    __VTYPE D = B*B-C;
    __VTYPE2 t;

    if(D>=0){
        __VTYPE kd = sqrt(D);
        __VTYPE t1 = -B+kd;
        __VTYPE t2 = -B-kd;
        t.x = __SURE_MIN(t1,t2);
        t.y = __SURE_MAX(t1,t2);
        if(t.x>*id) return false; // отсеиваем случаи когда ближайшая точка сферы дальше чем текущий intersect_dist
        if(t.y<(__VTYPE)SURE_R_DELTA) return false; // отсеиваем случаи когда сфера "сзади"
        *in = t.x<(__VTYPE)SURE_R_DELTA ? true : false; // определяем мы внутри или снаружи
        if(*in&&t.y>*id) return false; // отсеиваем случай когда мы внутри но дальняя стенка дальше id
        *id = *in ? t.y : t.x; //если внутри -- id = дальняя стенка, если снаружи ближняя
        return true;
    }else{
        return false;
    };
}

__VTYPE3 PenVec(__VTYPE3 qqq)
{
    __VTYPE3 V2;
    __VTYPE x = fabs(qqq.x);
    __VTYPE y = fabs(qqq.y);
    __VTYPE z = fabs(qqq.z);
    int typ = x < y ? x < z ?   0   :   2   : y < z ? 1 : 2 ;
//                          * -x<z- * -x>z- *
//                  * ----------x<y-------- * -- x>y -------*
    if(typ==0)
    {
        V2.x = 0;
        V2.y = qqq.z;
        V2.z = -qqq.y;
    };
    if(typ==1)
    {
        V2.x = qqq.z;
        V2.y = 0;
        V2.z = -qqq.x;
    };
    if(typ==2)
    {
        V2.x = qqq.y;
        V2.y = -qqq.x;
        V2.z = 0;
    };
    return V2;
};

__VTYPE3 DetermineTraceVectorSAA(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo,__SURE_DECLARE_RANDOM float* Randomf,uint* rr)
{
    __VTYPE3 dZ = CameraInfo->cam_vec;
    __VTYPE3 dY = -CameraInfo->cam_upvec;
    __VTYPE3 dX = cross(dZ,dY);
    size_t mx = CameraInfo->m_amx;
    size_t my = CameraInfo->m_amy;
    uint r = *rr;
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rx = (Randomf[r]-0.5);
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE ry = (Randomf[r]-0.5);
    __VTYPE kx = CameraInfo->xy_h*((__VTYPE)x+rx-(__VTYPE)mx/2.0)/(__VTYPE)mx;
    __VTYPE ky = CameraInfo->xy_h*((__VTYPE)y+ry-(__VTYPE)my/2.0)/(__VTYPE)mx;
    *rr = r;
    return __NORMALIZE(dZ+kx*dX+ky*dY);
};

__VTYPE3 DetermineTraceVector(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo)
{
    __VTYPE3 dZ = CameraInfo->cam_vec;
    __VTYPE3 dY = -CameraInfo->cam_upvec;
    __VTYPE3 dX = cross(dZ,dY);
    float mx = (float)CameraInfo->m_amx;
    float my = (float)CameraInfo->m_amy;
    __VTYPE kx = CameraInfo->xy_h*((__VTYPE)x-mx/2.0f)/mx;
    __VTYPE ky = CameraInfo->xy_h*((__VTYPE)y-my/2.0f)/mx;
    return __NORMALIZE(dZ+kx*dX+ky*dY);
};

__VTYPE3 randomize(__VTYPE3 cn,int col_dt,__VTYPE col_ds,__VTYPE col_dm,uint* rr,__SURE_DECLARE_RANDOM float* Randomf)
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

uint InitRandom(int *x,int *y){
    uint xx = *x+*y;
    uint yy = *y;
    uint r = mad24(xx,xx+yy,xx+yy);
    while(r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
    return r;
};

__VTYPE3 VectorInBasis(__VTYPE3 i_vector,__VTYPE3 ox,__VTYPE3 oy,__VTYPE3 oz){
    __VTYPE3 result;
    result.x = dot(ox,i_vector);               // Локальные координаты
    result.y = dot(oy,i_vector);               // Локальные координаты
    result.z = dot(oz,i_vector);               // Локальные координаты
    return result;
};

__VTYPE3 VectorInBasisNormalized(__VTYPE3 i_vector,__VTYPE3 ox,__VTYPE3 oy,__VTYPE3 oz){
    __VTYPE3 result;
    result.x = dot(ox,i_vector);               // Локальные координаты
    result.y = dot(oy,i_vector);               // Локальные координаты
    result.z = dot(oz,i_vector);               // Локальные координаты
    result = __NORMALIZE(result);
    return result;
};

__VTYPE3 VectorInDrawableBasis(const __VTYPE3 i_vector,const __SURE_GLOBAL __SURE_STRUCT SureDrawable *i_dr){
    __VTYPE3 result;
    result.x = dot(i_dr->ox,i_vector);               // Локальные координаты
    result.y = dot(i_dr->oy,i_vector);               // Локальные координаты
    result.z = dot(i_dr->oz,i_vector);               // Локальные координаты
    return result;
};

__VTYPE3 VectorInDrawableBasisNormalized(const __VTYPE3 i_vector,const __SURE_GLOBAL __SURE_STRUCT SureDrawable *i_dr){
    __VTYPE3 result;
    result.x = dot(i_dr->ox,i_vector);               // Локальные координаты
    result.y = dot(i_dr->oy,i_vector);               // Локальные координаты
    result.z = dot(i_dr->oz,i_vector);               // Локальные координаты
    result = __NORMALIZE(result);
    return result;
};

__VTYPE3 GetUVCoordinatesByLocalCoordinates(__VTYPE3 i_LocalCoords){
    __VTYPE3 result;
    result.y = 0.5*(i_LocalCoords.z+1.0); // от 0 (низ) до 1 (верх) (предполагается что локальные координаты от -1 до 1)
    if(i_LocalCoords.x>0){
        result.x = atan(i_LocalCoords.y/i_LocalCoords.x)/M_PI+0.5;
    }else{
        result.x = atan(i_LocalCoords.y/i_LocalCoords.x)/M_PI+1.5;
    };
    result.x*=0.5;
    return result;
};

// Логика макроса randomize с комментариями:
/*
    if(DrawableCollided.dist_type==SURE_D_EQUAL) // Равномерное распеределение
    {
        // Строим систему координат, осью Z для которой является collision_normal
        __VTYPE3 CollisionNormalOy = __NORMALIZE(PenVec(collision_normal));
        __VTYPE3 CollisionNormalOx = cross(CollisionNormalOy,collision_normal);
        __VTYPE3 RandomNormal;

        // Выбираем случайные 2 числа от -1 до 1
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
            RandomNormal.x = Randomf[r]*2.0-1.0;
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
            RandomNormal.y = Randomf[r]*2.0-1.0;

         // повторяем, пока не попадем в круг с радиусом 1
        while((RandomNormal.x*RandomNormal.x+RandomNormal.y*RandomNormal.y)>1.0f)
        {
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                RandomNormal.x = Randomf[r]*2.0-1.0;
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                RandomNormal.y = Randomf[r]*2.0-1.0;
        };
        // строим а этих двух координатах вектор с длиной 1
        RandomNormal.z = sqrt(1-RandomNormal.x*RandomNormal.x-RandomNormal.y*RandomNormal.y);
        // Результат, полученный для локальных координат нормали,
        // переводим в глобальные координаты
        CNR = collision_normal*RandomNormal.z+CollisionNormalOx*RandomNormal.x+CollisionNormalOy*RandomNormal.y;
        CNR = __NORMALIZE(CNR);
    }else if(DrawableCollided.dist_type==SURE_D_NORM) // Нормальное распределеие
    {
        // Строим систему координат, осью Z для которой является collision_normal
        __VTYPE3 CollisionNormalOy = __NORMALIZE(PenVec(collision_normal));
        __VTYPE3 CollisionNormalOx = cross(CollisionNormalOy,collision_normal);

        // RotationAngle -- случайное число от 0 до 2*PI -- угол поворота
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE RotationAngle = Randomf[r]*2*SURE_PI;
        // RandomEqualX -- случайное число от 0 до 1 -- случайная велечина с равномерным распределением
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE RandomEqualX = Randomf[r];

        // erfc -- функция ошибки превращающая равномерное распределение в нормальное
        __VTYPE LeanAngle = erfc(RandomEqualX)*DrawableCollided.dist_sigma+DrawableCollided.dist_m;
        CNR = collision_normal*cos(LeanAngle)+
              CollisionNormalOx*cos(RotationAngle)*sin(LeanAngle)+
              CollisionNormalOy*sin(RotationAngle)*sin(LeanAngle);
        CNR = __NORMALIZE(CNR);
    }else // Без распределения
    {
        CNR = collision_normal;
    }; //распределеие
*/
