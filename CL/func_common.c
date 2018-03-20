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

__VTYPE3 DetermineTraceVectorSAA(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo,__SURE_DECLARE_RANDOM float* Randomf,uint* rr)
{
    __VTYPE3 dZ = CameraInfo->cam_vec;
    __VTYPE3 dY = -CameraInfo->cam_upvec;
    __VTYPE3 dX = cross(dZ,dY);
    float mx = CameraInfo->m_amx;
    float my = CameraInfo->m_amy;
    uint r = *rr;
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rx = (Randomf[r]-0.5f);
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE ry = (Randomf[r]-0.5f);
    __VTYPE kx = CameraInfo->xy_h*__DIVIDE(((__VTYPE)x+rx-__DIVIDE(mx,2.0f)),mx);
    __VTYPE ky = CameraInfo->xy_h*__DIVIDE(((__VTYPE)y+ry-__DIVIDE(my,2.0f)),mx);
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
    __VTYPE kx = __DIVIDE(CameraInfo->xy_h*((__VTYPE)x-__DIVIDE(mx,2.0f)),mx);
    __VTYPE ky = __DIVIDE(CameraInfo->xy_h*((__VTYPE)y-__DIVIDE(my,2.0f)),mx);
    return __NORMALIZE(dZ+kx*dX+ky*dY);
};

uint InitRandom(int *x,int *y){
    uint xx = *x+*y;
    uint yy = *y;
    uint r = mad24(xx,xx+yy,xx+yy);
    while(r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
    return r;
};
