#ifndef SUREGPUDATA_H_INCLUDED
#define SUREGPUDATA_H_INCLUDED

struct SureDrawable {

#ifdef GPU
    float3 X; //Координаты центра
    float3 ox; //Локальная ось x
    float3 oy; //Локальная ось y
    float3 oz; //Нормаль (Локальная ось z)
    float lx; // длина
    float ly; // ширина
    float lz; // высота
    int type; // форма
    int radiance; // свечение
    float transp; // прозрачность
    float transp_i; // прозрачность внутренняя
    float refr; // Коэффициент преломления
    int dist_type; // тип рандомизации
    float dist_sigma;
    float dist_m;
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
    cl_float lx = 1.0; // длина
    cl_float ly = 1.0; // ширина
    cl_float lz = 1.0; // высота
    cl_int type = SURE_DR_SPHERE; // форма
    cl_int radiance = 255; // свечение
    cl_float transp = 0.0; // прозрачность
    cl_float transp_i = 0.0; // прозрачность внутренняя
    cl_float refr = 99.0; // Коэффициент преломления
    cl_int dist_type = SURE_D_EQUAL; // тип рандомизации
    cl_float dist_sigma = 1.0; // sigma рандомизации
    cl_float dist_m = SURE_PI2; // матожидание рандомизации
    cl_uchar3 rgb = cl_uchar3{255,255,255}; // цвет
    cl_bool sided = false; // "Внутренняя" и "Наружная" стороны отличаются
    cl_uint mesh_start = 0;
    cl_uint mesh_count = 0;
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
    int SAA;
    struct SureDrawable* Drawables; // для GPU -- не имеет смысла, присутствует тут для целостности структуры
#else
    cl_uchar reset = true; // сброс кадра -- фаза 2
    SureCameraInfo CameraInfo;
    cl_int m_drawables = 0; //Количество объектов на сцене
    cl_uchar toreset = true; // сброс кадра -- фаза 1
    cl_uchar r_maxiters = 20; // глубина анализа рендера
    cl_uchar r_rechecks = 20; // качество рендера
    cl_float r_backlight = 0.5;
    cl_int SAA = 8; // Количество субпикселей на пиксель
    SureDrawable* Drawables; // Для CPU -- указатель на массив с объектами сцены
#endif // GPU
};

bool RayAndSphereCollided(__VTYPE3 tp,__VTYPE3 tv,__VTYPE3 o,__VTYPE r, bool *in,__VTYPE* id);
__VTYPE3 DetermineTraceVectorSAA(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo,__SURE_DECLARE_RANDOM float* Randomf,uint* rr);
__VTYPE3 DetermineTraceVector(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo);
uint InitRandom(int *x,int *y);

#define SET_COLLISION \
    collision_found = true; \
    DrawableCollided.refr = __DIVIDE(cur->refr,col->refr); \
    DrawableCollided.transp = __SURE_MIN(cur->transp,col->transp); \
    if(cur->refr > col->refr){ \
        DrawableCollided.rgb = cur->rgb; \
        DrawableCollided.dist_type = cur->dist_type; \
        DrawableCollided.dist_sigma = cur->dist_sigma; \
        DrawableCollided.dist_m = cur->dist_m; \
    }else{ \
        DrawableCollided.rgb = col->rgb; \
        DrawableCollided.dist_type = col->dist_type; \
        DrawableCollided.dist_sigma = col->dist_sigma; \
        DrawableCollided.dist_m = col->dist_m; \
    };

#define SET_COLLISION_IO(I_IN,I_OUT) \
    collision_found = true; \
    DrawableCollided.refr = __DIVIDE(I_IN->refr,I_OUT->refr); \
    DrawableCollided.transp = __SURE_MIN(I_IN->transp,I_OUT->transp); \
    DrawableCollided.radiance = I_OUT->radiance; \
    if(I_IN->refr > I_OUT->refr){ \
        DrawableCollided.rgb = I_IN->rgb; \
        DrawableCollided.dist_type = I_IN->dist_type; \
        DrawableCollided.dist_sigma = I_IN->dist_sigma; \
        DrawableCollided.dist_m = I_IN->dist_m; \
    }else{ \
        DrawableCollided.rgb = I_OUT->rgb; \
        DrawableCollided.dist_type = I_OUT->dist_type; \
        DrawableCollided.dist_sigma = I_OUT->dist_sigma; \
        DrawableCollided.dist_m = I_OUT->dist_m; \
    };

#define SET_COLLISION_INSIDE SET_COLLISION_IO(DrawableIter,DrawableNext);

#define SET_COLLISION_OUTSIDE SET_COLLISION_IO(DrawableCurrent,DrawableIter);

#define SET_COLLISION_MESH \
intersect_dist = TraceDistance; \
if(DrawableIter->map_id>=0){ \
    __GET_TEXTURE_UV(CurrentMesh,DrawableIter->map_id); \
}; \
if(DrawableIter->advmap_id>=0){ \
    __GET_ADVMAP_UV(CurrentMesh,DrawableIter->advmap_id); \
};

#define GET_SPHERICAL_UV_COORDINATES(COORDS) \
    __VTYPE3 VectorToCollision = collision_point - DrawableIter->X; \
    __VTYPE3 LocalCoordinates; \
    LocalCoordinates.x = dot(DrawableIter->ox,VectorToCollision); \
    LocalCoordinates.y = dot(DrawableIter->oy,VectorToCollision); \
    LocalCoordinates.z = dot(DrawableIter->oz,VectorToCollision); \
    LocalCoordinates = __NORMALIZE(LocalCoordinates); \
    COORDS.y = __MAD(0.5f,LocalCoordinates.z,0.5f); \
    if(LocalCoordinates.x>0.0f){ \
        COORDS.x = __MAD(atan2(LocalCoordinates.y,LocalCoordinates.x),M_1_PI_F,0.5f); \
    }else{ \
        LocalCoordinates = -LocalCoordinates; \
        COORDS.x = __MAD(atan2(LocalCoordinates.y,LocalCoordinates.x),M_1_PI_F,1.5f); \
    }; \
    COORDS.x*=0.5f;

#define GET_TEXTURE_SQUARE \
if((DrawableIter->map_id>=0)||(DrawableIter->advmap_id>=0)){ \
    __VTYPE2 TextureCoordinates = {__DIVIDE((__VTYPE)SURE_R_TEXRES*0.5f*(LocalCoordinates.x+DrawableIter->lx),DrawableIter->lx), \
                                   __DIVIDE((__VTYPE)SURE_R_TEXRES*0.5f*(LocalCoordinates.y+DrawableIter->ly),DrawableIter->ly)}; \
    if(DrawableIter->map_id>=0) \
    { \
        __GET_TEXTURE(TextureCoordinates.x, \
                      TextureCoordinates.y, \
                      DrawableIter->map_id); \
    }; \
    if(DrawableIter->advmap_id>=0) \
    { \
        __GET_ADVMAP(TextureCoordinates.x, \
                     TextureCoordinates.y, \
                     DrawableIter->advmap_id); \
    }; \
};

#define AABB_CHECK_AXIS(__A,__LA) \
DistanceToSide = __DIVIDE((DrawableIter->__LA-LocalTracePoint.__A),LocalTraceVector.__A); \
if((LocalTraceVector.__A)>0.0f){ \
    if(DistanceToSide<AABB_NearestOut) AABB_NearestOut = DistanceToSide; \
}else{ \
    if(DistanceToSide>AABB_FarestIn) AABB_FarestIn = DistanceToSide; \
}; \
DistanceToSide = __DIVIDE((-DrawableIter->__LA-LocalTracePoint.__A),LocalTraceVector.__A); \
if(LocalTraceVector.__A<0.0f){ \
    if(DistanceToSide<AABB_NearestOut) AABB_NearestOut = DistanceToSide; \
}else{ \
    if(DistanceToSide>AABB_FarestIn) AABB_FarestIn = DistanceToSide; \
};


#define SURE_MOLLER_TRUMBOR \
pvec = cross(LocalTraceVector,LocalVertex3-LocalVertex1); \
det = dot(LocalVertex2-LocalVertex1,pvec); \
if(fabs(det)<0.0f) continue; \
inv_det = __INV(det); \
tvec = LocalTracePoint - LocalVertex1; \
u = dot(tvec, pvec) * inv_det; \
if (u < 0.0f || u > 1.0f ) continue; \
qvec = cross(tvec, LocalVertex2-LocalVertex1); \
v = dot(LocalTraceVector, qvec) * inv_det; \
if (v < 0.0f || (u + v) > 1.0f ) continue; \
TraceDistance = dot(LocalVertex3-LocalVertex1, qvec) * inv_det * ResizingKoeff;

#define SET_CURRENT \
if(DrawableIter->refr>DrawableCurrent->refr){ \
    if(DrawableCurrent->refr>DrawableNext->refr){ \
        DrawableNext = DrawableCurrent; \
    }; \
    DrawableCurrent = DrawableIter; \
}else if(DrawableIter->refr>DrawableNext->refr){ \
    DrawableNext = DrawableIter; \
};

#endif // SUREGPUDATA_H_INCLUDED
