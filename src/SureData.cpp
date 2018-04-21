#include "SureData.h"

SureData::SureData()
{

}

void SureData::LoadEngine(){
        VrtxCLImg = new cl_float[CLSIZE_VERTEX]; // 256*256
        MeshCLImg = new cl_int[CLSIZE_VERTEX]; //256*256
        TexturesData = new cl_uchar[SURE_R_MAXTEX * SURE_R_TEXRES * SURE_R_TEXRES * 4];
        UVMap = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        Normals = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        rgbmatrix = new cl_float[SURE_MAXRES_X*SURE_MAXRES_Y*3];
        Drawables = new SureDrawable[SURE_DR_MAX];
        Randomf = new cl_float[SURE_R_RNDSIZE];
        ModelsInfo = new SureModelData[SURE_R_MAXTEX];
        TexturesInfo = new SureTextureData[SURE_R_MAXTEX];

        srand(time(NULL));
        clock_gettime(CLOCK_MONOTONIC,&ShowTemplateTime);

        Log = new SureLog("engine");

        LoadTexture("parket");
        LoadTexture("earth_adv");
        LoadTexture("colstones");
        LoadTexture("golem");
        LoadTexture("ghost_dark");
        LoadTexture("golem_adv");
        LoadTexture("earth");
        LoadTexture("test_adv");

        LoadModel("test");

        LoadModel("golem");
        LoadModel("teapot");
        LoadModel("monkey");
        LoadModel("ghost_dark");
        GenModel("cube",0);
        GenModel("tetr",1);
        GenModel("pand",2);

    LoadControls("default");
    //Scene_box();
    //Scene_floor();
    Scene_Polygon();
    //Scene_mirrors();
    //Scene_ManyTetrs();
    //Scene_ManySpheres();
    //Scene_tetrs();
    //Scene_golem();
    // Шаблонный объект
    SetTemplate_DarkCube(10.0f);
    Loading = false;
}

SureData::~SureData()
{
    if(MenuWindowsCounter>0)
    for(int i = 0;i<MenuWindowsCounter;++i)
        delete MenuWindows[i];
    MenuWindowsCounter = 0;
    delete VrtxCLImg;
    delete MeshCLImg;
    delete TexturesData;
    delete UVMap;
    delete Normals;
    delete rgbmatrix;
    delete Drawables;
    delete Randomf;
    delete ModelsInfo;
    delete TexturesInfo;
    delete Log;
}

void SureData::WriteObjectToFile(FILE *f,SureObject *o)
{
    //int external_id;
    //int ParentID = -1;
    int ObjectDataVersion = 1;

    fwrite(&ObjectDataVersion,sizeof(int),1,f);

    fwrite(&o->X,sizeof(my_double3),1,f);
    fwrite(&o->ox,sizeof(my_double3),1,f);
    fwrite(&o->oy,sizeof(my_double3),1,f);
    fwrite(&o->oz,sizeof(my_double3),1,f);
    fwrite(&o->X_byparent,sizeof(my_double3),1,f);
    fwrite(&o->ox_byparent,sizeof(my_double3),1,f);
    fwrite(&o->oy_byparent,sizeof(my_double3),1,f);

    fwrite(&o->lx,sizeof(double),1,f);
    fwrite(&o->ly,sizeof(double),1,f);
    fwrite(&o->lz,sizeof(double),1,f);
    fwrite(&o->lp,sizeof(double),1,f);
    fwrite(&o->rig,sizeof(double),1,f);
    fwrite(&o->mass,sizeof(double),1,f);
    fwrite(&o->type,sizeof(int),1,f);
    fwrite(&o->movable,sizeof(bool),1,f);
    fwrite(&o->collidable,sizeof(bool),1,f);
    fwrite(&o->p1,sizeof(my_double3),1,f);
    fwrite(&o->p2,sizeof(my_double3),1,f);
    fwrite(&o->p3,sizeof(my_double3),1,f);
    fwrite(&o->p4,sizeof(my_double3),1,f);
    fwrite(&o->p1o,sizeof(my_double3),1,f);
    fwrite(&o->p2o,sizeof(my_double3),1,f);
    fwrite(&o->p3o,sizeof(my_double3),1,f);
    fwrite(&o->p4o,sizeof(my_double3),1,f);
    fwrite(&o->v1,sizeof(my_double3),1,f);
    fwrite(&o->v2,sizeof(my_double3),1,f);
    fwrite(&o->v3,sizeof(my_double3),1,f);
    fwrite(&o->v4,sizeof(my_double3),1,f);
    fwrite(o->ModelName_collider,sizeof(char),20,f);
    fwrite(o->ModelName_drawable,sizeof(char),20,f);
    //int ModelID_collider;
    //int ModelID_drawable;

    //SureDrawable drawable:
    fwrite(&o->drawable.X,sizeof(cl_float3),1,f);
    fwrite(&o->drawable.ox,sizeof(cl_float3),1,f);
    fwrite(&o->drawable.oy,sizeof(cl_float3),1,f);
    fwrite(&o->drawable.oz,sizeof(cl_float3),1,f);
    fwrite(&o->drawable.lx,sizeof(cl_float),1,f);
    fwrite(&o->drawable.ly,sizeof(cl_float),1,f);
    fwrite(&o->drawable.lz,sizeof(cl_float),1,f);
    fwrite(&o->drawable.type,sizeof(cl_int),1,f);
    fwrite(&o->drawable.radiance,sizeof(cl_int),1,f);
    fwrite(&o->drawable.transp,sizeof(cl_float),1,f);
    fwrite(&o->drawable.transp_i,sizeof(cl_float),1,f);
    fwrite(&o->drawable.refr,sizeof(cl_float),1,f);
    fwrite(&o->drawable.dist_type,sizeof(cl_int),1,f);
    fwrite(&o->drawable.dist_sigma,sizeof(cl_float),1,f);
    fwrite(&o->drawable.dist_m,sizeof(cl_float),1,f);
    fwrite(&o->drawable.rgb,sizeof(cl_uchar3),1,f);
    fwrite(&o->drawable.sided,sizeof(cl_bool),1,f);
    fwrite(&o->drawable.mesh_start,sizeof(cl_uint),1,f);
    fwrite(&o->drawable.mesh_count,sizeof(cl_uint),1,f);

    char EmptyMapName[21] = "--------------------";
    if(o->drawable.map_id>=0&&o->drawable.map_id<(int)cur_textures){
        fwrite(&TexturesInfo[o->drawable.map_id].name,sizeof(char),20,f);
    }else{
        fwrite(&EmptyMapName,sizeof(char),20,f);
    };
    if(o->drawable.advmap_id>=0&&o->drawable.advmap_id<(int)cur_textures){
        fwrite(&TexturesInfo[o->drawable.advmap_id].name,sizeof(char),20,f);
    }else{
        fwrite(&EmptyMapName,sizeof(char),20,f);
    };
    //drawable.map_id
    //drawable.advmap_id

    fwrite(&o->ParticlesCounter,sizeof(int),1,f);
    fwrite(&o->ParticlesInitiated,sizeof(bool),1,f);
    fwrite(&o->ParticlesLimit,sizeof(int),1,f);
    fwrite(&o->PSGravityMass,sizeof(double),1,f);
    fwrite(&o->PSInnerCollisions,sizeof(bool),1,f);
    fwrite(&o->PSSparks,sizeof(bool),1,f);

    //SureParticle *Particles;
    for(int i = 0;i<o->ParticlesCounter;++i){
     fwrite(&o->Particles[i].OX,sizeof(my_double3),1,f);
     fwrite(&o->Particles[i].X,sizeof(my_double3),1,f);
     fwrite(&o->Particles[i].Size,sizeof(double),1,f);
    };
    //int DrawableGPUID = -1;

    int Childs=0;
    for(int i=0;i<m_objects;++i)
    if((objects[i].ParentID==o->external_id)&&(o->external_id>=0))
        ++Childs;

    fwrite(&Childs,sizeof(int),1,f);

    for(int i=0;i<m_objects;++i)
    if((objects[i].ParentID==o->external_id)&&(o->external_id>=0)){
        WriteObjectToFile(f,&objects[i]);
    };// дочерние объекты
}

void SureData::SureFread(void* i_ptr,size_t i_s1,size_t i_s2,FILE* f)
{
    if(fread(i_ptr,i_s1,i_s2,f)!=i_s2){
        char LogLine[100];
        sprintf(LogLine,"Ошибка чтения файла");
        Log->AddLine(LogLine);
    };
}

int SureData::ReadObjectFromFile(FILE *f,int i_parent)
{
    int ObjectDataVersion = 0;
    int ParentID = -1;

    SureFread(&ObjectDataVersion,sizeof(int),1,f);

    if(ObjectDataVersion==1){
        char ReadingString[20];
        SureFread(&objects[m_objects].X,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].ox,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].oy,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].oz,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].X_byparent,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].ox_byparent,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].oy_byparent,sizeof(my_double3),1,f);

        SureFread(&objects[m_objects].lx,sizeof(double),1,f);
        SureFread(&objects[m_objects].ly,sizeof(double),1,f);
        SureFread(&objects[m_objects].lz,sizeof(double),1,f);
        SureFread(&objects[m_objects].lp,sizeof(double),1,f);
        SureFread(&objects[m_objects].rig,sizeof(double),1,f);
        SureFread(&objects[m_objects].mass,sizeof(double),1,f);
        SureFread(&objects[m_objects].type,sizeof(int),1,f);
        SureFread(&objects[m_objects].movable,sizeof(bool),1,f);
        SureFread(&objects[m_objects].collidable,sizeof(bool),1,f);
        SureFread(&objects[m_objects].p1,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p2,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p3,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p4,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p1o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p2o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p3o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p4o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].v1,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].v2,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].v3,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].v4,sizeof(my_double3),1,f);
        SureFread(objects[m_objects].ModelName_collider,sizeof(char),20,f);
        SureFread(objects[m_objects].ModelName_drawable,sizeof(char),20,f);
        objects[m_objects].ModelID_collider = GetModel(objects[m_objects].ModelName_collider);
        objects[m_objects].ModelID_drawable = GetModel(objects[m_objects].ModelName_drawable);

        //SureDrawable drawable:
        SureFread(&objects[m_objects].drawable.X,sizeof(cl_float3),1,f);
        SureFread(&objects[m_objects].drawable.ox,sizeof(cl_float3),1,f);
        SureFread(&objects[m_objects].drawable.oy,sizeof(cl_float3),1,f);
        SureFread(&objects[m_objects].drawable.oz,sizeof(cl_float3),1,f);
        SureFread(&objects[m_objects].drawable.lx,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.ly,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.lz,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.type,sizeof(cl_int),1,f);
        SureFread(&objects[m_objects].drawable.radiance,sizeof(cl_int),1,f);
        SureFread(&objects[m_objects].drawable.transp,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.transp_i,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.refr,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.dist_type,sizeof(cl_int),1,f);
        SureFread(&objects[m_objects].drawable.dist_sigma,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.dist_m,sizeof(cl_float),1,f);
        SureFread(&objects[m_objects].drawable.rgb,sizeof(cl_uchar3),1,f);
        SureFread(&objects[m_objects].drawable.sided,sizeof(cl_bool),1,f);
        SureFread(&objects[m_objects].drawable.mesh_start,sizeof(cl_uint),1,f);
        SureFread(&objects[m_objects].drawable.mesh_count,sizeof(cl_uint),1,f);

        SureFread(&ReadingString,sizeof(char),20,f);
        objects[m_objects].drawable.map_id = GetTexture(ReadingString);
        SureFread(&ReadingString,sizeof(char),20,f);
        objects[m_objects].drawable.advmap_id = GetTexture(ReadingString);

        SureFread(&objects[m_objects].ParticlesCounter,sizeof(int),1,f);
        int ParticlesCount = objects[m_objects].ParticlesCounter;
        SureFread(&objects[m_objects].ParticlesInitiated,sizeof(bool),1,f);
        SureFread(&objects[m_objects].ParticlesLimit,sizeof(int),1,f);
        SureFread(&objects[m_objects].PSGravityMass,sizeof(double),1,f);
        SureFread(&objects[m_objects].PSInnerCollisions,sizeof(bool),1,f);
        SureFread(&objects[m_objects].PSSparks,sizeof(bool),1,f);

        if(objects[m_objects].ParticlesInitiated&&objects[m_objects].ParticlesCounter>0&&objects[m_objects].ParticlesLimit>0){
            objects[m_objects].ParticlesInitiated = false;
            SetParticlesNumber(&objects[m_objects],objects[m_objects].ParticlesLimit);
        }else{
            objects[m_objects].MemFree();
        };
        for(int i = 0;i<ParticlesCount;++i){
             SureFread(&objects[m_objects].Particles[i].OX,sizeof(my_double3),1,f);
             SureFread(&objects[m_objects].Particles[i].X,sizeof(my_double3),1,f);
             SureFread(&objects[m_objects].Particles[i].Size,sizeof(double),1,f);
        };
        objects[m_objects].ParticlesCounter = ParticlesCount;

        objects[m_objects].DrawableGPUID = -1;
        objects[m_objects].external_id = m_ObjExternID++;
        ParentID = objects[m_objects].external_id;
        objects[m_objects].ParentID = i_parent;

        m_objects++;

        int Childs = 0;
        SureFread(&Childs,sizeof(int),1,f);
        for(int i=0;i<Childs;++i){
            ReadObjectFromFile(f,ParentID);
        };// дочерние объекты
    }else{ // if(ObjectDataVersion==2)...
        char LogString[100];
        sprintf(LogString,"Версия объекта №%i не поддерживается",ObjectDataVersion);
        Log->AddLine(LogString);
    };
    return ParentID;
}

void SureData::SaveObjectToFile(SureObject *o,const char *name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./objects/%s.SureObject",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    WriteObjectToFile(f,o);

    fclose(f);
    sprintf(LogLine,"Сохранен объект %s",name);
    Log->AddLine(LogLine);
}

int SureData::LoadObjectFromFile(const char *name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./objects/%s.SureObject",name);
    FILE *f = NULL;
    f = fopen(fname,"rb");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return -1;
    };
    int ObjID = -1;
    ObjID = ReadObjectFromFile(f,-1);
    reset = true;
    fclose(f);
    sprintf(LogLine,"Успешно загружен объект %s",name);
    Log->AddLine(LogLine);
    return ObjID;
}

void SureData::SaveState(const char *name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./%s.state",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int StateDataVersion = 1;
    fwrite(&StateDataVersion,sizeof(int),1,f);

    fwrite(&cam_dx,sizeof(my_double3),1,f);
    fwrite(&cam_dw,sizeof(my_double3),1,f);
    fwrite(&r_iters,sizeof(cl_uchar),1,f);
    fwrite(&r_rechecks,sizeof(cl_uchar),1,f);
    fwrite(&r_backlight,sizeof(cl_float),1,f);
    fwrite(&r_drawdebug,sizeof(int),1,f);
    fwrite(&r_type,sizeof(int),1,f);
    fwrite(&paused,sizeof(bool),1,f);

    fwrite(&CameraInfo.cam_x,sizeof(cl_float3),1,f);
    fwrite(&CameraInfo.cam_vec,sizeof(cl_float3),1,f);
    fwrite(&CameraInfo.cam_upvec,sizeof(cl_float3),1,f);
    fwrite(&CameraInfo.xy_h,sizeof(cl_float),1,f);
    fwrite(&CameraInfo.m_amx,sizeof(cl_uint),1,f);
    fwrite(&CameraInfo.m_amy,sizeof(cl_uint),1,f);
    fwrite(&CameraInfo.subp_rnd,sizeof(cl_bool),1,f);

    int mainObjs = 0;
    for(int o = 0;o<m_objects;++o)
    if(objects[o].ParentID<0)
        ++mainObjs;

    fwrite(&mainObjs,sizeof(int),1,f);
    for(int o = 0;o<m_objects;++o)
    if(objects[o].ParentID<0)
        WriteObjectToFile(f,&objects[o]);

    fclose(f);
    sprintf(LogLine,"Успешно сохранено состояние %s",name);
    Log->AddLine(LogLine);
}

void SureData::LoadState(const char *name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./%s.state",name);
    FILE *f = NULL;
    f = fopen(fname,"rb");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int StateDataVersion = 0;
    SureFread(&StateDataVersion,sizeof(int),1,f);

    if(StateDataVersion==1){
        sprintf(LogLine,"Загружается состояние %s...",name);
        Log->AddLine(LogLine);
            SureFread(&cam_dx,sizeof(my_double3),1,f);
            SureFread(&cam_dw,sizeof(my_double3),1,f);
            SureFread(&r_iters,sizeof(cl_uchar),1,f);
            SureFread(&r_rechecks,sizeof(cl_uchar),1,f);
            SureFread(&r_backlight,sizeof(cl_float),1,f);
            SureFread(&r_drawdebug,sizeof(int),1,f);
            SureFread(&r_type,sizeof(int),1,f);
            SureFread(&paused,sizeof(bool),1,f);

            SureFread(&CameraInfo.cam_x,sizeof(cl_float3),1,f);
            SureFread(&CameraInfo.cam_vec,sizeof(cl_float3),1,f);
            SureFread(&CameraInfo.cam_upvec,sizeof(cl_float3),1,f);
            SureFread(&CameraInfo.xy_h,sizeof(cl_float),1,f);
            SureFread(&CameraInfo.m_amx,sizeof(cl_uint),1,f);
            SureFread(&CameraInfo.m_amy,sizeof(cl_uint),1,f);
            SureFread(&CameraInfo.subp_rnd,sizeof(cl_bool),1,f);

        int lv_objects = 0;
        SureFread(&lv_objects,sizeof(int),1,f);
        for(int o = 0;o<m_objects;++o)
            objects[o].MemFree();
        m_objects = 0;
        for(int o = 0;o<lv_objects;++o)
            ReadObjectFromFile(f,-1);

        reset = true;
        fclose(f);
        sprintf(LogLine,"Успешно загружено состояние %s",name);
        Log->AddLine(LogLine);
    }else{ // if(StateDataVersion==2)...
        char LogString[100];
        sprintf(LogString,"Версия состояния №%i не поддерживается",StateDataVersion);
        Log->AddLine(LogString);
    };
}

void SureData::SelectObjectByScreenTrace(int x,int y,SureGPUData *GPUData,float *Randomf)
{
    cl_uchar* Textures = TexturesData;
    SelectedObject = -1;
    #define __SELECT_OBJECT
    #define SURE_RLEVEL 10
    #include <trace_common.c>
    #undef __SELECT_OBJECT
}

void SureData::AddTraceLog(int x,int y,SureGPUData *GPUData,float *Randomf,bool OnlyVisible)
{
    cl_uchar* Textures = TexturesData;
    int Iters = OnlyVisible?50000:1;

    for(int iter=0;iter<Iters;++iter){
        #define __LOGGING
        #define SURE_RLEVEL 100
        #include <trace_common.c>
        #undef __LOGGING
        if(TraceLogs[TraceLogsCount].ItemsCount>0)
        if((TraceLogs[TraceLogsCount].Items[TraceLogs[TraceLogsCount].ItemsCount-1].Color.x
            + TraceLogs[TraceLogsCount].Items[TraceLogs[TraceLogsCount].ItemsCount-1].Color.y
            + TraceLogs[TraceLogsCount].Items[TraceLogs[TraceLogsCount].ItemsCount-1].Color.z)
            > 50) // Если точка достаточно освещен -- выходим из цикла
            iter = Iters;
    };
    if(++TraceLogsCount>45)
        TraceLogsCount=0;
}

void SureData::SinglePointTrace(int x,int y,SureGPUData *GPUData,float *Randomf,float *rgbmatrix)
{
    cl_uchar* Textures = TexturesData;
    #define SURE_RLEVEL 100
    #include <trace_common.c>
}

#include <SureData/Controls.cpp>
#include <SureData/Generate.cpp>
#include <SureData/GJK.cpp>
#include <SureData/Load.cpp>
#include <SureData/Menu.cpp>
#include <SureData/Object.cpp>
#include <SureData/Scene.cpp>

#include <func_common.c>
#include <func_CPU.cpp>
