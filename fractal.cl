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

        double znx;
        double zny;
        double cx;
        double cy;
        double znpx;
        double znpy;
        cy = GPUData->cam_x.y+GPUData->xy_h*y*SURE_SCALE;
        cx = GPUData->cam_x.x+GPUData->xy_h*x*SURE_SCALE;
        znx = cx; zny = cy;
        znpx = mad(znx,znx,-mad(zny,zny,-cx));
        znpy = mad(znx,zny,mad(zny,znx,cy));
        int iter = 0;
        while((mad(znpx,znpx,znpy*znpy)<100.0)&&(iter<(255*SURE_FR_ITERS-1)))
        {
            znx = znpx; zny = znpy;
            znpx = mad(znx,znx,-mad(zny,zny,-cx));
            znpy = mad(znx,zny,mad(zny,znx,cy));
            ++iter;
        };
        iter /= SURE_FR_ITERS;
        int d = SURE_MAXRES_X*3*SURE_FAA;
        int k;
        for(int sx=0;sx<SURE_SCALE;++sx)
        for(int sy=0;sy<SURE_SCALE;++sy)
        {
            k = y*d*SURE_SCALE+sy*d+x*3*SURE_SCALE+sx*3;
            if(GPUData->reset==1)
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
