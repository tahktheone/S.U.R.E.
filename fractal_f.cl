#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#include <SureDefines.h>
#define GPU
#include <SureGPUData.h>
__kernel void Frac(__global float* rgbmatrix,
                   __global uchar* palette,
                   __global struct SureGPUData* GPUData)
{
	// координаты обрабатываемой точки
	size_t x = get_global_id(0);
    size_t y = get_global_id(1);
        float2 zn;
        float2 c;
        float2 znp1;
        c.y = convert_float(GPUData->cam_x.y)+convert_float(GPUData->xy_h)*y*SURE_SCALE;
        c.x = convert_float(GPUData->cam_x.x)+convert_float(GPUData->xy_h)*x*SURE_SCALE;
        zn = c;
        znp1.x = mad(zn.x,zn.x,-mad(zn.y,zn.y,-c.x));
        znp1.y = mad(zn.x,zn.y,mad(zn.y,zn.x,c.y));
        int iter = 0;
        while(((mad(znp1.x,znp1.x,znp1.y*znp1.y))<100.0)&&(iter<(255*SURE_FR_ITERS-1)))
        {
            zn = znp1;
            znp1.x = mad(zn.x,zn.x,-mad(zn.y,zn.y,-c.x));
            znp1.y = mad(zn.x,zn.y,mad(zn.y,zn.x,c.y));
            ++iter;
        };
        iter /= SURE_FR_ITERS;
        int d = SURE_MAXRES_X*3*SURE_FAA;
        int k;
        for(int sx=0;sx<SURE_SCALE;++sx)
        for(int sy=0;sy<SURE_SCALE;++sy)
        {
            k = y*d*SURE_SCALE+sy*d+x*3*SURE_SCALE+sx*3;
            if(GPUData->reset)
            {
                rgbmatrix[k] = palette[iter*3];
                rgbmatrix[k+1] = palette[iter*3+1];
                rgbmatrix[k+2] = palette[iter*3+2];
            }else{
                rgbmatrix[k] += palette[iter*3];
                rgbmatrix[k+1] += palette[iter*3+1];
                rgbmatrix[k+2] += palette[iter*3+2];
            };
        };
}
