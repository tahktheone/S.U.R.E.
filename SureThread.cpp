#include "SureThread.h"

SureThread::~SureThread()
{
    //dtor
}

void SureThread::oclInit()
{

    cl_platform_id platforms[5];
    cl_uint platforms_num;
    OCL_RUN_("clGetPlatformIDs",clGetPlatformIDs(5,platforms,&platforms_num));
    char l_info[100];
    for(cl_uint i = 0;i<platforms_num;++i)
    {
        OCL_RUN_("clGetPlatformInfo",clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,100,&l_info,NULL));
        if(ret==CL_SUCCESS){ std::cout << "Платформа №" << i << ":" << l_info << "\n";};
    };
    std::cout << "Используется платформа №0." << "\n";
    // всыбираем устройство
    cl_device_id devices[5];
    cl_uint dev_num;
    char devname[500];
    size_t ret_size;
    OCL_RUN_("clGetDeviceIDs",clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 5, devices, &dev_num));
    if(ret==CL_SUCCESS)
    {
        for(cl_uint i = 0;i<dev_num;++i)
        {
            std::cout << "Устройкство №" << i << ":" << "\n";
            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_NAME,300,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){ std::cout << "Имя:" << devname << "\n"; };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_VENDOR,300,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Поставщик:" << devname << "\n";
            };
            cl_ulong mem_size;
            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_GLOBAL_MEM_SIZE,300,(void*)&mem_size,&ret_size));
            if(ret==CL_SUCCESS)
            {
               mem_size /= 1024*1024;
               std::cout << "Память:" << mem_size << "МБ\n";
            };

            cl_uint cores;
            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_COMPUTE_UNITS,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Ядра:" << cores << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_CLOCK_FREQUENCY,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Частота:" << cores << "МГц\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_ADDRESS_BITS,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Разрядность адреса:" << cores << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_EXTENSIONS,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Доступные расширения:" << devname << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "макс. разрядность вектора double:" << cores << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_OPENCL_C_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Поддерживается версия OpenCL:" << devname << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Версия устройства:" << devname << "\n";
            };

            OCL_RUN_("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DRIVER_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               std::cout << "Версия драйвера:" << devname << "\n";
            };

            bool img_supp;

            OCL_RUN_("clGetDeviceInfo(CL_DEVICE_IMAGE_SUPPORT)",clGetDeviceInfo(devices[i],CL_DEVICE_IMAGE_SUPPORT,500,(void*)&img_supp,&ret_size));
            if(ret==CL_SUCCESS)
            {
                if(img_supp)
                {
                    std::cout << "Аппаратная поддержка изображений -- есть\n";
                }else{
                    std::cout << "Аппаратной поддержки изображений -- нет :(\n";
                };
            };

        };
    };
    device = devices[0];
    std::cout << "Используется устройство №0." << "\n";
    // создание вычислительного контекста для GPU (видеокарты)
    OCL_GET_("clCreateContext",context,clCreateContext( NULL, 1, &device, NULL, NULL, &ret));

    cl_image_format cif_vertex;
    cif_vertex.image_channel_data_type = CL_FLOAT;
    cif_vertex.image_channel_order = CL_RGBA;

    cl_image_format cif_mesh;
    cif_mesh.image_channel_data_type = CL_SIGNED_INT32;
    cif_mesh.image_channel_order = CL_RGBA;

    cl_image_format cif_textures;
    cif_textures.image_channel_data_type = CL_UNSIGNED_INT8;
    cif_textures.image_channel_order = CL_BGRA;

    cl_image_format formats[100];
    cl_uint formats_c = 0;
    OCL_RUN_("clGetSupportedImageFormats",clGetSupportedImageFormats(context,
        CL_MEM_READ_ONLY ,
        CL_MEM_OBJECT_IMAGE2D,
        100,
        formats,
        &formats_c));
    if(ret==CL_SUCCESS)
    {
        std::cout << "Поддерживаемые форматы изображений:\n";
        for(cl_uint u = 0;u<formats_c;++u)
        {
            if((formats[u].image_channel_order==cif_vertex.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_vertex.image_channel_data_type))
            {
                std::cout
                << "CL_RGBA/CL_FLOAT(vertex image) -- поддерживается\n";
            };
            if((formats[u].image_channel_order==cif_mesh.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_mesh.image_channel_data_type))
            {
                std::cout
                << "CL_RGBA/CL_SIGNED_INT32(mesh image) -- поддерживается\n";
            };
            if((formats[u].image_channel_order==cif_textures.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_textures.image_channel_data_type))
            {
                std::cout
                << "CL_BGRA/CL_UNSIGNED_INT8(textures image) -- поддерживается\n";
            };

        };
    };


    // создаем очередь команд
    OCL_GET_("clCreateCommandQueue",OCLData->cqCommandQue,clCreateCommandQueue(context, device, 0,&ret));
    // Создаем буфферы обмены данными между CPU и GPU
    OCL_GET_("clCreateBuffer(RGBmatrix)",OCLData->cmRGBmatrix,clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * SURE_MAXRES_X*SURE_MAXRES_Y*3*SURE_FAA*SURE_FAA, NULL, &ret));
    OCL_GET_("clCreateBuffer(GPUData)",OCLData->cmGPUData,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(SureGPUData), NULL, &ret));
    OCL_GET_("clCreateBuffer(Drawables)",OCLData->cmDrawables,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(SureDrawable) * SURE_DR_MAX, NULL, &ret));
    OCL_GET_("clCreateBuffer(Random)",OCLData->cmRandom,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * SURE_R_RNDSIZE, NULL, &ret));

/*
    cl_image_desc cid_vertex;
    cid_vertex.image_type = CL_MEM_OBJECT_IMAGE1D;
    cid_vertex.image_width = SURE_R_MAXVERTEX;
    cid_vertex.image_height = 1;
    cid_vertex.image_depth = 1;
    cid_vertex.image_array_size = 1;
    cid_vertex.image_row_pitch = sizeof(cl_float)*CLSIZE_VERTEX;
    cid_vertex.image_slice_pitch = 0;
    cid_vertex.num_mip_levels = 0;
    cid_vertex.num_samples = 0;
    cid_vertex.buffer = NULL;
*/
    //OCL_GET_("clCreateImage(Vertexes)",OCLData->cmVertexImage,clCreateImage(context,CL_MEM_READ_ONLY,&cif_vertex,&cid_vertex,NULL,&ret));

    OCL_GET_("clCreateImage(Vertexes)",OCLData->cmVertexImage,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET_("clCreateImage(Meshes)",OCLData->cmMeshImage,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_mesh,CLSIZE_VERTEX_DIV,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET_("clCreateImage(Normals)",OCLData->cmNormals,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET_("clCreateImage(Textures)",OCLData->cmTextures,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_textures,SURE_R_TEXRES,SURE_R_TEXRES*SURE_R_MAXTEX,0,NULL,&ret));
    OCL_GET_("clCreateImage(UVMap)",OCLData->cmUVMap,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM,CLSIZE_VERTEX_DIV,0,NULL,&ret));

    // vvv Объявления для макроса OCL_PROGRAM()
    size_t source_size = 0;
    char *source_str = (char *)malloc(SURE_CL_MAXSOURCE);
    char log_data[30000];
    size_t logsize;
    // ^^^ Объявления для макроса OCL_PROGRAM()

    OCL_PROGRAM(program_d,"../raytrace_t.cl","-w -cl-fast-relaxed-math -I./ -I../ -D SURE_RLEVEL=99");
    OCL_PROGRAM(program_t,"../raytrace_t.cl","-w -cl-fast-relaxed-math -I./ -I../ -D SURE_GPU_FLOAT -D SURE_RLEVEL=99");
    OCL_PROGRAM(program_f,"../raytrace_t.cl","-w -cl-fast-relaxed-math -I./ -I../ -D SURE_GPU_FLOAT -D SURE_RLEVEL=50");
    OCL_PROGRAM(program_n,"../raytrace_t.cl","-w -cl-fast-relaxed-math -I./ -I../ -D SURE_GPU_FLOAT -D SURE_RLEVEL=10");
    OCL_GET_("clCreateKernel",OCLData->kernel_t,clCreateKernel(program_t,"Trace",NULL));
    OCL_GET_("clCreateKernel",OCLData->kernel_f,clCreateKernel(program_f,"Trace",NULL));
    OCL_GET_("clCreateKernel",OCLData->kernel_d,clCreateKernel(program_d,"Trace",NULL));
    OCL_GET_("clCreateKernel",OCLData->kernel_n,clCreateKernel(program_n,"Trace",NULL));
    OCLData->kernel = &OCLData->kernel_t;
    OCLData->rtype = SURE_RT_T;
    OCL_RUN_("clSetKernelArg 0t",clSetKernelArg(OCLData->kernel_t, 0, sizeof(cl_mem), (void*)&OCLData->cmRGBmatrix));
    OCL_RUN_("clSetKernelArg 1t",clSetKernelArg(OCLData->kernel_t, 1, sizeof(cl_mem), (void*)&OCLData->cmRandom));
    OCL_RUN_("clSetKernelArg 2t",clSetKernelArg(OCLData->kernel_t, 2, sizeof(cl_mem), (void*)&OCLData->cmGPUData));
    OCL_RUN_("clSetKernelArg 3t",clSetKernelArg(OCLData->kernel_t, 3, sizeof(cl_mem), (void*)&OCLData->cmDrawables));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_t, 4, sizeof(cl_mem), (void*)&OCLData->cmVertexImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_t, 5, sizeof(cl_mem), (void*)&OCLData->cmTextures));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_t, 6, sizeof(cl_mem), (void*)&OCLData->cmMeshImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_t, 7, sizeof(cl_mem), (void*)&OCLData->cmUVMap));
    OCL_RUN_("clSetKernelArg 9t",clSetKernelArg(OCLData->kernel_t, 8, sizeof(cl_mem), (void*)&OCLData->cmNormals));
    OCL_RUN_("clSetKernelArg 0f",clSetKernelArg(OCLData->kernel_f, 0, sizeof(cl_mem), (void*)&OCLData->cmRGBmatrix));
    OCL_RUN_("clSetKernelArg 1f",clSetKernelArg(OCLData->kernel_f, 1, sizeof(cl_mem), (void*)&OCLData->cmRandom));
    OCL_RUN_("clSetKernelArg 2f",clSetKernelArg(OCLData->kernel_f, 2, sizeof(cl_mem), (void*)&OCLData->cmGPUData));
    OCL_RUN_("clSetKernelArg 3f",clSetKernelArg(OCLData->kernel_f, 3, sizeof(cl_mem), (void*)&OCLData->cmDrawables));
    OCL_RUN_("clSetKernelArg 8f",clSetKernelArg(OCLData->kernel_f, 4, sizeof(cl_mem), (void*)&OCLData->cmVertexImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_f, 5, sizeof(cl_mem), (void*)&OCLData->cmTextures));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_f, 6, sizeof(cl_mem), (void*)&OCLData->cmMeshImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_f, 7, sizeof(cl_mem), (void*)&OCLData->cmUVMap));
    OCL_RUN_("clSetKernelArg 9t",clSetKernelArg(OCLData->kernel_f, 8, sizeof(cl_mem), (void*)&OCLData->cmNormals));
    OCL_RUN_("clSetKernelArg 0d",clSetKernelArg(OCLData->kernel_d, 0, sizeof(cl_mem), (void*)&OCLData->cmRGBmatrix));
    OCL_RUN_("clSetKernelArg 1d",clSetKernelArg(OCLData->kernel_d, 1, sizeof(cl_mem), (void*)&OCLData->cmRandom));
    OCL_RUN_("clSetKernelArg 2d",clSetKernelArg(OCLData->kernel_d, 2, sizeof(cl_mem), (void*)&OCLData->cmGPUData));
    OCL_RUN_("clSetKernelArg 3d",clSetKernelArg(OCLData->kernel_d, 3, sizeof(cl_mem), (void*)&OCLData->cmDrawables));
    OCL_RUN_("clSetKernelArg 8d",clSetKernelArg(OCLData->kernel_d, 4, sizeof(cl_mem), (void*)&OCLData->cmVertexImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_d, 5, sizeof(cl_mem), (void*)&OCLData->cmTextures));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_d, 6, sizeof(cl_mem), (void*)&OCLData->cmMeshImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_d, 7, sizeof(cl_mem), (void*)&OCLData->cmUVMap));
    OCL_RUN_("clSetKernelArg 9t",clSetKernelArg(OCLData->kernel_d, 8, sizeof(cl_mem), (void*)&OCLData->cmNormals));
    OCL_RUN_("clSetKernelArg 0n",clSetKernelArg(OCLData->kernel_n, 0, sizeof(cl_mem), (void*)&OCLData->cmRGBmatrix));
    OCL_RUN_("clSetKernelArg 1n",clSetKernelArg(OCLData->kernel_n, 1, sizeof(cl_mem), (void*)&OCLData->cmRandom));
    OCL_RUN_("clSetKernelArg 2n",clSetKernelArg(OCLData->kernel_n, 2, sizeof(cl_mem), (void*)&OCLData->cmGPUData));
    OCL_RUN_("clSetKernelArg 3n",clSetKernelArg(OCLData->kernel_n, 3, sizeof(cl_mem), (void*)&OCLData->cmDrawables));
    OCL_RUN_("clSetKernelArg 8n",clSetKernelArg(OCLData->kernel_n, 4, sizeof(cl_mem), (void*)&OCLData->cmVertexImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_n, 5, sizeof(cl_mem), (void*)&OCLData->cmTextures));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_n, 6, sizeof(cl_mem), (void*)&OCLData->cmMeshImage));
    OCL_RUN_("clSetKernelArg 8t",clSetKernelArg(OCLData->kernel_n, 7, sizeof(cl_mem), (void*)&OCLData->cmUVMap));
    OCL_RUN_("clSetKernelArg 9t",clSetKernelArg(OCLData->kernel_n, 8, sizeof(cl_mem), (void*)&OCLData->cmNormals));
    if(!OCLData->OpenCL)
        std::cout << "Запускаемся без OpenCL!\n";

}

void SureThread::oclDelete()
{
    OCL_RUN_("clReleaseCommandQueue",clReleaseCommandQueue(OCLData->cqCommandQue));
    OCL_RUN_("clReleaseKernel",clReleaseKernel(OCLData->kernel_t));
    OCL_RUN_("clReleaseProgram",clReleaseProgram(program_t));
    OCL_RUN_("clReleaseKernel",clReleaseKernel(OCLData->kernel_f));
    OCL_RUN_("clReleaseProgram",clReleaseProgram(program_f));
    OCL_RUN_("clReleaseKernel",clReleaseKernel(OCLData->kernel_d));
    OCL_RUN_("clReleaseProgram",clReleaseProgram(program_d));
    OCL_RUN_("clReleaseKernel",clReleaseKernel(OCLData->kernel_n));
    OCL_RUN_("clReleaseProgram",clReleaseProgram(program_n));
    OCL_RUN_("clReleaseMemObject(RGBmatrix)",clReleaseMemObject(OCLData->cmRGBmatrix));
    OCL_RUN_("clReleaseMemObject(GPUData)",clReleaseMemObject(OCLData->cmGPUData));
    OCL_RUN_("clReleaseMemObject(Drawables)",clReleaseMemObject(OCLData->cmDrawables));
    OCL_RUN_("clReleaseMemObject(Random)",clReleaseMemObject(OCLData->cmRandom));
    OCL_RUN_("clReleaseMemObject(Textures)",clReleaseMemObject(OCLData->cmTextures));
    OCL_RUN_("clReleaseMemObject(TexMapData)",clReleaseMemObject(OCLData->cmUVMap));
    OCL_RUN_("clReleaseMemObject(Vertexes)",clReleaseMemObject(OCLData->cmVertexImage));
    OCL_RUN_("clReleaseMemObject(Meshes)",clReleaseMemObject(OCLData->cmMeshImage));
    OCL_RUN_("clReleaseContext",clReleaseContext(context));
}

void SureThread::run()
{
    // Данные отладки:
    struct timespec framestart; // время начала текущего фрейма
    struct timespec frametime; // время отсечения
    // для обеспечения частоты кадров:
    struct timespec lframestart; // время начала текущего фрейма
    struct timespec lframetime; // время отсечения
    float tick = 0;

    oclInit();
    OCL_RUN_("clEnqueueWriteBuffer",clEnqueueWriteBuffer(OCLData->cqCommandQue,OCLData->cmRGBmatrix,CL_TRUE,0,sizeof(cl_float)*SURE_MAXRES_X*SURE_MAXRES_Y*3*SURE_FAA*SURE_FAA,(void*)widget->rgbmatrix,0,NULL,NULL));
    cl_int ret = 0;
    srand(time(NULL));

    clock_gettime(CLOCK_MONOTONIC,&lframestart);

    while(m_running&&widget!=nullptr)
    {
        clock_gettime(CLOCK_MONOTONIC,&framestart);
        cl_float* r = Randomf;
        for(int i = 0;i<SURE_R_RNDSIZE;++i)
        {
            *r = (float)rand()/(float)RAND_MAX;
            ++r;
        };
        OCL_RUN_("clEnqueueWriteBuffer",clEnqueueWriteBuffer(OCLData->cqCommandQue,OCLData->cmRandom,CL_FALSE,0,sizeof(cl_float)*SURE_R_RNDSIZE,(void*)Randomf,0,NULL,NULL));
        GPUData->curandom = 0;
        //clock_gettime(CLOCK_MONOTONIC,&frametime);

        if(OCLData->OpenCL)
        {

            if(OCLData->rtype==SURE_RT_D)
                OCLData->kernel = &OCLData->kernel_d;
            if(OCLData->rtype==SURE_RT_T)
                OCLData->kernel = &OCLData->kernel_t;
            if(OCLData->rtype==SURE_RT_F)
                OCLData->kernel = &OCLData->kernel_f;
            if(OCLData->rtype==SURE_RT_N)
                OCLData->kernel = &OCLData->kernel_n;
            OCLData->sizes[0] = widget->rect().right()*SURE_FAA/SURE_SCALE;
            OCLData->sizes[1] = widget->rect().bottom()*SURE_FAA/SURE_SCALE;
            OCL_RUN_("clEnqueueWriteBuffer(GPUData)",clEnqueueWriteBuffer(OCLData->cqCommandQue,OCLData->cmGPUData,CL_TRUE,0,sizeof(SureGPUData),(void*)GPUData,0,NULL,NULL));
            OCL_RUN_("clEnqueueWriteBuffer(Drawables)",clEnqueueWriteBuffer(OCLData->cqCommandQue,OCLData->cmDrawables,CL_TRUE,0,sizeof(SureDrawable)*GPUData->m_drawables,(void*)GPUData->Drawables,0,NULL,NULL));
            for(int o = 0;o<EngineData->m_objects;++o)
            {
                if( EngineData->objects[o].drawable.mesh_changed&&
                    EngineData->objects[o].drawable.type==SURE_DR_MESH)
                {

                    size_t origin[3];
                    size_t region[3];
                    int ymin = EngineData->objects[o].drawable.mesh_start>>CLSIZE_VERTEX_SHF;
                    int ymax = (EngineData->objects[o].drawable.mesh_start
                               +EngineData->objects[o].drawable.mesh_count)>>CLSIZE_VERTEX_SHF;

                    origin[0] = 0;
                    origin[1] = ymin;
                    origin[2] = 0;

                    region[0] = CLSIZE_VERTEX_DIV;
                    region[1] = ymax+1;
                    region[2] = 1;

                    OCL_RUN_("clEnqueueWriteImage(meshIm)",
                              clEnqueueWriteImage(OCLData->cqCommandQue,
                                                  OCLData->cmMeshImage,
                                                  CL_TRUE,
                                                  origin,
                                                  region,
                                                  sizeof(cl_int)*CLSIZE_VERTEX_PTCH,
                                                  0,
                                                  (void*)&EngineData->MeshCLImg[origin[1]*CLSIZE_VERTEX_PTCH],
                                                  0,NULL,NULL));

                    ymin = EngineData->objects[o].drawable.mesh_start>>CLSIZE_VERTEX_SHF;
                    ymax = (EngineData->objects[o].drawable.mesh_start
                               +EngineData->objects[o].drawable.mesh_count)>>CLSIZE_VERTEX_SHF;

                    origin[0] = 0;
                    origin[1] = ymin;
                    origin[2] = 0;

                    region[0] = CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM;
                    region[1] = ymax+1;
                    region[2] = 1;

                    OCL_RUN_("clEnqueueWriteImage(UVMap)",
                              clEnqueueWriteImage(OCLData->cqCommandQue,
                                                  OCLData->cmUVMap,
                                                  CL_TRUE,
                                                  origin,
                                                  region,
                                                  sizeof(cl_float)*CLSIZE_MESH_PTCH,
                                                  0,
                                                  (void*)&EngineData->UVMap[origin[1]*CLSIZE_MESH_PTCH],
                                                  0,NULL,NULL));

                    OCL_RUN_("clEnqueueWriteImage(Normals)",
                              clEnqueueWriteImage(OCLData->cqCommandQue,
                                                  OCLData->cmNormals,
                                                  CL_TRUE,
                                                  origin,
                                                  region,
                                                  sizeof(cl_float)*CLSIZE_MESH_PTCH,
                                                  0,
                                                  (void*)&EngineData->Normals[origin[1]*CLSIZE_MESH_PTCH],
                                                  0,NULL,NULL));


                    int v_min = SURE_R_MAXVERTEX;
                    int v_max = 0;
                    int* MeshCLImg = EngineData->MeshCLImg;
                    for(cl_uint m = 0;m<EngineData->objects[o].drawable.mesh_count;++m)
                    {
                        int k = EngineData->objects[o].drawable.mesh_start+m;
                        if(v_min>__MESH_V1(k))v_min = __MESH_V1(k);
                        if(v_max<__MESH_V1(k))v_max = __MESH_V1(k);
                        if(v_min>__MESH_V2(k))v_min = __MESH_V2(k);
                        if(v_max<__MESH_V2(k))v_max = __MESH_V2(k);
                        if(v_min>__MESH_V3(k))v_min = __MESH_V3(k);
                        if(v_max<__MESH_V3(k))v_max = __MESH_V3(k);
                    };
                    if(v_min<v_max)
                    {
                    size_t origin[3];
                    size_t region[3];
                    int ymin = v_min>>CLSIZE_VERTEX_SHF;
                    int ymax = v_max>>CLSIZE_VERTEX_SHF;

                    origin[0] = 0;
                    origin[1] = ymin;
                    origin[2] = 0;

                    region[0] = CLSIZE_VERTEX_DIV;
                    region[1] = ymax+1;
                    region[2] = 1;

                    OCL_RUN_("clEnqueueWriteImage(vertIm)",
                              clEnqueueWriteImage(OCLData->cqCommandQue,
                                                  OCLData->cmVertexImage,
                                                  CL_TRUE,
                                                  origin,
                                                  region,
                                                  sizeof(cl_float)*CLSIZE_VERTEX_PTCH,
                                                  0,
                                                  (void*)&EngineData->VrtxCLImg[origin[1]*CLSIZE_VERTEX_PTCH],
                                                  0,NULL,NULL));
                   };
                   EngineData->objects[o].drawable.mesh_changed = false;
                };
            };
            for(cl_uint t = 0;t<EngineData->cur_textures;++t)
            if(EngineData->TexturesInfo[t].toupdate)
            {
                    size_t origin[3];
                    size_t region[3];

                    origin[0] = 0;
                    origin[1] = t*SURE_R_TEXRES;
                    origin[2] = 0;

                    region[0] = SURE_R_TEXRES;
                    region[1] = SURE_R_TEXRES;
                    region[2] = 1;

                    OCL_RUN_("clEnqueueWriteImage(Textures)",
                              clEnqueueWriteImage(OCLData->cqCommandQue,
                                                  OCLData->cmTextures,
                                                  CL_TRUE,
                                                  origin,
                                                  region,
                                                  sizeof(cl_uchar)*SURE_R_TEXRES*4,
                                                  0,
                                                  (void*)&EngineData->TexturesData[t*SURE_R_TEXRES*SURE_R_TEXRES*4],
                                                  0,NULL,NULL));
                EngineData->TexturesInfo[t].toupdate = false;
            };
            OCL_RUN_("Running kernel",clEnqueueNDRangeKernel(OCLData->cqCommandQue,*OCLData->kernel,2,NULL,OCLData->sizes,NULL,0,NULL,NULL));
            OCL_RUN_("clFinish",clFinish(OCLData->cqCommandQue));
        }else{
            raytrace();
        };

        GPUData->reset = false;

        clock_gettime(CLOCK_MONOTONIC,&frametime);
        widget->rendertime = (float)frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  (float)framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );

        clock_gettime(CLOCK_MONOTONIC,&lframetime);
        tick = (float)lframetime.tv_sec * 1000.0 + (float) lframetime.tv_nsec / 1000000.0 - (  (float)lframestart.tv_sec*1000.0 + (float) lframestart.tv_nsec / 1000000.0 );
        if(tick>SURE_R_DELAY)
        {
            if(widget!=nullptr)
                OCL_RUN_("clEnqueueReadBuffer",clEnqueueReadBuffer(OCLData->cqCommandQue,OCLData->cmRGBmatrix,CL_TRUE,0,sizeof(cl_float)*SURE_MAXRES_X*SURE_MAXRES_Y*3*SURE_FAA*SURE_FAA,widget->rgbmatrix,0,NULL,NULL));
            widget->update();
            clock_gettime(CLOCK_MONOTONIC,&lframestart);

            if(GPUData->toreset)
            {
                GPUData->reset = true;
                GPUData->toreset = false;
            };
        };
    };
    oclDelete();
} // run()

void SureThread::raytrace()
{

    float* rgbmatrix = widget->rgbmatrix;
    SureDrawable* Drawables = GPUData->Drawables;
    cl_uchar* Textures = EngineData->TexturesData; // Текстуры
    cl_float* UVMap = EngineData->UVMap; // мэппинг мешей на текстуры
    cl_float* Normals = EngineData->Normals; // мэппинг мешей на текстуры
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей

    int amx = widget->rect().right()*SURE_FAA/SURE_SCALE;
    int amy = widget->rect().bottom()*SURE_FAA/SURE_SCALE;
    #pragma omp parallel for schedule(dynamic)
    for(int y=0;y<amy;++y)
        {
        for(int x=0;x<amx;++x)
        {
           #include <trace_common.c>
        };//X
    };//Y ; parallel
}
