#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#include <SureDefines.h>
#define GPU
#include <SureGPUData.h>

__kernel void Noise(__global float* rgbmatrix,
                   __global float* Random,
                   __global struct SureGPUData* GPUData)
{
	uint x = get_global_id(0);
	uint y = get_global_id(1);

	uint d = SURE_MAXRES_X*3*SURE_FAA;

    uint xx = x+y;
    uint yy = y;
    uint ri = mad24(xx,xx+yy,xx+yy);
    while(ri>=SURE_R_RNDSIZE)ri-=SURE_R_RNDSIZE;
    float3 rgb;
    if(++ri>=SURE_R_RNDSIZE)ri-=SURE_R_RNDSIZE;
    rgb.x = 255.0*Random[ri];
    if(++ri>=SURE_R_RNDSIZE)ri-=SURE_R_RNDSIZE;
    rgb.y = 255.0*Random[ri];
    if(++ri>=SURE_R_RNDSIZE)ri-=SURE_R_RNDSIZE;
    rgb.z = 255.0*Random[ri];

    for(uint sx=0;sx<SURE_SCALE;++sx)
    for(uint sy=0;sy<SURE_SCALE;++sy)
    {
        uint k = y*d*SURE_SCALE+sy*d+x*3*SURE_SCALE+sx*3;
        if(GPUData->reset==1)
        {
            rgbmatrix[k] = rgb.x;
            rgbmatrix[k+1] = rgb.y;
            rgbmatrix[k+2] = rgb.z;
        }else{
            rgbmatrix[k] += rgb.x;
            rgbmatrix[k+1] += rgb.y;
            rgbmatrix[k+2] += rgb.z;
        };
    };
};

