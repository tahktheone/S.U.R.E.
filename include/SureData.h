#ifndef SUREDATA_H
#define SUREDATA_H

#include <SureDefines.h>
#include <SureDefinesCPU.h>
#include <SureGPUData.h>

struct SureObject
{
    uint external_id;
    int ParentID = -1;
    my_double3 X;  //Координаты центра
    my_double3 ox; //Локальная ось x
    my_double3 oy; //Локальная ось y
    my_double3 oz; //Нормаль (Локальная ось z)
    my_double3 X_byparent;  //Координаты центра
    my_double3 ox_byparent; //Локальная ось x
    my_double3 oy_byparent; //Локальная ось y
    //my_double3 oz_byparent; //Нормаль (Локальная ось z)  - вычисляемая, cross(ox,oy)
    double lx; // длина
    double ly; // ширина
    double lz; // высота
    double lp = 4.0; // величина иннерциального тетраэдра
    double rig = 0.7; // жесткость
    int type; // тип
    bool movable; // может двигаться
    bool collidable;
    my_double3 p1;
    my_double3 p2;
    my_double3 p3;
    my_double3 p4;
    my_double3 p1o;
    my_double3 p2o;
    my_double3 p3o;
    my_double3 p4o;
    my_double3 v1;
    my_double3 v2;
    my_double3 v3;
    my_double3 v4;
    cl_uint ModelID_collider;
    char ModelName_collider[20];
    cl_uint ModelID_drawable;
    char ModelName_drawable[20];
    SureDrawable drawable;

    int DrawableGPUID = -1;


    void align_byparent(SureObject *parent){
                    X = parent->X + X_byparent.x*parent->ox
                                  + X_byparent.y*parent->oy
                                  + X_byparent.z*parent->oz;
                    ox = __NORMALIZE( ox_byparent.x*parent->ox
                                    + ox_byparent.y*parent->oy
                                    + ox_byparent.z*parent->oz);
                    oy = __NORMALIZE( oy_byparent.x*parent->ox
                                    + oy_byparent.y*parent->oy
                                    + oy_byparent.z*parent->oz);
                    oz = __NORMALIZE(cross(ox,oy));
    }
    void initp4(){
                  // тэтраэдр создается чуть ниже чем нужно, поэтому при первом же выравнивании объект "проваливается" вниз
                  // чтобы этого не было -- костыль:
                  X=X+0.1*lp*oz;

                  p1 = X+ox*(SURE_P4_X*lp)-oz*(SURE_P4_Y*lp);
                  p2 = X-ox*(SURE_P4_Y*lp)+oy*(0.5*lp)-oz*(SURE_P4_Y*lp);
                  p3 = X-ox*(SURE_P4_Y*lp)-oy*(0.5*lp)-oz*(SURE_P4_Y*lp);
                  p4 = X+oz*(SURE_P4_Z*lp-SURE_P4_Y*lp);
                  p1o.x=p1.x;p1o.y=p1.y;p1o.z=p1.z;
                  p2o.x=p2.x;p2o.y=p2.y;p2o.z=p2.z;
                  p3o.x=p3.x;p3o.y=p3.y;p3o.z=p3.z;
                  p4o.x=p4.x;p4o.y=p4.y;p4o.z=p4.z;
                  v1.x=0;v1.y=0;v1.z=0;
                  v2.x=0;v2.y=0;v2.z=0;
                  v3.x=0;v3.y=0;v3.z=0;
                  v4.x=0;v4.y=0;v4.z=0;}
    void push(my_double3 pp,my_double3 pv,double pd)
    {
        my_double3 vp1 = pp-p1;
        double lp1 = __LENGTH(vp1);
        my_double3 vp2 = pp-p2;
        double lp2 = __LENGTH(vp2);
        my_double3 vp3 = pp-p3;
        double lp3 = __LENGTH(vp3);
        my_double3 vp4 = pp-p4;
        double lp4 = __LENGTH(vp4);

        my_double3 np5 = pp+pv*pd;

        double d;
        my_double3 vd;

        #define __ITER_5VD(A,B,C,D,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        C += vd*d*0.5; \
        D -= vd*d*0.5;

        #define __ITER_5VDF(A,B,C,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        C += vd*d;

        my_double3 np1=p1;
        my_double3 np2=p2;
        my_double3 np3=p3;
        my_double3 np4=p4;

        for(int i = 0;i<1;++i)
        {
            my_double3 d1 = my_double3{0,0,0};
            my_double3 d2 = my_double3{0,0,0};
            my_double3 d3 = my_double3{0,0,0};
            my_double3 d4 = my_double3{0,0,0};
            __ITER_5VD(np1,np2,d1,d2,lp);
            __ITER_5VD(np1,np3,d1,d3,lp);
            __ITER_5VD(np1,np4,d1,d4,lp);
            __ITER_5VDF(np1,np5,d1,lp1);
            __ITER_5VD(np2,np3,d2,d3,lp);
            __ITER_5VD(np2,np4,d2,d4,lp);
            __ITER_5VDF(np2,np5,d2,lp2);
            __ITER_5VD(np3,np4,d3,d4,lp);
            __ITER_5VDF(np3,np5,d3,lp3);
            __ITER_5VDF(np4,np5,d4,lp4);
            np1+=d1; np2+=d2; np3+=d3; np4+=d4; //np5+=d5;
        };

        p1=np1;
        p2=np2;
        p3=np3;
        p4=np4;

        movebyp4();
    }
    void next_tick()
    {
        v1 = p1-p1o; v2 = p2-p2o; v3 = p3-p3o; v4 = p4-p4o;
        // Затухание
        v1 = v1 - v1*0.001;
        v2 = v2 - v2*0.001;
        v3 = v3 - v3*0.001;
        v4 = v4 - v4*0.001;
        double dv = fabs(v1.x)+fabs(v1.y)+fabs(v1.z)+
                    fabs(v2.x)+fabs(v2.y)+fabs(v2.z)+
                    fabs(v3.x)+fabs(v3.y)+fabs(v3.z)+
                    fabs(v4.x)+fabs(v4.y)+fabs(v4.z);
        dv = dv * lp;
        if(dv > 0.5){
            p1o=p1;p2o=p2;p3o=p3;p4o=p4;
            p1+=v1; p2+=v2; p3+=v3; p4+=v4;
        }else{
            p1=p1o;p2=p2o;p3=p3o;p4=p4o;
        };
        movebyp4();
    }
    void align_p4()
    {
            double d;
            my_double3 pd;
            #define __ITER(A,B,C,D) \
            pd = A-B; \
            d = (lp-__LENGTH(pd))*rig; \
            pd = __NORMALIZE(pd); \
            C += pd*(d*0.5); \
            D -= pd*(d*0.5);
        for(int i = 0;i<1;++i)
        {
            my_double3 d1 = my_double3{0,0,0};
            my_double3 d2 = my_double3{0,0,0};
            my_double3 d3 = my_double3{0,0,0};
            my_double3 d4 = my_double3{0,0,0};
            __ITER(p1,p2,d1,d2);
            __ITER(p1,p3,d1,d3);
            __ITER(p1,p4,d1,d4);
            __ITER(p2,p3,d2,d3);
            __ITER(p2,p4,d2,d4);
            __ITER(p3,p4,d3,d4);
            p1+=d1; //dp1+=d1;
            p2+=d2; //dp2+=d2;
            p3+=d3; //dp3+=d3;
            p4+=d4; //dp4+=d4;
        };
    }
    void movebyp4()
    {
        oy=p2-p3;
        oz=cross(p2-p1,oy);
        ox=cross(oz,oy);
        ox=__NORMALIZE(ox);
        oy=__NORMALIZE(oy);
        oz=__NORMALIZE(oz);
        X=(p1+p2+p3+p4)*0.25;
    }
    my_double3 velbypp(my_double3 pp)
    {
        my_double3 vp1 = pp-p1o;
        double lp1 = __LENGTH(vp1);
        my_double3 vp2 = pp-p2o;
        double lp2 = __LENGTH(vp2);
        my_double3 vp3 = pp-p3o;
        double lp3 = __LENGTH(vp3);
        my_double3 vp4 = pp-p4o;
        double lp4 = __LENGTH(vp4);

        double d;
        my_double3 vd;

        #define __ITER_5VDFV(A,B,D,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        D -= vd*d;

        my_double3 np1=p1;
        my_double3 np2=p2;
        my_double3 np3=p3;
        my_double3 np4=p4;
        my_double3 np5=pp;

        for(int i = 0;i<1;++i)
        {
            my_double3 d5 = my_double3{0,0,0};
            __ITER_5VDFV(np1,np5,d5,lp1);
            __ITER_5VDFV(np2,np5,d5,lp2);
            __ITER_5VDFV(np3,np5,d5,lp3);
            __ITER_5VDFV(np4,np5,d5,lp4);
            np5+=d5;
        };

        return np5-pp;
    };
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

class SureData
{
    public:
        SureData();
        virtual ~SureData();
        void SureClear();
        SureLog *Log;
        char LogLine[255];
        // камера
        SureCameraInfo CameraInfo;
        my_double3 cam_dx = my_double3{0,0,0};
        my_double3 cam_dw = my_double3{0,0,0};
        cl_uchar r_iters = 10;
        cl_uchar r_rechecks = 10;
        cl_float r_backlight = 0.5;
        int r_type = SURE_RT_D;

        bool reset = true; // сброс кадра
        bool paused = true; // Пауза физики

        cl_float *VrtxCLImg; // [SURE_R_MAXVERTEX*4] -- для OpenCL image2d_t
        cl_int *MeshCLImg;  // [SURE_R_MAXMESH*4] -- для OpenCL image2d_t
        cl_uchar *TexturesData; //[SURE_R_MAXTEX * SURE_R_TEXRES * SURE_R_TEXRES * 4];
        cl_float *UVMap;  //[SURE_R_MAXTEXMAP * 4]; {u1, v1 .. }
        cl_float *Normals; // [SURE_R_MAXTEXMAP * 4]; {n1x, n1y, n1z ...}
        cl_uint cur_vertexes = 0;
        cl_uint cur_meshes = 0;
        cl_uint cur_textures = 0;
        cl_uint cur_models = 0;
        SureModelData ModelsInfo[SURE_R_MAXTEX];
        SureTextureData TexturesInfo[SURE_R_MAXTEX];

        void SaveState(const char *);
        void LoadState(const char *);

        void LoadTexture(const char*);
        void LoadModel(const char*);
        int GetTexture(const char*);
        int GenTexture(const char*,int type);
        int GetModel(const char*);
        int GenModel(const char*,int type);

        SureObject TemplateObject; // шаблонный объект
        uint CreateObjectFromTemplate(__VTYPE3* i_X); // создать объект из TemplateObject
        void DeleteObject(uint);
        SureObject *ObjByID(uint i_id); // Объект по внешнему айдишнику

        SureObject objects[SURE_OBJ_MAX];
        SureLink links[SURE_LIN_MAX];
        int m_links = 0;
        int m_objects = 0;
        uint m_ObjExternID = 1;

        // Стандартные объекты
        int TemplateIndex = 0;
        const int Templates = 9;
        timespec ShowTemplateTime;
        bool ShowTemplate = false;
        void SetNextTemplate();
        void SetTemplate_GlassCube(float SideLen);
        void SetTemplate_GlassSphere(float Radius);
        void SetTemplate_DarkCube(float SideLen);
        void SetTemplate_DarkSphere(float Radius);
        void SetTemplate_EarthCube(float SideLen);
        void SetTemplate_EarthSphere(float Radius);
        void SetTemplate_GlowCube(float SideLen);
        void SetTemplate_GlowSphere(float Radius);
        void SetTemplate_RegularTetr(float SideLen);

        void SetTemplate_GlowPlane();
        void SetTemplate_RegularPlane();
        void SetTemplate_FogPlane();
        void SetTemplate_RegularCube();
        void SetTemplate_RegularPandus();


        void ObjCoordsToDrawable(int); // не забыть вернуть в private
        void Mesh_GenerateTetr(int i_model,int norm_type); // не забыть вернуть в private
        void Mesh_GenerateCube(int i_model,int norm_type); // не забыть вернуть в private
        void Mesh_GeneratePandus(int i_model,int norm_type);
        void Mesh_GenerateHull(int i_model,my_double3* vertexes,int vert_count,int norm_type);
        void Mesh_GenNormals(int i_model,int norm_type);
        void MapTexture(int i_model,int type); // не забыть вернуть в private

        // Для отладки
        float frametime = 0;
        int r_drawdebug = 99;
        SureTraceLog TraceLogs[50];
        uint TraceLogsCount = 0;
        int SelectedObject = -1;


    protected:
    private:
        void SetDefaultRotationBasis(int);

        int AddVertex(double x, double y, double z);
        int AddVertex(my_double3 X);
        int AddMesh(int v1, int v2, int v3);

        void Scene_box(); // коробка со светящимся потолком
        void Scene_floor(); // Пол и круглая лампа
        void Scene_mirrors(); // Пол и зеркала
        void Scene_tetrs(); // Пол и светильник -- тетраэдры
        void Scene_golem();
        void Scene_ManyTetrs();
        void Scene_ManySpheres();
        void Scene_Polygon();
        //void Scene_metaball(double x, double y, double z, double sz,int nt);
        //void Scene_cube(double x, double y, double z, double sz,int nt,int mt);
        //void Scene_tetra(double x, double y, double z, double sz,int nt,int mt,bool movable);
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
    int g_workgroup_size = SURE_L_WGRPSIZE;
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

struct SurePhysCollision{
    SureObject* Object1;
    SureObject* Object2;
    my_double3 CollisionPoint;
    my_double3 CollisionVector;
    double CollisionLength;
};

void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl);
bool PhysCollideSphereSphere(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollideSpherePlane(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollideSphereMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollideMeshPlane(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollideMeshMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);

class SureGJK{
    public:
        SureGJK();
        SureGJK(SureData *i_Engine);
        virtual ~SureGJK();

        SurePhysCollision Collision;

        void SetEngine(SureData *i_Engine);
        void SetupMinkowski(SureObject *o1,SureObject *o2);
        void InitiateLoop();
        bool LoopExit();
        bool TIContains00();
        void LoopNextStep();
        void ExitLoop_Collision();
        void ExpandCoverLoop();
        bool CollisionFound();
        void Clear();

    protected:

    private:
        SureData *EngineData;

        void ExitLoop_NoCollision();
        void InitiateCoverLoop();
        bool CoverExpanded();
        void SetCoverExpanded();
        void CheckCoverIterration();
        void ClearCover();
        void SetCollisionPoint();
        double GetMaximumDistanceTo00();
        bool FindNextTNI();
        bool FindFarestMinkowskiByVector(my_double3 Vector,uint *e_result);
        bool IsCoverFaceVisibleByPoint(uint f,uint point);
        void CheckAndAddFaceToNewCover(uint face,uint fndi);
        void AddFaceToNewCover(uint face);
        void CheckAndAddFaceToNewCover(uint p1, uint p2, uint p3);
        void AddFaceToNewCover(uint p1, uint p2, uint p3);
        void SwitchCover();
        void ExpandCover(uint fndi);
        void SetIncover(uint i);
        void ClearNewCover();
        void SetCollisionByCover();
        my_double3 GetCoverFaceNormal(uint face);
        my_double3 GetCollisionPointByObject(SureObject *o,my_double3 i_vector,double *dist);
        uint GetFarestVertexByObj(SureObject *o,my_double3 i_vector);

        double L1;
        double L2;
        double L3;
        double L4;
        bool exit_no_collision = false;
        bool exit_collision = false;
        bool cover_expanded = false;
        bool collision_found = false;

        my_double3 *M; // разность минкосвского
        bool *incover;
        uint *C;
        uint *C_N;
        uint mc = 0; // количество точек в разности минковского
        uint cc = 0;
        uint cc_n = 0;
        uint iter = 0; // подсчет итерраций - гарантия от зацикливаний
        uint CollisionFace; // номер грани cover'а, ближайшей к 0,0
        uint TI[4] = {0,0,0,0};
        uint TNI[4] = {0,0,0,0};
        void TNI_to_TI();
};

// Работа с OpenCL из CPU-части:
void ocl_errtext(cl_int);
int SourceFromFile(const char*,char*);

// Почему дублируются макросы:
// OCL_RUN и OCL_GET обращаются к OCLData напрямую, а OCL_RUN_и OCL_GET_ по указателю.
// Потому, что вызываются из разных потоков.
#define OCL_RUN(text,module) if(OCLData.OpenCL){ret=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);OCLData.OpenCL=false;};}
#define OCL_GET(text,item,module) if(OCLData.OpenCL){item=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);OCLData.OpenCL=false;};}
#define OCL_RUN_(text,module) if(OCLData->OpenCL){ret=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);OCLData->OpenCL=false;};}
#define OCL_GET_(text,item,module) if(OCLData->OpenCL){item=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);OCLData->OpenCL=false;};}

#define OCL_PROGRAM(prog,name,params) source_size = SourceFromFile(name,source_str); OCL_GET_("clCreateProgramWithSource",prog,clCreateProgramWithSource (context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret)); \
 OCL_RUN_("clBuildProgram",clBuildProgram(prog,1,&device,params,NULL,NULL)); if(ret!=CL_SUCCESS) { OCLData->OpenCL = true; OCL_RUN_("clGetProgramBuildInfo",clGetProgramBuildInfo \
 (prog,device,CL_PROGRAM_BUILD_LOG,30000,(void*)log_data,&logsize)); if(ret==CL_SUCCESS) { char line[255]; sprintf(line,"Имя=%s;Параметры=%s",name,params);Log->AddLine(line);Log->AddBigText("Журнал компиляции",log_data);}; OCLData->OpenCL = false; }


#endif // SUREDATA_H
