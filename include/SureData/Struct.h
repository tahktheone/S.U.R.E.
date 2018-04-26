struct SureParticle
{
    my_double3 OX;
    my_double3 X;
    double Size = 1.0;
};

struct SureLink
{
    SureObject* o1;
    SureObject* o2;
    double l;
    double k;
};

struct SureTextureData
{
    bool toupdate;
    char name[20];
};

struct SureModelData
{
    bool toupdate;
    uint mesh_start;
    uint mesh_count;
    uint vertex_start;
    uint vertex_count;
    char name[20];
};

struct SureTraceLogItem{
    my_double3 TraceVector;
    my_double3 TracePoint;
    my_double3 CollisionNomal;
    float IntersectDistance;
    my_double3 Fade;
    my_uchar3 Color;
    my_uchar3 rgb_current;
    my_double3 NormalRandomized;
    int iter;
    int rechecks;
    float transp_i = 0;
};

struct SureTraceLog{
    SureTraceLogItem Items[20];
    uint ItemsCount = 0;
};

struct SureControllerInput{
    int type = 0;
    int key = 0;
    float x = 0;
    float y = 0;
};

struct SureControllerAction{
    int type = 0;
};

struct SureControl{
    bool active = false;
    SureControllerInput Input;
    SureControllerAction Action;
};

struct SureOCLData{
    bool OpenCL = true;
    //int rtype = SURE_RT_D;
    cl_kernel* kernel;

    cl_kernel kernel_t;
    cl_kernel kernel_f;
    cl_kernel kernel_d;
    cl_kernel kernel_n;

    size_t sizes[2] = {1920,1080};
    // OpenCL штуки
    cl_command_queue cqCommandQue;
    cl_mem cmRGBmatrix;
    cl_mem cmGPUData;
    cl_mem cmDrawables;
    cl_mem cmRandom;

    cl_mem cmVertexImage; //указатель на массив vertex'ов
    cl_mem cmMeshImage; //  указатель на массив mesh'ей
    cl_mem cmTextures; //указатель на массив текстур
    cl_mem cmUVMap; // указатель на массив uv-mappingа
    cl_mem cmNormals; // указатель на массив uv-mappingа
};
