#include "SureData.h"

void SureData::SureClear()
{
    delete TexturesData;
    delete UVMap;
    delete Normals;
    delete VrtxCLImg;
    delete MeshCLImg;
};

#include <SureData_load.cpp>
#include <SureData_gen.cpp>
#include <SureData_scene.cpp>

SureData::SureData()
{

        VrtxCLImg = new cl_float[CLSIZE_VERTEX]; // 256*256
        MeshCLImg = new cl_int[CLSIZE_VERTEX]; //256*256
        TexturesData = new cl_uchar[SURE_R_MAXTEX * SURE_R_TEXRES * SURE_R_TEXRES * 4];
        UVMap = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        Normals = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        srand(time(NULL));

        LoadTexture("parket");
        LoadTexture("earth_adv");
        LoadTexture("colstones");
        LoadTexture("golem");
        //LoadTexture("grid");
        LoadTexture("golem_adv");
        LoadTexture("earth");

    int i;

  //Scene_cube(0,0,20,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(20,-30,20,19,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(-20,20,15,14,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_YZ);
  //Scene_box();
  Scene_floor();

  //Scene_tetrs();
  //Scene_tetra(20,10,30,40,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);
  //Scene_tetra(25,15,80,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);

  //Scene_tetra(-50,10,20,80,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);
  //Scene_tetra(-25,-30,20,70,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);

  //Scene_golem();
  //Scene_metaball(-30,-30,40,40,SURE_NORMALS_SHPERICAL);
  //Scene_metaball(30,30,20,20,SURE_NORMALS_DEFAULT);
}

SureData::~SureData()
{

}

#include <func_common.c>

void ocl_errtext(cl_int i_ret)
{
    switch(i_ret)
    {
        case  CL_SUCCESS: std::cout << "CL_SUCCESS ";break;
        case  CL_DEVICE_NOT_FOUND: std::cout << "CL_DEVICE_NOT_FOUND ";break;
        case  CL_DEVICE_NOT_AVAILABLE: std::cout << "CL_DEVICE_NOT_AVAILABLE ";break;
        case  CL_COMPILER_NOT_AVAILABLE: std::cout << "CL_COMPILER_NOT_AVAILABLE ";break;
        case  CL_MEM_OBJECT_ALLOCATION_FAILURE: std::cout << "CL_MEM_OBJECT_ALLOCATION_FAILURE ";break;
        case  CL_OUT_OF_RESOURCES: std::cout << "CL_OUT_OF_RESOURCES ";break;
        case  CL_OUT_OF_HOST_MEMORY: std::cout << "CL_OUT_OF_HOST_MEMORY ";break;
        case  CL_PROFILING_INFO_NOT_AVAILABLE: std::cout << "CL_PROFILING_INFO_NOT_AVAILABLE ";break;
        case  CL_MEM_COPY_OVERLAP: std::cout << "CL_MEM_COPY_OVERLAP ";break;
        case  CL_IMAGE_FORMAT_MISMATCH: std::cout << "CL_IMAGE_FORMAT_MISMATCH ";break;
        case  CL_IMAGE_FORMAT_NOT_SUPPORTED: std::cout << "CL_IMAGE_FORMAT_NOT_SUPPORTED ";break;
        case  CL_BUILD_PROGRAM_FAILURE: std::cout << "CL_BUILD_PROGRAM_FAILURE ";break;
        case  CL_MAP_FAILURE: std::cout << "CL_MAP_FAILURE ";break;
        case  CL_MISALIGNED_SUB_BUFFER_OFFSET: std::cout << "CL_MISALIGNED_SUB_BUFFER_OFFSET ";break;
        case  CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: std::cout << "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST ";break;
        case  CL_COMPILE_PROGRAM_FAILURE: std::cout << "CL_COMPILE_PROGRAM_FAILURE ";break;
        case  CL_LINKER_NOT_AVAILABLE: std::cout << "CL_LINKER_NOT_AVAILABLE ";break;
        case  CL_LINK_PROGRAM_FAILURE: std::cout << "CL_LINK_PROGRAM_FAILURE ";break;
        case  CL_DEVICE_PARTITION_FAILED: std::cout << "CL_DEVICE_PARTITION_FAILED ";break;
        case  CL_KERNEL_ARG_INFO_NOT_AVAILABLE: std::cout << "CL_KERNEL_ARG_INFO_NOT_AVAILABLE ";break;
        case  CL_INVALID_VALUE: std::cout << "CL_INVALID_VALUE ";break;
        case  CL_INVALID_DEVICE_TYPE: std::cout << "CL_INVALID_DEVICE_TYPE ";break;
        case  CL_INVALID_PLATFORM: std::cout << "CL_INVALID_PLATFORM ";break;
        case  CL_INVALID_DEVICE: std::cout << "CL_INVALID_DEVICE ";break;
        case  CL_INVALID_CONTEXT: std::cout << "CL_INVALID_CONTEXT ";break;
        case  CL_INVALID_QUEUE_PROPERTIES: std::cout << "CL_INVALID_QUEUE_PROPERTIES ";break;
        case  CL_INVALID_COMMAND_QUEUE: std::cout << "CL_INVALID_COMMAND_QUEUE ";break;
        case  CL_INVALID_HOST_PTR: std::cout << "CL_INVALID_HOST_PTR ";break;
        case  CL_INVALID_MEM_OBJECT: std::cout << "CL_INVALID_MEM_OBJECT ";break;
        case  CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: std::cout << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR ";break;
        case  CL_INVALID_IMAGE_SIZE: std::cout << "CL_INVALID_IMAGE_SIZE ";break;
        case  CL_INVALID_SAMPLER: std::cout << "CL_INVALID_SAMPLER ";break;
        case  CL_INVALID_BINARY: std::cout << "CL_INVALID_BINARY ";break;
        case  CL_INVALID_BUILD_OPTIONS: std::cout << "CL_INVALID_BUILD_OPTIONS ";break;
        case  CL_INVALID_PROGRAM: std::cout << "CL_INVALID_PROGRAM ";break;
        case  CL_INVALID_PROGRAM_EXECUTABLE: std::cout << "CL_INVALID_PROGRAM_EXECUTABLE ";break;
        case  CL_INVALID_KERNEL_NAME: std::cout << "CL_INVALID_KERNEL_NAME ";break;
        case  CL_INVALID_KERNEL_DEFINITION: std::cout << "CL_INVALID_KERNEL_DEFINITION ";break;
        case  CL_INVALID_KERNEL: std::cout << "CL_INVALID_KERNEL ";break;
        case  CL_INVALID_ARG_INDEX: std::cout << "CL_INVALID_ARG_INDEX ";break;
        case  CL_INVALID_ARG_VALUE: std::cout << "CL_INVALID_ARG_VALUE ";break;
        case  CL_INVALID_ARG_SIZE: std::cout << "CL_INVALID_ARG_SIZE ";break;
        case  CL_INVALID_KERNEL_ARGS: std::cout << "CL_INVALID_KERNEL_ARGS ";break;
        case  CL_INVALID_WORK_DIMENSION: std::cout << "CL_INVALID_WORK_DIMENSION ";break;
        case  CL_INVALID_WORK_GROUP_SIZE: std::cout << "CL_INVALID_WORK_GROUP_SIZE ";break;
        case  CL_INVALID_WORK_ITEM_SIZE: std::cout << "CL_INVALID_WORK_ITEM_SIZE ";break;
        case  CL_INVALID_GLOBAL_OFFSET: std::cout << "CL_INVALID_GLOBAL_OFFSET ";break;
        case  CL_INVALID_EVENT_WAIT_LIST: std::cout << "CL_INVALID_EVENT_WAIT_LIST ";break;
        case  CL_INVALID_EVENT: std::cout << "CL_INVALID_EVENT ";break;
        case  CL_INVALID_OPERATION: std::cout << "CL_INVALID_OPERATION ";break;
        case  CL_INVALID_GL_OBJECT: std::cout << "CL_INVALID_GL_OBJECT ";break;
        case  CL_INVALID_BUFFER_SIZE: std::cout << "CL_INVALID_BUFFER_SIZE ";break;
        case  CL_INVALID_MIP_LEVEL: std::cout << "CL_INVALID_MIP_LEVEL ";break;
        case  CL_INVALID_GLOBAL_WORK_SIZE: std::cout << "CL_INVALID_GLOBAL_WORK_SIZE ";break;
        case  CL_INVALID_PROPERTY: std::cout << "CL_INVALID_PROPERTY ";break;
        case  CL_INVALID_IMAGE_DESCRIPTOR: std::cout << "CL_INVALID_IMAGE_DESCRIPTOR ";break;
        case  CL_INVALID_COMPILER_OPTIONS: std::cout << "CL_INVALID_COMPILER_OPTIONS ";break;
        case  CL_INVALID_LINKER_OPTIONS: std::cout << "CL_INVALID_LINKER_OPTIONS ";break;
        case  CL_INVALID_DEVICE_PARTITION_COUNT: std::cout << "CL_INVALID_DEVICE_PARTITION_COUNT ";break;
    };
};

int SourceFromFile(const char* i_fname,char* e_source)
{
    FILE *fp;
    int e_size = 0;
    fp = fopen(i_fname, "r");
    if (!fp) {
        std::cout << "Не удается найти файл " << i_fname << "\n";
    }else{
        e_size = fread(e_source,1,SURE_CL_MAXSOURCE,fp);
    };
    fclose( fp );
    return e_size;
};

cl_double3 normalize(cl_double3 Vec)
{
    cl_double3 vr;
    double l = sqrt(Vec.s[0]*Vec.s[0] + Vec.s[1]*Vec.s[1] + Vec.s[2]*Vec.s[2]);
    vr.s[0] = Vec.s[0]/l; vr.s[1] = Vec.s[1]/l; vr.s[2] = Vec.s[2]/l;
    return vr;
};

my_double3 normalize(my_double3 Vec)
{
    my_double3 vr;
    double l = sqrt(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
    vr.x = Vec.x/l; vr.y = Vec.y/l; vr.z = Vec.z/l;
    return vr;
};

cl_double3 cross(cl_double3 v1, cl_double3 v2)
{
    cl_double3 vr;
    vr.s[0] = v1.s[1]*v2.s[2] - v1.s[2]*v2.s[1];
    vr.s[1] = v1.s[2]*v2.s[0] - v1.s[0]*v2.s[2];
	vr.s[2] = v1.s[0]*v2.s[1] - v1.s[1]*v2.s[0];
	return vr;
};

cl_double dot(cl_double3 v1, cl_double3 v2)
{
    return v1.s[0]*v2.s[0] + v1.s[1]*v2.s[1] + v1.s[2]*v2.s[2];
};

my_double3 cross(my_double3 v1, my_double3 v2)
{
    my_double3 vr;
    vr.x = v1.y*v2.z - v1.z*v2.y;
    vr.y = - v1.x*v2.z + v1.z*v2.x;
	vr.z = v1.x*v2.y - v1.y*v2.x;
	return vr;
};

double dot(my_double3 v1, my_double3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
};

uint mad24(uint x,uint y,uint z)
{
    return x*y+z;
};

const my_double3 operator*(my_double3 a, double b)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

const my_double3 operator*(double b, my_double3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

const my_double3 operator*(double b, my_uchar3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

my_double3& operator+=(my_double3 &a,const my_double3 &b)
{ a.x+=b.x;a.y+=b.y;a.z+=b.z; return a; }

my_double3& operator-=(my_double3 &a,const my_double3 &b)
{ a.x-=b.x;a.y-=b.y;a.z-=b.z; return a; }

void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl)
{
    my_double3 v1 = my_double3{0,0,0};
    my_double3 v2 = my_double3{0,0,0};
    if(o1->movable)v1 = o1->velbypp(pp);
    if(o2->movable)v2 = o2->velbypp(pp);
    my_double3 oz = __NORMALIZE(pd);
    double l_v1z = dot(oz,v1);
    double l_v2z = dot(oz,v2);
    my_double3 v1z = oz*l_v1z;
    my_double3 v1xy = v1-v1z;
    my_double3 v2z = oz*l_v2z;
    my_double3 v2xy = v2-v2z;

    if(o2->movable)o2->push(pp,pd,-pl*0.5);
    if(!o1->movable)o2->push(pp,pd,-pl*0.5);
    if(o1->movable)o1->push(pp,pd,pl*0.5);
    if(!o2->movable)o1->push(pp,pd,pl*0.5);

    if(o2->movable)o2->push(pp,v2xy,-1.0);
    if(o1->movable)o1->push(pp,v1xy,-1.0);

 }
