#include "SureThread.h"

SureThread::SureThread(SureData* i_engine):QThread(){
    EngineData = i_engine;
}

SureThread::~SureThread()
{
    //dtor
}

int SureThread::SourceFromFile(const char* i_fname,char* e_source)
{
    FILE *fp;
    int e_size = 0;
    fp = fopen(i_fname, "r");
    if (!fp) {
        char LogLine[500];
        sprintf(LogLine,"Не удается открыть файл %s",i_fname);
        Log->AddLine(LogLine);
    }else{
        e_size = fread(e_source,1,SURE_CL_MAXSOURCE,fp);
    };
    fclose( fp );
    return e_size;
}

void SureThread::oclInit()
{
    char LogLine[500];
    cl_platform_id platforms[5];
    cl_uint platforms_num;
    OCL_RUN("clGetPlatformIDs",clGetPlatformIDs(5,platforms,&platforms_num));
    char l_info[100];
    for(cl_uint i = 0;i<platforms_num;++i)
    {
        OCL_RUN("clGetPlatformInfo",clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,100,&l_info,NULL));
        if(ret==CL_SUCCESS){ sprintf(LogLine,"Платформа №%i (%s)",i,l_info);Log->AddLine(LogLine);};
    };
    Log->AddLine("Используется платформа №0");
    // всыбираем устройство
    cl_device_id devices[5];
    cl_uint dev_num;
    char devname[500];
    size_t ret_size;
    OCL_RUN("clGetDeviceIDs",clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 5, devices, &dev_num));
    if(ret==CL_SUCCESS){
        for(cl_uint i = 0;i<dev_num;++i){
            sprintf(LogLine,"Устройство №%i:",i); Log->AddLine(LogLine);
            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_NAME,300,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){ sprintf(LogLine,"Имя - %s",devname); Log->AddLine(LogLine); };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_VENDOR,300,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS)
            {
               sprintf(LogLine,"Поставщик - %s",devname); Log->AddLine(LogLine);
            };
            cl_ulong mem_size;
            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_GLOBAL_MEM_SIZE,300,(void*)&mem_size,&ret_size));
            if(ret==CL_SUCCESS)
            {
               mem_size /= 1024*1024;
               sprintf(LogLine,"Память: %i МБ",(int)mem_size); Log->AddLine(LogLine);
            };

            cl_uint cores;
            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_COMPUTE_UNITS,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               sprintf(LogLine,"Ядра: %i",cores); Log->AddLine(LogLine);
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_CLOCK_FREQUENCY,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
                sprintf(LogLine,"Частота: %i МГц",cores); Log->AddLine(LogLine);
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_ADDRESS_BITS,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS)
            {
               sprintf(LogLine,"Разрядность адреса: %i бит",cores); Log->AddLine(LogLine);
            };

            Log->AddLine("Параметры параллелизации (размер группы/изменерия/размерность):");

            cl_uint grps = 0;
            cl_uint dims = 0;

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_WORK_GROUP_SIZE,300,(void*)&grps,&ret_size));
            // пока убираем автоматическое определение размера группы. Похоже, это не очень помогает
            // if(ret==CL_SUCCESS){ EngineData->OCLData.g_workgroup_size = sqrt(grps)/2; };
            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,300,(void*)&dims,&ret_size));

            sprintf(LogLine,"Размер группы параррелизации %i, изменений %i",grps,dims);
            Log->AddLine(LogLine);

            size_t l_szs[100];

            Log->AddLine("Размерности:");

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_MAX_WORK_ITEM_SIZES,300,(void*)l_szs,&ret_size));
            if(ret==CL_SUCCESS){
                for(uint i=0;i<dims;++i){
                    sprintf(LogLine,"Измерение %i размер %i",i,(int)l_szs[i]);
                    Log->AddLine(LogLine);
                };
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_EXTENSIONS,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){
               Log->AddBigText("Доступные расширения:",devname);
            };


            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,300,(void*)&cores,&ret_size));
            if(ret==CL_SUCCESS){
                    sprintf(LogLine,"Макс. разрядность вектора double: %i",cores);
                    Log->AddLine(LogLine);
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_OPENCL_C_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){
                    sprintf(LogLine,"Поддерживается версия OpenCL: %s",devname);
                    Log->AddLine(LogLine);
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DEVICE_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){
                    sprintf(LogLine,"Версия устройства: %s",devname);
                    Log->AddLine(LogLine);
            };

            OCL_RUN("clGetDeviceInfo",clGetDeviceInfo(devices[i],CL_DRIVER_VERSION,500,(void*)devname,&ret_size));
            if(ret==CL_SUCCESS){
                    sprintf(LogLine,"Версия драйвера: %s",devname);
                    Log->AddLine(LogLine);
            };

            bool img_supp;

            OCL_RUN("clGetDeviceInfo(CL_DEVICE_IMAGE_SUPPORT)",clGetDeviceInfo(devices[i],CL_DEVICE_IMAGE_SUPPORT,500,(void*)&img_supp,&ret_size));
            if(ret==CL_SUCCESS){
                if(img_supp){
                    Log->AddLine("Аппаратная поддержка изображений есть");
                }else{
                    Log->AddLine("Аппаратной поддержки изображений нет");
                };
            };

        }; // Цикл по устройствам
    };
    device = devices[0];
    Log->AddLine("Используется устройство №0.");
    // создание вычислительного контекста для GPU (видеокарты)
    OCL_GET("clCreateContext",context,clCreateContext( NULL, 1, &device, NULL, NULL, &ret));

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
    OCL_RUN("clGetSupportedImageFormats",clGetSupportedImageFormats(context,
        CL_MEM_READ_ONLY ,
        CL_MEM_OBJECT_IMAGE2D,
        100,
        formats,
        &formats_c));
    if(ret==CL_SUCCESS){
        Log->AddLine("Поддерживаемые форматы изображений:");
        for(cl_uint u = 0;u<formats_c;++u)
        {
            if((formats[u].image_channel_order==cif_vertex.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_vertex.image_channel_data_type)){
                Log->AddLine("CL_RGBA/CL_FLOAT(vertex image) -- поддерживается");
            };
            if((formats[u].image_channel_order==cif_mesh.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_mesh.image_channel_data_type)){
                Log->AddLine("CL_RGBA/CL_SIGNED_INT32(mesh image) -- поддерживается");
            };
            if((formats[u].image_channel_order==cif_textures.image_channel_order)&&
              (formats[u].image_channel_data_type==cif_textures.image_channel_data_type)){
                Log->AddLine("CL_BGRA/CL_UNSIGNED_INT8(textures image) -- поддерживается");
            };
        };
    };

    // создаем очередь команд
    OCL_GET("clCreateCommandQueue",EngineData->OCLData.cqCommandQue,clCreateCommandQueue(context, device, 0,&ret));
    // Создаем буфферы обмены данными между CPU и GPU
    OCL_GET("clCreateBuffer(RGBmatrix)",EngineData->OCLData.cmRGBmatrix,clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * SURE_MAXRES_X*SURE_MAXRES_Y*3, NULL, &ret));
    OCL_GET("clCreateBuffer(GPUData)",EngineData->OCLData.cmGPUData,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(SureGPUData), NULL, &ret));
    OCL_GET("clCreateBuffer(Drawables)",EngineData->OCLData.cmDrawables,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(SureDrawable) * SURE_DR_MAX, NULL, &ret));
    OCL_GET("clCreateBuffer(Random)",EngineData->OCLData.cmRandom,clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * SURE_R_RNDSIZE, NULL, &ret));

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
    //OCL_GET("clCreateImage(Vertexes)",EngineData->OCLData.cmVertexImage,clCreateImage(context,CL_MEM_READ_ONLY,&cif_vertex,&cid_vertex,NULL,&ret));

    OCL_GET("clCreateImage(Vertexes)",EngineData->OCLData.cmVertexImage,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET("clCreateImage(Meshes)",EngineData->OCLData.cmMeshImage,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_mesh,CLSIZE_VERTEX_DIV,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET("clCreateImage(Normals)",EngineData->OCLData.cmNormals,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM,CLSIZE_VERTEX_DIV,0,NULL,&ret));
    OCL_GET("clCreateImage(Textures)",EngineData->OCLData.cmTextures,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_textures,SURE_R_TEXRES,SURE_R_TEXRES*SURE_R_MAXTEX,0,NULL,&ret));
    OCL_GET("clCreateImage(UVMap)",EngineData->OCLData.cmUVMap,clCreateImage2D(context,CL_MEM_READ_ONLY,&cif_vertex,CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM,CLSIZE_VERTEX_DIV,0,NULL,&ret));

    // vvv Объявления для макроса OCL_PROGRAM()
    size_t source_size = 0;
    char *source_str = (char *)malloc(SURE_CL_MAXSOURCE);
    char log_data[30000];
    size_t logsize;
    // ^^^ Объявления для макроса OCL_PROGRAM()

    // -cl-fp32-correctly-rounded-divide-sqrt -- попробовать на новой версии...
    // -cl-single-precision-constant
    // -cl-opt-disable -- выключить оптимизацию, всю
    // -Werror -- все предупреждения = ошибки. -w -- подавлять предупреждения

    OCL_PROGRAM(program_d,"./CL/raytrace.cl","-w -cl-fast-relaxed-math -I./CL -D VERSION0_0003_1 -D SURE_RLEVEL=99"); //99
    OCL_PROGRAM(program_t,"./CL/raytrace.cl","-w -cl-fast-relaxed-math -I./CL -D VERSION0_0003_1 -D SURE_RLEVEL=65"); //65
    OCL_PROGRAM(program_f,"./CL/raytrace.cl","-w -cl-fast-relaxed-math -I./CL -D VERSION0_0003_1 -D SURE_RLEVEL=45"); //45
    OCL_PROGRAM(program_n,"./CL/raytrace.cl","-w -cl-fast-relaxed-math -I./CL -D VERSION0_0003_1 -D SURE_RLEVEL=10"); //10
    OCL_GET("clCreateKernel",EngineData->OCLData.kernel_t,clCreateKernel(program_t,"Trace",NULL));
    OCL_GET("clCreateKernel",EngineData->OCLData.kernel_f,clCreateKernel(program_f,"Trace",NULL));
    OCL_GET("clCreateKernel",EngineData->OCLData.kernel_d,clCreateKernel(program_d,"Trace",NULL));
    OCL_GET("clCreateKernel",EngineData->OCLData.kernel_n,clCreateKernel(program_n,"Trace",NULL));
    EngineData->OCLData.kernel = &EngineData->OCLData.kernel_d;
    EngineData->r_type = SURE_RT_D;
    OCL_RUN("clSetKernelArg 0t",clSetKernelArg(EngineData->OCLData.kernel_t, 0, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRGBmatrix));
    OCL_RUN("clSetKernelArg 1t",clSetKernelArg(EngineData->OCLData.kernel_t, 1, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRandom));
    OCL_RUN("clSetKernelArg 2t",clSetKernelArg(EngineData->OCLData.kernel_t, 2, sizeof(cl_mem), (void*)&EngineData->OCLData.cmGPUData));
    OCL_RUN("clSetKernelArg 3t",clSetKernelArg(EngineData->OCLData.kernel_t, 3, sizeof(cl_mem), (void*)&EngineData->OCLData.cmDrawables));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_t, 4, sizeof(cl_mem), (void*)&EngineData->OCLData.cmVertexImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_t, 5, sizeof(cl_mem), (void*)&EngineData->OCLData.cmTextures));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_t, 6, sizeof(cl_mem), (void*)&EngineData->OCLData.cmMeshImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_t, 7, sizeof(cl_mem), (void*)&EngineData->OCLData.cmUVMap));
    OCL_RUN("clSetKernelArg 9t",clSetKernelArg(EngineData->OCLData.kernel_t, 8, sizeof(cl_mem), (void*)&EngineData->OCLData.cmNormals));
    OCL_RUN("clSetKernelArg 0f",clSetKernelArg(EngineData->OCLData.kernel_f, 0, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRGBmatrix));
    OCL_RUN("clSetKernelArg 1f",clSetKernelArg(EngineData->OCLData.kernel_f, 1, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRandom));
    OCL_RUN("clSetKernelArg 2f",clSetKernelArg(EngineData->OCLData.kernel_f, 2, sizeof(cl_mem), (void*)&EngineData->OCLData.cmGPUData));
    OCL_RUN("clSetKernelArg 3f",clSetKernelArg(EngineData->OCLData.kernel_f, 3, sizeof(cl_mem), (void*)&EngineData->OCLData.cmDrawables));
    OCL_RUN("clSetKernelArg 8f",clSetKernelArg(EngineData->OCLData.kernel_f, 4, sizeof(cl_mem), (void*)&EngineData->OCLData.cmVertexImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_f, 5, sizeof(cl_mem), (void*)&EngineData->OCLData.cmTextures));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_f, 6, sizeof(cl_mem), (void*)&EngineData->OCLData.cmMeshImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_f, 7, sizeof(cl_mem), (void*)&EngineData->OCLData.cmUVMap));
    OCL_RUN("clSetKernelArg 9t",clSetKernelArg(EngineData->OCLData.kernel_f, 8, sizeof(cl_mem), (void*)&EngineData->OCLData.cmNormals));
    OCL_RUN("clSetKernelArg 0d",clSetKernelArg(EngineData->OCLData.kernel_d, 0, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRGBmatrix));
    OCL_RUN("clSetKernelArg 1d",clSetKernelArg(EngineData->OCLData.kernel_d, 1, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRandom));
    OCL_RUN("clSetKernelArg 2d",clSetKernelArg(EngineData->OCLData.kernel_d, 2, sizeof(cl_mem), (void*)&EngineData->OCLData.cmGPUData));
    OCL_RUN("clSetKernelArg 3d",clSetKernelArg(EngineData->OCLData.kernel_d, 3, sizeof(cl_mem), (void*)&EngineData->OCLData.cmDrawables));
    OCL_RUN("clSetKernelArg 8d",clSetKernelArg(EngineData->OCLData.kernel_d, 4, sizeof(cl_mem), (void*)&EngineData->OCLData.cmVertexImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_d, 5, sizeof(cl_mem), (void*)&EngineData->OCLData.cmTextures));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_d, 6, sizeof(cl_mem), (void*)&EngineData->OCLData.cmMeshImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_d, 7, sizeof(cl_mem), (void*)&EngineData->OCLData.cmUVMap));
    OCL_RUN("clSetKernelArg 9t",clSetKernelArg(EngineData->OCLData.kernel_d, 8, sizeof(cl_mem), (void*)&EngineData->OCLData.cmNormals));
    OCL_RUN("clSetKernelArg 0n",clSetKernelArg(EngineData->OCLData.kernel_n, 0, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRGBmatrix));
    OCL_RUN("clSetKernelArg 1n",clSetKernelArg(EngineData->OCLData.kernel_n, 1, sizeof(cl_mem), (void*)&EngineData->OCLData.cmRandom));
    OCL_RUN("clSetKernelArg 2n",clSetKernelArg(EngineData->OCLData.kernel_n, 2, sizeof(cl_mem), (void*)&EngineData->OCLData.cmGPUData));
    OCL_RUN("clSetKernelArg 3n",clSetKernelArg(EngineData->OCLData.kernel_n, 3, sizeof(cl_mem), (void*)&EngineData->OCLData.cmDrawables));
    OCL_RUN("clSetKernelArg 8n",clSetKernelArg(EngineData->OCLData.kernel_n, 4, sizeof(cl_mem), (void*)&EngineData->OCLData.cmVertexImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_n, 5, sizeof(cl_mem), (void*)&EngineData->OCLData.cmTextures));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_n, 6, sizeof(cl_mem), (void*)&EngineData->OCLData.cmMeshImage));
    OCL_RUN("clSetKernelArg 8t",clSetKernelArg(EngineData->OCLData.kernel_n, 7, sizeof(cl_mem), (void*)&EngineData->OCLData.cmUVMap));
    OCL_RUN("clSetKernelArg 9t",clSetKernelArg(EngineData->OCLData.kernel_n, 8, sizeof(cl_mem), (void*)&EngineData->OCLData.cmNormals));
    if(!EngineData->OCLData.OpenCL)
        Log->AddLine("Запускаемся без OpenCL!");

    free(source_str);
}

void SureThread::oclDelete()
{
    OCL_RUN("clReleaseCommandQueue",clReleaseCommandQueue(EngineData->OCLData.cqCommandQue));
    OCL_RUN("clReleaseKernel",clReleaseKernel(EngineData->OCLData.kernel_t));
    OCL_RUN("clReleaseProgram",clReleaseProgram(program_t));
    OCL_RUN("clReleaseKernel",clReleaseKernel(EngineData->OCLData.kernel_f));
    OCL_RUN("clReleaseProgram",clReleaseProgram(program_f));
    OCL_RUN("clReleaseKernel",clReleaseKernel(EngineData->OCLData.kernel_d));
    OCL_RUN("clReleaseProgram",clReleaseProgram(program_d));
    OCL_RUN("clReleaseKernel",clReleaseKernel(EngineData->OCLData.kernel_n));
    OCL_RUN("clReleaseProgram",clReleaseProgram(program_n));
    OCL_RUN("clReleaseMemObject(RGBmatrix)",clReleaseMemObject(EngineData->OCLData.cmRGBmatrix));
    OCL_RUN("clReleaseMemObject(GPUData)",clReleaseMemObject(EngineData->OCLData.cmGPUData));
    OCL_RUN("clReleaseMemObject(Drawables)",clReleaseMemObject(EngineData->OCLData.cmDrawables));
    OCL_RUN("clReleaseMemObject(Random)",clReleaseMemObject(EngineData->OCLData.cmRandom));
    OCL_RUN("clReleaseMemObject(Textures)",clReleaseMemObject(EngineData->OCLData.cmTextures));
    OCL_RUN("clReleaseMemObject(TexMapData)",clReleaseMemObject(EngineData->OCLData.cmUVMap));
    OCL_RUN("clReleaseMemObject(Vertexes)",clReleaseMemObject(EngineData->OCLData.cmVertexImage));
    OCL_RUN("clReleaseMemObject(Meshes)",clReleaseMemObject(EngineData->OCLData.cmMeshImage));
    OCL_RUN("clReleaseContext",clReleaseContext(context));
}

void SureThread::SwitchKernel()
{
    if(EngineData->r_type==SURE_RT_D) EngineData->OCLData.kernel = &EngineData->OCLData.kernel_d;
    if(EngineData->r_type==SURE_RT_T) EngineData->OCLData.kernel = &EngineData->OCLData.kernel_t;
    if(EngineData->r_type==SURE_RT_F) EngineData->OCLData.kernel = &EngineData->OCLData.kernel_f;
    if(EngineData->r_type==SURE_RT_N) EngineData->OCLData.kernel = &EngineData->OCLData.kernel_n;
}

void SureThread::WriteModelToGPU(SureModelData *i_model)
{
    size_t origin[3];
    size_t region[3];
    int ymin = i_model->mesh_start>>CLSIZE_VERTEX_SHF;
    int ymax = (i_model->mesh_start+i_model->mesh_count)>>CLSIZE_VERTEX_SHF;

    origin[0] = 0;
    origin[1] = ymin;
    origin[2] = 0;

    region[0] = CLSIZE_VERTEX_DIV;
    region[1] = ymax+1;
    region[2] = 1;

    OCL_RUN("clEnqueueWriteImage(meshIm)",
              clEnqueueWriteImage(EngineData->OCLData.cqCommandQue,
              EngineData->OCLData.cmMeshImage,
              CL_TRUE,
              origin,
              region,
              sizeof(cl_int)*CLSIZE_VERTEX_PTCH,
              0,
              (void*)&EngineData->MeshCLImg[origin[1]*CLSIZE_VERTEX_PTCH],
              0,NULL,NULL));

    ymin = i_model->mesh_start>>CLSIZE_VERTEX_SHF;
    ymax = (i_model->mesh_start+i_model->mesh_count)>>CLSIZE_VERTEX_SHF;

    origin[0] = 0;
    origin[1] = ymin;
    origin[2] = 0;

    region[0] = CLSIZE_VERTEX_DIV*CLSIZE_MESH_DIM;
    region[1] = ymax+1;
    region[2] = 1;

    OCL_RUN("clEnqueueWriteImage(UVMap)",
              clEnqueueWriteImage(EngineData->OCLData.cqCommandQue,
              EngineData->OCLData.cmUVMap,
              CL_TRUE,
              origin,
              region,
              sizeof(cl_float)*CLSIZE_MESH_PTCH,
              0,
              (void*)&EngineData->UVMap[origin[1]*CLSIZE_MESH_PTCH],
              0,NULL,NULL));

    OCL_RUN("clEnqueueWriteImage(Normals)",
              clEnqueueWriteImage(EngineData->OCLData.cqCommandQue,
              EngineData->OCLData.cmNormals,
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
    for(cl_uint m = 0;m<i_model->mesh_count;++m){
        int k = i_model->mesh_start+m;
        if(v_min>__MESH_V1(k))v_min = __MESH_V1(k);
        if(v_max<__MESH_V1(k))v_max = __MESH_V1(k);
        if(v_min>__MESH_V2(k))v_min = __MESH_V2(k);
        if(v_max<__MESH_V2(k))v_max = __MESH_V2(k);
        if(v_min>__MESH_V3(k))v_min = __MESH_V3(k);
        if(v_max<__MESH_V3(k))v_max = __MESH_V3(k);
    };
    if(v_min<v_max){
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

        OCL_RUN("clEnqueueWriteImage(vertIm)",
                  clEnqueueWriteImage(EngineData->OCLData.cqCommandQue,
                  EngineData->OCLData.cmVertexImage,
                  CL_TRUE,
                  origin,
                  region,
                  sizeof(cl_float)*CLSIZE_VERTEX_PTCH,
                  0,
                  (void*)&EngineData->VrtxCLImg[origin[1]*CLSIZE_VERTEX_PTCH],
                  0,NULL,NULL));
    };
    i_model->toupdate = false;
}

void SureThread::WriteTextureToGPU(int TextureID)
{
    size_t origin[3];
    size_t region[3];

    origin[0] = 0;
    origin[1] = TextureID*SURE_R_TEXRES;
    origin[2] = 0;

    region[0] = SURE_R_TEXRES;
    region[1] = SURE_R_TEXRES;
    region[2] = 1;

    OCL_RUN("clEnqueueWriteImage(Textures)",
              clEnqueueWriteImage(EngineData->OCLData.cqCommandQue,
              EngineData->OCLData.cmTextures,
              CL_TRUE,
              origin,
              region,
              sizeof(cl_uchar)*SURE_R_TEXRES*4,
              0,
              (void*)&EngineData->TexturesData[TextureID*SURE_R_TEXRES*SURE_R_TEXRES*4],
              0,NULL,NULL));

    EngineData->TexturesInfo[TextureID].toupdate = false;
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
    Log = new SureLog("render");

    oclInit();
    OCL_RUN("clEnqueueWriteBuffer",clEnqueueWriteBuffer(EngineData->OCLData.cqCommandQue,EngineData->OCLData.cmRGBmatrix,CL_TRUE,0,sizeof(cl_float)*SURE_MAXRES_X*SURE_MAXRES_Y*3,(void*)EngineData->rgbmatrix,0,NULL,NULL));
    cl_int ret = 0;
    srand(time(NULL));

    clock_gettime(CLOCK_MONOTONIC,&lframestart);

    while(m_running){
        clock_gettime(CLOCK_MONOTONIC,&framestart);
        if(!EngineData->Loading)continue;

        // Генерируем набор случайных чисел от 0 до 1
        // чтобы не изобретать генератор случайных чисел на GPU
        for(int i = 0;i<SURE_R_RNDSIZE;++i) EngineData->Randomf[i] = (float)rand()/(float)RAND_MAX;

        OCL_RUN("clEnqueueWriteBuffer(Randomf)",clEnqueueWriteBuffer(EngineData->OCLData.cqCommandQue,EngineData->OCLData.cmRandom,CL_FALSE,0,sizeof(cl_float)*SURE_R_RNDSIZE,(void*)EngineData->Randomf,0,NULL,NULL));

        if(EngineData->OCLData.OpenCL){
            SwitchKernel();
            EngineData->OCLData.sizes[0] = EngineData->GPUData.CameraInfo.m_amx;
            EngineData->OCLData.sizes[1] = EngineData->GPUData.CameraInfo.m_amy;
            // Выравниваем размер области по размеру локальной рабочей группы
            EngineData->OCLData.sizes[0] /= EngineData->OCLData.g_workgroup_size;
            EngineData->OCLData.sizes[0] *= EngineData->OCLData.g_workgroup_size;
            EngineData->OCLData.sizes[0] += EngineData->OCLData.g_workgroup_size;
            EngineData->OCLData.sizes[1] /= EngineData->OCLData.g_workgroup_size;
            EngineData->OCLData.sizes[1] *= EngineData->OCLData.g_workgroup_size;
            EngineData->OCLData.sizes[1] += EngineData->OCLData.g_workgroup_size;
            OCL_RUN("clEnqueueWriteBuffer(GPUData)",clEnqueueWriteBuffer(EngineData->OCLData.cqCommandQue,EngineData->OCLData.cmGPUData,CL_TRUE,0,sizeof(SureGPUData),(void*)&EngineData->GPUData,0,NULL,NULL));
            OCL_RUN("clEnqueueWriteBuffer(Drawables)",clEnqueueWriteBuffer(EngineData->OCLData.cqCommandQue,EngineData->OCLData.cmDrawables,CL_TRUE,0,sizeof(SureDrawable)*EngineData->GPUData.m_drawables,(void*)EngineData->Drawables,0,NULL,NULL));

            for(uint ModelID = 0;ModelID<EngineData->cur_models;++ModelID)
            if (EngineData->ModelsInfo[ModelID].toupdate)
                WriteModelToGPU(&EngineData->ModelsInfo[ModelID]);

            for(cl_uint t = 0;t<EngineData->cur_textures;++t)
            if(EngineData->TexturesInfo[t].toupdate)
                WriteTextureToGPU(t);

            size_t l_lws[3];
            l_lws[0] = EngineData->OCLData.g_workgroup_size;
            l_lws[1] = EngineData->OCLData.g_workgroup_size;
            l_lws[2] = 1;
            int h_gr = SURE_WGRPSIZE; //EngineData->OCLData.g_workgroup_size*16;
            size_t l_start[2];
            size_t l_gws[2];
            l_gws[0] = h_gr;
            l_gws[1] = h_gr;

            for(uint i_x_gr=0;i_x_gr<EngineData->OCLData.sizes[0];i_x_gr+=h_gr)
            for(uint i_y_gr=0;i_y_gr<EngineData->OCLData.sizes[1];i_y_gr+=h_gr){
                l_start[0]=i_x_gr;
                l_start[1]=i_y_gr;
                OCL_RUN("Running kernel (raytrace)",clEnqueueNDRangeKernel(EngineData->OCLData.cqCommandQue, // Очередь
                                                                *EngineData->OCLData.kernel, // Программа
                                                                2, // 2 измерения для ID потока (X * Y)
                                                                l_start, // стартовые координаты (тип size_t[измерения])
                                                                l_gws, // размер области обработки (тип size_t[измерения])
                                                                l_lws, // размер локальной области (workgroup)
                                                                0,NULL, // события, которые нужно дождаться перед запуском
                                                                NULL)); // событие, которое генерится по завершении работы
                OCL_RUN("clFinish",clFinish(EngineData->OCLData.cqCommandQue));
            };

        }else{
            raytrace();
        };

        EngineData->GPUData.reset = false;

        clock_gettime(CLOCK_MONOTONIC,&frametime);
        EngineData->rendertime = (float)frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  (float)framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );

        clock_gettime(CLOCK_MONOTONIC,&lframetime);
        tick = (float)lframetime.tv_sec * 1000.0 + (float) lframetime.tv_nsec / 1000000.0 - (  (float)lframestart.tv_sec*1000.0 + (float) lframestart.tv_nsec / 1000000.0 );
        if(tick>SURE_R_DELAY){
            OCL_RUN("clEnqueueReadBuffer",clEnqueueReadBuffer(EngineData->OCLData.cqCommandQue,EngineData->OCLData.cmRGBmatrix,CL_TRUE,0,sizeof(cl_float)*SURE_MAXRES_X*SURE_MAXRES_Y*3,EngineData->rgbmatrix,0,NULL,NULL));
            widget->update();
            clock_gettime(CLOCK_MONOTONIC,&lframestart);

            if(GPUData->toreset){
                GPUData->reset = true;
                GPUData->toreset = false;
            };
        };
    };
    oclDelete();
    delete Log;
} // run()

void SureThread::raytrace()
{
    int amx = GPUData->CameraInfo.m_amx;
    int amy = GPUData->CameraInfo.m_amy;
    #pragma omp parallel for schedule(static,8)
    for(int y=0;y<amy;++y)
    for(int x=0;x<amx;++x)
        EngineData->SinglePointTrace(x,y,GPUData,Randomf,EngineData->rgbmatrix);
}
