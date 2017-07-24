#pragma OPENCL EXTENSION cl_khr_fp64 : enable
 #include <SureDefines.h>

#define __SURE_GLOBAL global
#define __SURE_CONSTANT __constant
#define __SURE_VINT4 int4
#define __SURE_UCHAR3 uchar3
#define __SURE_UINT4 uint4
#define __SURE_GLOBAL_MX get_global_size(0)
#define __SURE_GLOBAL_MY get_global_size(1)
#define __SURE_MIN(A,B) min(A,B)
#define __SURE_MAX(A,B) max(A,B)
#define __SURE_COL_RGB_X col->rgb.x
#define __SURE_COL_RGB_Y col->rgb.y
#define __SURE_COL_RGB_Z col->rgb.z
#define __MAD(A,B,C) mad(A,B,C)

#ifdef SURE_GPU_FLOAT

#define __VTYPE float
#define __VTYPE2 float2
#define __VTYPE3 float3
#define __FCONV3(A) c_d_f(A)
#define __DCONV2(A) A
#define __DFCONV2(A) A
#define __DCONV3(A) A
#define __FCONV(A) (float)A
#define __NORMALIZE(A) fast_normalize(A)
#define __LENGTH(A) fast_length(A)
float3 c_d_f(double3 d)
{
    float3 result;
    result.x = d.x;
    result.y = d.y;
    result.z = d.z;
    return result;
};

#else // !SURE_GPU_FLOAT

#define __VTYPE double
#define __VTYPE2 double2
#define __VTYPE3 double3
#define __FCONV3(A) A
#define __DCONV2(A) c_d_f2(A)
#define __DFCONV2(A) c_f_d2(A)
#define __DCONV3(A) c_f_d(A)
#define __FCONV(A) A
#define __NORMALIZE(A) normalize(A)
#define __LENGTH(A) length(A)

double3 c_f_d(float3 d)
{
    double3 result;
    result.x = d.x;
    result.y = d.y;
    result.z = d.z;
    return result;
};
float2 c_d_f2(double2 d)
{
    float2 result;
    result.x = d.x;
    result.y = d.y;
    return result;
};
double2 c_f_d2(float2 d)
{
    double2 result;
    result.x = d.x;
    result.y = d.y;
    return result;
};

#endif

#define __GET_VERTEX(P,VID) \
        coords.y = VID>>CLSIZE_VERTEX_SHF; \
        coords.x = VID-(coords.y<<CLSIZE_VERTEX_SHF); \
        P.xyz = __DCONV3(read_imagef(VrtxCLImg,smpVertex,coords).xyz);

#define __GET_MESH(P,VID) \
        mcoords.y = VID>>CLSIZE_VERTEX_SHF; \
        mcoords.x = VID-(mcoords.y<<CLSIZE_VERTEX_SHF); \
        P = read_imagei(MeshCLImg,smpVertex,mcoords);

#define __GET_TEXTURE(ix,iy,id) \
        map_uv.x = ix; \
        map_uv.y = iy+id*SURE_R_TEXRES; \
        col_rgba = read_imageui(Textures,smpTex,__DCONV2(map_uv)); \
        col_transp = 1.01 - (col_rgba.w/255.0); \
        col_rgb.x = col_rgba.x; \
        col_rgb.y = col_rgba.y; \
        col_rgb.z = col_rgba.z; \
        if(col_transp>0.5)col_dt=SURE_D_NORM;

#define __GET_TEXTURE_UV(cm,id) \
__VTYPE2 v1,v2,v0; \
int tid = cm; \
coords.y = tid>>CLSIZE_VERTEX_SHF; \
coords.x = (tid - (coords.y<<CLSIZE_VERTEX_SHF))<<2; \
v0.xy = __DFCONV2(read_imagef(UVMap,smpVertex,coords).xy); \
coords.x++; \
v1.xy = __DFCONV2(read_imagef(UVMap,smpVertex,coords).xy); \
coords.x++; \
v2.xy = __DFCONV2(read_imagef(UVMap,smpVertex,coords).xy); \
map_uv = v0+(v1-v0)*u+(v2-v0)*v; \
map_uv.y += id*SURE_R_TEXRES; \
col_rgba = read_imageui(Textures,smpTex,__DCONV2(map_uv)); \
col_transp = 1.01 - (col_rgba.w / 255.0); \
col_rgb.x = col_rgba.x; \
col_rgb.y = col_rgba.y; \
col_rgb.z = col_rgba.z; \
if(col_transp>0.5)col_dt=SURE_D_NORM;

#define GPU
#include <SureGPUData.h>
#include <func_common.c>

__kernel
__attribute__(( vec_type_hint(__VTYPE3)))
void Trace(        __global float* rgbmatrix, // картинка, в которую рисуем
                   __global float* Randomf, // массив случайных чисел
                   constant struct SureGPUData* GPUData, // структура с общими настройками рендера
                   __global struct SureDrawable* Drawables, // набор объектов для отрисовки
                   __read_only image2d_t VrtxCLImg, // Набор vertexов
                   __read_only image2d_t Textures, // Текстуры
                   __read_only image2d_t MeshCLImg, // Mesh'ы
                   __read_only image2d_t UVMap // мэппинг мешей на текстуры
                   )
{
// координаты обрабатываемой точки
int x = get_global_id(0);
int y = get_global_id(1);

// для чтения изображений:
const sampler_t smpVertex = CLK_NORMALIZED_COORDS_FALSE |
                             CLK_ADDRESS_NONE            |
                             CLK_FILTER_NEAREST;
const sampler_t smpTex = CLK_NORMALIZED_COORDS_FALSE |
                             CLK_ADDRESS_NONE     |
                             CLK_FILTER_LINEAR;
int2 coords;
int2 mcoords;
__VTYPE2 map_uv;

// общая для CPU и GPU функция трассировки
 #include <trace_common.c>
}
