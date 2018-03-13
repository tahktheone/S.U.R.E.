#ifndef SUREGPUDATA_H_INCLUDED
#define SUREGPUDATA_H_INCLUDED

struct SureDrawable {

#ifdef GPU
    double3 X; //Координаты центра
    double3 ox; //Локальная ось x
    double3 oy; //Локальная ось y
    double3 oz; //Нормаль (Локальная ось z)
    double lx; // длина
    double ly; // ширина
    double lz; // высота
    int type; // форма
    int radiance; // свечение
    double transp; // прозрачность
    double transp_i; // прозрачность внутренняя
    double refr; // Коэффициент преломления
    int dist_type; // тип рандомизации
    double dist_sigma;
    double dist_m;
    uchar3 rgb;
    bool sided;
    uint mesh_start;
    uint mesh_count;
    bool mesh_changed;
    int map_id;
    int advmap_id;
#else
    cl_double3 X = cl_double3{0,0,0}; //Координаты центра
    cl_double3 ox = cl_double3{1,0,0}; //Локальная ось x
    cl_double3 oy = cl_double3{0,1,0}; //Локальная ось y
    cl_double3 oz = cl_double3{0,0,1}; //Нормаль (Локальная ось z)
    cl_double lx = 1.0; // длина
    cl_double ly = 1.0; // ширина
    cl_double lz = 1.0; // высота
    cl_int type = SURE_DR_SPHERE; // форма
    cl_int radiance = 255; // свечение
    cl_double transp = 0.0; // прозрачность
    cl_double transp_i = 0.0; // прозрачность внутренняя
    cl_double refr = 99.0; // Коэффициент преломления
    cl_int dist_type = SURE_D_EQUAL; // тип рандомизации
    cl_double dist_sigma = 1.0; // sigma рандомизации
    cl_double dist_m = SURE_PI2; // матожидание рандомизации
    cl_uchar3 rgb = cl_uchar3{255,255,255}; // цвет
    cl_bool sided = false; // "Внутренняя" и "Наружная" стороны отличаются
    cl_uint mesh_start;
    cl_uint mesh_count;
    cl_bool mesh_changed = false;
    cl_int map_id = -1;
    cl_int advmap_id = -1;
#endif // CPU

};

struct SureCameraInfo {
#ifdef GPU
    double3 cam_x; // Камера
    double3 cam_vec;
    double3 cam_upvec;
    double xy_h; // Параметры изображения
    uint m_amx;
    uint m_amy;
    bool subp_rnd;
#else
    cl_double3 cam_x = cl_double3{-30,0,5}; // Камера
    cl_double3 cam_vec = cl_double3{1,0,0};
    cl_double3 cam_upvec = cl_double3{0,0,1};
    cl_double xy_h = cl_double{3.0}; // Угол обзора
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

__VTYPE3 PenVec(__VTYPE3 V1);
bool CollideRaySphered(__VTYPE3 tp,__VTYPE3 tv,__VTYPE3 o,__VTYPE r, bool *in,__VTYPE* id);
__VTYPE3 randomize(__VTYPE3 cn,int col_dt,__VTYPE col_ds,__VTYPE col_dm,uint* rr,__SURE_CONSTANT float* Randomf);
__VTYPE3 DetermineTraceVectorSAA(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo,__SURE_CONSTANT float* Randomf,uint* rr);
__VTYPE3 DetermineTraceVector(int x,int y,__SURE_STRUCT SureCameraInfo *CameraInfo);

#define RT_SETCOL \
collision_found = true; \
col_refr = cur.refr / col.refr; \
if(cur.refr > col.refr){  \
    col_rgb = cur.rgb;  \
    col_transp = cur.transp;  \
    col_dt = cur.dist_type;  \
    col_ds = cur.dist_sigma;  \
    col_dm = cur.dist_m;  \
}else{  \
    col_rgb = col.rgb;  \
    col_transp = col.transp;  \
    col_dt = col.dist_type;   \
    col_ds = col.dist_sigma;  \
    col_dm = col.dist_m;   \
}

#endif // SUREGPUDATA_H_INCLUDED
