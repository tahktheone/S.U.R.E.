#include <SureDefines.h>

#define __SURE_BARRIER barrier(CLK_LOCAL_MEM_FENCE)
#define __SURE_GLOBAL __global
#define __SURE_STRUCT struct
#define __SURE_LOCAL __local
#define __SURE_PRIVATE __private
#define __SURE_CONSTANT __constant
#define __SURE_VINT4 int4
#define __SURE_UCHAR3 uchar3
#define __SURE_UINT4 uint4
#define __SURE_MIN(A,B) min(A,B)
#define __SURE_MAX(A,B) max(A,B)
#define __DIVIDE(A,B) native_divide((A),(B))
#define __MAD(A,B,C) fma(A,B,C)
//#define __MAD(A,B,C) (A*B+C)
#define __SURE_DECLARE_RANDOM __global
#define __XX x
#define __YY y
#define __ZZ z
#define __VTYPE float
#define __VTYPE2 float2
#define __VTYPE3 float3
//#define __NORMALIZE(A) normalize(A)
//#define __LENGTH(A) length(A)
#define __NORMALIZE(A) fast_normalize(A)
#define __LENGTH(A) fast_length(A)

#define __GET_NORMALS_INDEX(MESH_ID) \
coords.y = MESH_ID>>CLSIZE_VERTEX_SHF; \
coords.x = (MESH_ID - (coords.y<<CLSIZE_VERTEX_SHF))<<2;

#define __GET_NORMALS_BYINDEX(P1,P2,P3) \
P1.xyz = read_imagef(Normals,smpVertex,coords).xyz; \
coords.x += 1; \
P2.xyz = read_imagef(Normals,smpVertex,coords).xyz; \
coords.x += 1; \
P3.xyz = read_imagef(Normals,smpVertex,coords).xyz;

#define __GET_VERTEX(P,VID) \
        coords.y = VID>>CLSIZE_VERTEX_SHF; \
        coords.x = VID-(coords.y<<CLSIZE_VERTEX_SHF); \
        P.xyz = read_imagef(VrtxCLImg,smpVertex,coords).xyz;

#define __GET_MESH(P,VID) \
        coords.y = VID>>CLSIZE_VERTEX_SHF; \
        coords.x = VID-(coords.y<<CLSIZE_VERTEX_SHF); \
        P = read_imagei(MeshCLImg,smpVertex,coords);

#define __GET_TEXTURE(ix,iy,id) \
        map_uv.x = ix; \
        map_uv.y = iy+id*SURE_R_TEXRES; \
        if(map_uv.x>SURE_R_TEXRES)map_uv.x-=SURE_R_TEXRES; \
        col_rgba = read_imageui(Textures,smpTex,map_uv); \
        DrawableCollided.transp = 1.01 - native_divide(col_rgba.w,255.0f); \
        DrawableCollided.rgb.x = col_rgba.x; \
        DrawableCollided.rgb.y = col_rgba.y; \
        DrawableCollided.rgb.z = col_rgba.z; \
        if(DrawableCollided.transp>0.5)DrawableCollided.dist_type=SURE_D_NORM;

#define __GET_ADVMAP(ix,iy,id) \
        map_uv.x = ix; \
        map_uv.y = iy+id*SURE_R_TEXRES; \
        if(map_uv.x>SURE_R_TEXRES)map_uv.x-=SURE_R_TEXRES; \
        advmap = read_imageui(Textures,smpTex,map_uv); \
        DrawableCollided.radiance = advmap.x; \
        DrawableCollided.dist_sigma = native_divide(advmap.y,20.0f);

#define __GET_TEXTURE_UV(cm,id) \
__VTYPE2 v1,v2,v0; \
int tid = cm; \
coords.y = tid>>CLSIZE_VERTEX_SHF; \
coords.x = (tid - (coords.y<<CLSIZE_VERTEX_SHF))<<2; \
v0.xy = read_imagef(UVMap,smpVertex,coords).xy; \
coords.x++; \
v1.xy = read_imagef(UVMap,smpVertex,coords).xy; \
coords.x++; \
v2.xy = read_imagef(UVMap,smpVertex,coords).xy; \
map_uv = v0+(v1-v0)*u+(v2-v0)*v; \
map_uv.y += id*SURE_R_TEXRES; \
if(map_uv.x>SURE_R_TEXRES)map_uv.x-=SURE_R_TEXRES; \
col_rgba = read_imageui(Textures,smpTex,map_uv); \
DrawableCollided.transp = 1.01 - native_divide(col_rgba.w,255.0f); \
DrawableCollided.rgb.x = col_rgba.x; \
DrawableCollided.rgb.y = col_rgba.y; \
DrawableCollided.rgb.z = col_rgba.z; \
if(DrawableCollided.transp>0.5)DrawableCollided.dist_type=SURE_D_NORM;

#define __GET_ADVMAP_UV(cm,id) \
__VTYPE2 v1,v2,v0; \
int tid = cm; \
coords.y = tid>>CLSIZE_VERTEX_SHF; \
coords.x = (tid - (coords.y<<CLSIZE_VERTEX_SHF))<<2; \
v0.xy = read_imagef(UVMap,smpVertex,coords).xy; \
coords.x++; \
v1.xy = read_imagef(UVMap,smpVertex,coords).xy; \
coords.x++; \
v2.xy = read_imagef(UVMap,smpVertex,coords).xy; \
map_uv = v0+(v1-v0)*u+(v2-v0)*v; \
map_uv.y += id*SURE_R_TEXRES; \
if(map_uv.x>SURE_R_TEXRES)map_uv.x-=SURE_R_TEXRES; \
advmap = read_imageui(Textures,smpTex,map_uv); \
DrawableCollided.radiance = advmap.x; \
DrawableCollided.dist_sigma = native_divide(advmap.y,100.0f);

#define GPU

#include <SureGPUData.h>
#include <func_common.c>

__kernel
__attribute__((vec_type_hint(float3),work_group_size_hint(16,16,1)))
void Trace(        __global float* rgbmatrix, // картинка, в которую рисуем
                   __SURE_DECLARE_RANDOM float* Randomf, // массив случайных чисел
                   __constant struct SureGPUData* GPUData, // структура с общими настройками рендера
                   __global struct SureDrawable* Drawables, // набор объектов для отрисовки
                   __read_only image2d_t VrtxCLImg, // Набор vertexов
                   __read_only image2d_t Textures, // Текстуры
                   __read_only image2d_t MeshCLImg, // Mesh'ы
                   __read_only image2d_t UVMap, // мэппинг мешей на текстуры
                   __read_only image2d_t Normals // нормали
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
                         CLK_ADDRESS_NONE            |
                         CLK_FILTER_LINEAR;
int2 coords;
float2 map_uv;
uint4 col_rgba;
uint4 advmap;
if(x>=GPUData->CameraInfo.m_amx||y>=GPUData->CameraInfo.m_amy)return; // не рисуем за перделами области
// общая для CPU и GPU функция трассировки

#define SET_TRACE_POINT_MINUS \
intersect_dist-=SURE_R_DELTA_GPU_FIX; \
TracePoint = fma(intersect_dist,TraceVector,TracePoint);

#define SET_TRACE_POINT_PLUS \
intersect_dist+=SURE_R_DELTA_GPU_FIX; \
TracePoint = fma(intersect_dist,TraceVector,TracePoint);

#include <trace_common_d.c>
}
