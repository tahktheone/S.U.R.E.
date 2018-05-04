#ifndef SUREDATA_H
#define SUREDATA_H

class SureObject;
class SureMenuWindow;
class SureData;
class SureWidget;
class SureTruck;

#include <SureData/Defines.h>
#include <SureData/Struct.h>
#include <SureData/Menu.h>
#include <SureData/Object.h>
#include <SureData/Collisions.h>

class SureData{
    public:
        SureData();
        virtual ~SureData();

        bool Loading = true;
        void LoadEngine();
        virtual void onLoading();
        virtual void onPhysics();
        void Stop();

        SureLog *Log;
        char LogLine[255];

        SureGPUData GPUData;
        SureOCLData OCLData;

        int CurrentWidth = 0;
        int CurrentHeight = 0;
        my_double3 cam_dx = my_double3{0,0,0};
        my_double3 cam_dw = my_double3{0,0,0};
        int ImageScale = 4;
        int r_type = SURE_RT_D;
        int EnviromentMap = -1;

        bool reset = true; // сброс кадра
        bool paused = true; // Пауза физики
        bool mousemove = false;

        SureDrawable *Drawables;
        cl_int *RandomSeed;
        cl_float* rgbmatrix;
        cl_float *VrtxCLImg; // [SURE_R_MAXVERTEX*4] -- для OpenCL image2d_t
        cl_int *MeshCLImg;  // [SURE_R_MAXMESH*4] -- для OpenCL image2d_t
        cl_uchar *TexturesData; //[SURE_R_MAXTEX * SURE_R_TEXRES * SURE_R_TEXRES * 4];
        cl_float *UVMap;  //[SURE_R_MAXTEXMAP * 4]; {u1, v1 .. }
        cl_float *Normals; // [SURE_R_MAXTEXMAP * 4]; {n1x, n1y, n1z ...}
        SureModelData* ModelsInfo;
        SureTextureData* TexturesInfo;
        SureWidget* widget;
        SureObject objects[SURE_OBJ_MAX];
        SureLink links[SURE_LIN_MAX];
        int m_links = 0;
        int m_objects = 0;
        cl_uint cur_vertexes = 0;
        cl_uint cur_meshes = 0;
        cl_uint cur_models = 0;
        cl_uint cur_textures = 0;

        void SaveScene(const char *,SureObject** i_objects, int i_ObjCounter);
        void LoadScene(const char *);
        void SaveOptions(const char *);
        void LoadOptions(const char *);
        void WriteObjectToFile(FILE *f,SureObject *o,int i_exclude);
        int ReadObjectFromFile(FILE *f,int i_parent);
        void SaveObjectToFile(SureObject *o,const char *fname);
        int LoadObjectFromFile(const char *fname);

        void SureFread(void*,size_t,size_t,FILE*);
        int LoadTexture(const char*);
        int LoadModel(const char*);
        int GetTexture(const char*);
        int GenTexture(const char*,int type);
        int GetModel(const char*);
        int GenModel(const char*,int type);

        void SetParticlesNumber(SureObject *i_ps,int i_number);
        void GenerateParticleCloud(SureObject *i_ps,int i_number);
        void GenerateParticle(SureObject *i_ps,my_double3 i_X,double i_size,my_double3 i_vec);

        SureObject TemplateObject; // шаблонный объект
        void SelectObjectByScreenTrace(int x,int y);
        void AddTraceLog(int x,int y,bool OnlyVisible);
        void SinglePointTrace(int x,int y);
        uint CreateObjectFromTemplate(__VTYPE3* i_X); // создать объект из TemplateObject
        void DeleteObject(uint);
        void DeleteLink(uint);
        void DeleteObjectByID(int);
        uint ObjectNumberByID(int);
        SureObject *ObjByID(int); // Объект по внешнему айдишнику

        uint m_ObjExternID = 1;

        // меню
        SureMenuWindow* MenuWindows[10];
        int MenuWindowsCounter = 0;

        // Управление
        SureControl Controls[300];
        int ControlsCounter = 0;
        void ExecuteAction(SureControllerAction* i_action,SureControllerInput* i_input);
        virtual void onAction(SureControllerAction* i_action,SureControllerInput* i_input,bool* i_continue);
        void HandleInput(SureControllerInput* i_input);
        void AssignControl(SureControllerInput* i_input,SureControllerAction* i_action);
        void SaveControls(const char* name);
        void LoadControls(const char* name);
        void GetControllerActionName(int i_action,wchar_t *e_name);
        void ActionThrowTemplateObject();

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
        float rendertime = 0;
        float posttime = 0;
        struct timespec framestart; // время начала текущего фрейма
        struct timespec frametime_time; // время отсечения
        float frametime_f = 0;

        bool DrawDebugFPS = false;
        bool DrawDebugTraces = false;
        bool DrawDebugSelectedObject = false;
        bool DrawDebugAllObjects = false;
        bool DrawDebugMode = false;
        bool DrawDebugPhysicsInfo = false;
        bool DrawDebugCursorInfo = false;
        bool DrawDebugPhysicsTetrs = false;
        bool DrawDebugLinks = false;

        SureTraceLog TraceLogs[50];
        uint TraceLogsCount = 0;
        int SelectedObject = -1;

        SureTruck* Test_Truck;

    protected:
    private:
        void SetDefaultRotationBasis(int);

        int AddVertex(double x, double y, double z);
        int AddVertex(my_double3 X);
        int AddMesh(int v1, int v2, int v3);

        void Scene_box(); // коробка со светящимся потолком
        void Scene_floor(); // Пол и круглая лампа
        void Scene_mirrors(); // Пол и зеркала
        void Scene_vechicle();
        void Scene_golem();
        void Scene_ManyTetrs();
        void Scene_ManySpheres();
        void Scene_Polygon();
        //void Scene_metaball(double x, double y, double z, double sz,int nt);
        //void Scene_cube(double x, double y, double z, double sz,int nt,int mt);
        //void Scene_tetra(double x, double y, double z, double sz,int nt,int mt,bool movable);
};

// Работа с OpenCL из CPU-части:
void ocl_errtext(cl_int);
int SourceFromFile(const char*,char*);

// Почему дублируются макросы:
// OCL_RUN и OCL_GET обращаются к OCLData напрямую, а OCL_RUN_и OCL_GET_ по указателю.
// Потому, что вызываются из разных потоков.
#define OCL_RUN(text,module) if(EngineData->OCLData.OpenCL){ret=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);EngineData->OCLData.OpenCL=false;};}
#define OCL_GET(text,item,module) if(EngineData->OCLData.OpenCL){item=module;if(ret!=CL_SUCCESS){Log->AddOCLError(ret,text);EngineData->OCLData.OpenCL=false;};}

#define OCL_PROGRAM(prog,name,params) source_size = SourceFromFile(name,source_str); OCL_GET("clCreateProgramWithSource",prog,clCreateProgramWithSource (context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret)); \
 OCL_RUN("clBuildProgram",clBuildProgram(prog,1,&device,params,NULL,NULL)); if(ret!=CL_SUCCESS) { EngineData->OCLData.OpenCL = true; OCL_RUN("clGetProgramBuildInfo",clGetProgramBuildInfo \
 (prog,device,CL_PROGRAM_BUILD_LOG,30000,(void*)log_data,&logsize)); if(ret==CL_SUCCESS) { char line[255]; sprintf(line,"Имя=%s;Параметры=%s",name,params);Log->AddLine(line);Log->AddBigText("Журнал компиляции",log_data);}; EngineData->OCLData.OpenCL = false; }


#endif // SUREDATA_H
