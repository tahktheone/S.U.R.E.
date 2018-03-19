#ifndef SUREGPUDATA_H_INCLUDED
#define SUREGPUDATA_H_INCLUDED

struct SureDrawable {

#ifdef GPU
    float3 X; //Координаты центра
    float3 ox; //Локальная ось x
    float3 oy; //Локальная ось y
    float3 oz; //Нормаль (Локальная ось z)
    double lx; // длина
    double ly; // ширина
    double lz; // высота
    int type; // форма
    int radiance; // свечение
    double transp; // прозрачность
    double transp_i; // прозрачность внутренняя
    float refr; // Коэффициент преломления
    int dist_type; // тип рандомизации
    double dist_sigma;
    double dist_m;
    uchar3 rgb;
    bool sided;
    uint mesh_start;
    uint mesh_count;
    int map_id;
    int advmap_id;
#else
    cl_float3 X = cl_float3{0,0,0}; //Координаты центра
    cl_float3 ox = cl_float3{1,0,0}; //Локальная ось x
    cl_float3 oy = cl_float3{0,1,0}; //Локальная ось y
    cl_float3 oz = cl_float3{0,0,1}; //Нормаль (Локальная ось z)
    cl_double lx = 1.0; // длина
    cl_double ly = 1.0; // ширина
    cl_double lz = 1.0; // высота
    cl_int type = SURE_DR_SPHERE; // форма
    cl_int radiance = 255; // свечение
    cl_double transp = 0.0; // прозрачность
    cl_double transp_i = 0.0; // прозрачность внутренняя
    cl_float refr = 99.0; // Коэффициент преломления
    cl_int dist_type = SURE_D_EQUAL; // тип рандомизации
    cl_double dist_sigma = 1.0; // sigma рандомизации
    cl_double dist_m = SURE_PI2; // матожидание рандомизации
    cl_uchar3 rgb = cl_uchar3{255,255,255}; // цвет
    cl_bool sided = false; // "Внутренняя" и "Наружная" стороны отличаются
    cl_uint mesh_start;
    cl_uint mesh_count;
    cl_int map_id = -1;
    cl_int advmap_id = -1;
#endif // CPU

};

struct SureCameraInfo {
#ifdef GPU
    float3 cam_x; // Камера
    float3 cam_vec;
    float3 cam_upvec;
    float xy_h; // Параметры изображения
    uint m_amx;
    uint m_amy;
    bool subp_rnd;
#else
    cl_float3 cam_x = cl_float3{-30,0,5}; // Камера
    cl_float3 cam_vec = cl_float3{1,0,0};
    cl_float3 cam_upvec = cl_float3{0,0,1};
    cl_float xy_h = cl_float{3.0}; // Угол обзора
    cl_uint m_amx = 1920;
    cl_uint m_amy = 1080;
    cl_bool subp_rnd = false;
#endif // GPU
};

struct SureGPUData {
#ifdef GPU
    uchar reset; // сброс кадра
    struct SureCameraInfo CameraInfo;
    int m_drawables; //Количество объектов на сцене
    uchar toreset; // сброс кадра
    uchar r_maxiters;
    uchar r_rechecks;
    float r_backlight;
    struct SureDrawable* Drawables; // для GPU -- не имеет смысла, присутствует тут для целостности структуры
#else
    cl_uchar reset = true; // сброс кадра -- фаза 2
    SureCameraInfo CameraInfo;
    cl_int m_drawables = 0; //Количество объектов на сцене
    cl_uchar toreset = true; // сброс кадра -- фаза 1
    cl_uchar r_maxiters = 20; // глубина анализа рендера
    cl_uchar r_rechecks = 20; // качество рендера
    cl_float r_backlight = 0.5;
    SureDrawable* Drawables; // Для CPU -- указатель на массив с объектами сцены
#endif // GPU
};

__VTYPE3 PenVec(__VTYPE3 qqq);
bool RayAndSphereCollided(__VTYPE3 tp,__VTYPE3 tv,__VTYPE3 o,__VTYPE r, bool *in,__VTYPE* id);
__VTYPE3 randomize(__VTYPE3 cn,int col_dt,__VTYPE col_ds,__VTYPE col_dm,uint* rr,__SURE_DECLARE_RANDOM float* Randomf);
__VTYPE3 DetermineTraceVectorSAA(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo,__SURE_DECLARE_RANDOM float* Randomf,uint* rr);
__VTYPE3 DetermineTraceVector(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo);
uint InitRandom(int *x,int *y);
__VTYPE3 VectorInBasis(__VTYPE3 i_vector,__VTYPE3 ox,__VTYPE3 oy,__VTYPE3 oz);
__VTYPE3 VectorInBasisNormalized(__VTYPE3 i_vector,__VTYPE3 ox,__VTYPE3 oy,__VTYPE3 oz);
__VTYPE3 VectorInDrawableBasis(const __VTYPE3 i_vector,const __SURE_GLOBAL __SURE_STRUCT SureDrawable *i_dr);
__VTYPE3 VectorInDrawableBasisNormalized(const __VTYPE3 i_vector,const __SURE_GLOBAL __SURE_STRUCT SureDrawable *i_dr);
__VTYPE3 GetUVCoordinatesByLocalCoordinates(__VTYPE3 LocalCoordinates);

#define SET_COLLISION \
    collision_found = true; \
    DrawableCollided.refr = __DIVIDE(cur->refr,col->refr); \
    if(cur->refr > col->refr){ \
        DrawableCollided.rgb = cur->rgb; \
        DrawableCollided.transp = cur->transp; \
        DrawableCollided.dist_type = cur->dist_type; \
        DrawableCollided.dist_sigma = cur->dist_sigma; \
        DrawableCollided.dist_m = cur->dist_m; \
    }else{ \
        DrawableCollided.rgb = col->rgb; \
        DrawableCollided.transp = col->transp; \
        DrawableCollided.dist_type = col->dist_type; \
        DrawableCollided.dist_sigma = col->dist_sigma; \
        DrawableCollided.dist_m = col->dist_m; \
    };

#define SET_COLLISION_MESH \
SET_COLLISION; \
intersect_dist = TraceDistance; \
if(lv_dr->map_id>=0){ \
    __GET_TEXTURE_UV(CurrentMesh,lv_dr->map_id); \
}; \
if(lv_dr->advmap_id>=0){ \
    __GET_ADVMAP_UV(CurrentMesh,lv_dr->advmap_id); \
};

#define GET_SPHERICAL_UV_COORDINATES(COORDS) \
    __VTYPE3 VectorToCollision = collision_point - lv_dr->X; \
    __VTYPE3 LocalCoordinates; \
    LocalCoordinates.x = dot(lv_dr->ox,VectorToCollision); \
    LocalCoordinates.y = dot(lv_dr->oy,VectorToCollision); \
    LocalCoordinates.z = dot(lv_dr->oz,VectorToCollision); \
    LocalCoordinates = __NORMALIZE(LocalCoordinates); \
    COORDS.y = 0.5*(LocalCoordinates.z+1.0); \
    if(LocalCoordinates.x>0){ \
        COORDS.x = atan(LocalCoordinates.y/LocalCoordinates.x)/M_PI+0.5; \
    }else{ \
        COORDS.x = atan(LocalCoordinates.y/LocalCoordinates.x)/M_PI+1.5; \
    }; \
    COORDS.x*=0.5;

#define GET_TEXTURE_SQUARE \
if((lv_dr->map_id>=0)||(lv_dr->advmap_id>=0)){ \
    __VTYPE2 TextureCoordinates = {(__VTYPE)SURE_R_TEXRES*0.5f*(LocalCoordinates.x+lv_dr->lx)/lv_dr->lx, \
                                   (__VTYPE)SURE_R_TEXRES*0.5f*(LocalCoordinates.y+lv_dr->ly)/lv_dr->ly}; \
    if(lv_dr->map_id>=0) \
    { \
        __GET_TEXTURE(TextureCoordinates.x, \
                      TextureCoordinates.y, \
                      lv_dr->map_id); \
    }; \
    if(lv_dr->advmap_id>=0) \
    { \
        __GET_ADVMAP(TextureCoordinates.x, \
                     TextureCoordinates.y, \
                     lv_dr->advmap_id); \
    }; \
};

#define AABB_CHECK_AXIS(__A,__LA) \
DistanceToSide = (lv_dr->__LA-LocalTracePoint.__A)/LocalTraceVector.__A; \
if((LocalTraceVector.__A)>0.0f){ \
    if(DistanceToSide<AABB_NearestOut) AABB_NearestOut = DistanceToSide; \
}else{ \
    if(DistanceToSide>AABB_FarestIn) AABB_FarestIn = DistanceToSide; \
}; \
DistanceToSide = -(LocalTracePoint.__A+lv_dr->__LA)/LocalTraceVector.__A; \
if(LocalTraceVector.__A<0.0f){ \
    if(DistanceToSide<AABB_NearestOut) AABB_NearestOut = DistanceToSide; \
}else{ \
    if(DistanceToSide>AABB_FarestIn) AABB_FarestIn = DistanceToSide; \
};

// __VTYPE3 CollisionNormalOy = __NORMALIZE(PenVec(collision_normal));
// __VTYPE3 CollisionNormalOy = cross(collision_normal,TraceVector);

#if SURE_RLEVEL<60
#define SURE_RANDOMIZE(CNR) CNR = collision_normal;
#else
#define SURE_RANDOMIZE(CNR) \
    if(DrawableCollided.dist_type==SURE_D_EQUAL){ \
        __VTYPE3 CollisionNormalOy = __NORMALIZE(PenVec(collision_normal)); \
        __VTYPE3 CollisionNormalOx = cross(CollisionNormalOy,collision_normal); \
        __VTYPE3 RandomNormal; \
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
            RandomNormal.x = Randomf[r]*2.0f-1.0f; \
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
            RandomNormal.y = Randomf[r]*2.0f-1.0f; \
        while((RandomNormal.x*RandomNormal.x+RandomNormal.y*RandomNormal.y)>1.0f){ \
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
                RandomNormal.x = Randomf[r]*2.0f-1.0f; \
            if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
                RandomNormal.y = Randomf[r]*2.0f-1.0f; \
        }; \
        RandomNormal.z = sqrt(1-RandomNormal.x*RandomNormal.x-RandomNormal.y*RandomNormal.y); \
        CNR = collision_normal*RandomNormal.z+CollisionNormalOx*RandomNormal.x+CollisionNormalOy*RandomNormal.y; \
        CNR = __NORMALIZE(CNR); \
    }else if(DrawableCollided.dist_type==SURE_D_NORM){ \
        __VTYPE3 CollisionNormalOy = __NORMALIZE(PenVec(collision_normal)); \
        __VTYPE3 CollisionNormalOx = cross(CollisionNormalOy,collision_normal); \
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
        __VTYPE RotationAngle = Randomf[r]*2.0*SURE_PI; \
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE; \
        __VTYPE RandomEqualX = Randomf[r]; \
        __VTYPE LeanAngle = erfc(RandomEqualX)*DrawableCollided.dist_sigma+DrawableCollided.dist_m; \
        CNR = collision_normal*cos(LeanAngle)+ \
              CollisionNormalOx*cos(RotationAngle)*sin(LeanAngle)+ \
              CollisionNormalOy*sin(RotationAngle)*sin(LeanAngle); \
        CNR = __NORMALIZE(CNR); \
    }else{ \
        CNR = collision_normal; \
    };
#endif

#define SURE_MOLLER_TRUMBOR \
pvec = cross(LocalTraceVector,LocalVertex3-LocalVertex1); \
det = dot(LocalVertex2-LocalVertex1,pvec); \
if(fabs(det)<0.0f) continue; \
inv_det = 1.0f / det; \
tvec = LocalTracePoint - LocalVertex1; \
u = dot(tvec, pvec) * inv_det; \
if (u < 0.0f || u > 1.0f ) continue; \
qvec = cross(tvec, LocalVertex2-LocalVertex1); \
v = dot(LocalTraceVector, qvec) * inv_det; \
if (v < 0.0f || (u + v) > 1.0f ) continue; \
TraceDistance = dot(LocalVertex3-LocalVertex1, qvec) * inv_det * ResizingKoeff;

#endif // SUREGPUDATA_H_INCLUDED
