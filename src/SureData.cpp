#include "SureData.h"
#include "SureTruck.h"

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
        RandomSeed = new cl_int[SURE_R_RNDSIZE];
        ModelsInfo = new SureModelData[SURE_R_MAXTEX];
        TexturesInfo = new SureTextureData[SURE_R_MAXTEX];

        srand(time(NULL));
        clock_gettime(CLOCK_MONOTONIC,&ShowTemplateTime);

        Log = new SureLog("engine");
        EnviromentMap = GetTexture("enviroment");

        GenModel("cube",0);
        GenModel("tetr",1);
        GenModel("pand",2);

    LoadControls("default");
    LoadOptions("current");
    //LoadState("initial");
    //Scene_box();
    Scene_floor();
    //Scene_Polygon();
    //Scene_mirrors();
    //Scene_ManyTetrs();
    //Scene_ManySpheres();
    Scene_vechicle();
    //Scene_golem();
    // Шаблонный объект

    // вызываем загрузку наследников:
    onLoading();

    SetTemplate_DarkCube(10.0f);
    Loading = false;
}

SureData::~SureData()
{
    SaveOptions("current");
    Log->AddLine("Выход из SURE...");
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
    delete RandomSeed;
    delete ModelsInfo;
    delete TexturesInfo;
    delete Log;
}

void SureData::WriteObjectToFile(FILE *f,SureObject *o,int i_exclude)
{
    //int external_id;
    //int ParentID = -1;
    int ObjectDataVersion = 2;

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
    fwrite(&i_exclude,sizeof(int),1,f);
    fwrite(&o->p1,sizeof(my_double3),1,f);
    fwrite(&o->p2,sizeof(my_double3),1,f);
    fwrite(&o->p3,sizeof(my_double3),1,f);
    fwrite(&o->p4,sizeof(my_double3),1,f);
    fwrite(&o->p1o,sizeof(my_double3),1,f);
    fwrite(&o->p2o,sizeof(my_double3),1,f);
    fwrite(&o->p3o,sizeof(my_double3),1,f);
    fwrite(&o->p4o,sizeof(my_double3),1,f);
    //fwrite(&o->v1,sizeof(my_double3),1,f);
    //fwrite(&o->v2,sizeof(my_double3),1,f);
    //fwrite(&o->v3,sizeof(my_double3),1,f);
    //fwrite(&o->v4,sizeof(my_double3),1,f);
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
        WriteObjectToFile(f,&objects[i],i_exclude);
    };// дочерние объекты
}

void SureData::SureFread(void* i_ptr,size_t i_s1,size_t i_s2,FILE* f)
{
    if(fread(i_ptr,i_s1,i_s2,f)!=i_s2){
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
        my_double3 t;
        SureFread(&t,sizeof(my_double3),1,f);
        SureFread(&t,sizeof(my_double3),1,f);
        SureFread(&t,sizeof(my_double3),1,f);
        SureFread(&t,sizeof(my_double3),1,f);
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
    }else if(ObjectDataVersion==2){
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
        SureFread(&objects[m_objects].CollideExcludeID,sizeof(int),1,f);
        SureFread(&objects[m_objects].p1,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p2,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p3,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p4,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p1o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p2o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p3o,sizeof(my_double3),1,f);
        SureFread(&objects[m_objects].p4o,sizeof(my_double3),1,f);
        //SureFread(&objects[m_objects].v1,sizeof(my_double3),1,f);
        //SureFread(&objects[m_objects].v2,sizeof(my_double3),1,f);
        //SureFread(&objects[m_objects].v3,sizeof(my_double3),1,f);
        //SureFread(&objects[m_objects].v4,sizeof(my_double3),1,f);
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
    sprintf(fname,"./objects/%s.SureObject",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    WriteObjectToFile(f,o,-1);

    fclose(f);
    sprintf(LogLine,"Сохранен объект %s",name);
    Log->AddLine(LogLine);
}

int SureData::LoadObjectFromFile(const char *name)
{
    char fname[100];
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

void SureData::SaveScene(const char *name,SureObject** i_objects, int i_ObjCounter)
{
    char fname[100];
    sprintf(fname,"./objects/%s.SureScene",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int SceneDataVersion = 2;
    fwrite(&SceneDataVersion,sizeof(int),1,f);

    fwrite(&i_ObjCounter,sizeof(int),1,f);
    for(int o = 0;o<i_ObjCounter;++o){
        int CollideExcludeID = -1;
        if(i_objects[0]->CollideExcludeID>=0)
        for(int oi=0;oi<i_ObjCounter;++oi)
        if(i_objects[oi]->external_id==i_objects[o]->CollideExcludeID)
            CollideExcludeID = oi;
        WriteObjectToFile(f,i_objects[o],CollideExcludeID);
    };

    // посчитать линки с этими объектами
    int lv_links = 0;
    for(int il=0;il<m_links;++il){
        bool found1 = false;
        bool found2 = false;
        for(int oi=0;oi<i_ObjCounter;++oi){
            if(i_objects[oi]->external_id==links[il].Object1)
                found1 = true;
            if(i_objects[oi]->external_id==links[il].Object2)
                found2 = true;
        };
        if(found1&&found2)
            lv_links++;
    };

    // записать количество линков
    fwrite(&lv_links,sizeof(int),1,f);
    for(int il=0;il<m_links;++il){
        int LocalObject1 = -1;
        int LocalObject2 = -1;
        for(int oi=0;oi<i_ObjCounter;++oi){
            if(i_objects[oi]->external_id==links[il].Object1)
                LocalObject1 = oi;
            if(i_objects[oi]->external_id==links[il].Object2)
                LocalObject2 = oi;
        };
        if(LocalObject1<0)continue;
        if(LocalObject2<0)continue;
        fwrite(&LocalObject1,sizeof(int),1,f);
        fwrite(&LocalObject2,sizeof(int),1,f);
        fwrite(&links[il].l,sizeof(double),1,f);
        fwrite(&links[il].k,sizeof(double),1,f);
        fwrite(&links[il].Object1Dmper,sizeof(my_double3),1,f);
        fwrite(&links[il].Object1Dmper,sizeof(my_double3),1,f);
        fwrite(&links[il].type,sizeof(SureLinkType),1,f);
    };

    fclose(f);
    sprintf(LogLine,"Успешно сохранеа сцена %s",name);
    Log->AddLine(LogLine);
}

void SureData::LoadScene(const char *name)
{
    char fname[100];
    sprintf(fname,"./objects/%s.SureScene",name);
    FILE *f = NULL;
    f = fopen(fname,"rb");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int SceneDataVersion = 0;
    SureFread(&SceneDataVersion,sizeof(int),1,f);

    if(SceneDataVersion==1){
        sprintf(LogLine,"Загружается сцена %s...",name);
        Log->AddLine(LogLine);

        int lv_objects = 0;

        SureFread(&lv_objects,sizeof(int),1,f);
        for(int o = 0;o<lv_objects;++o)
            ReadObjectFromFile(f,-1);

        reset = true;
        fclose(f);
        sprintf(LogLine,"Успешно загружена сцена %s",name);
        Log->AddLine(LogLine);
    }else if(SceneDataVersion==2){
        sprintf(LogLine,"Загружается сцена %s...",name);
        Log->AddLine(LogLine);

        int lv_objects = 0;
        SureFread(&lv_objects,sizeof(int),1,f);
        int* LocalObjectIDs;
        LocalObjectIDs = new int[lv_objects];

        for(int o = 0;o<lv_objects;++o){
            LocalObjectIDs[o] = ReadObjectFromFile(f,-1);
        };
        for(int o = 0;o<lv_objects;++o){
            SureObject* lv_o = ObjByID(LocalObjectIDs[o]);
            if(lv_o->CollideExcludeID>=0)
                lv_o->CollideExcludeID = LocalObjectIDs[lv_o->CollideExcludeID];
        };

        int lv_links = 0;
        SureFread(&lv_links,sizeof(int),1,f);
        for(int o = 0;o<lv_links;++o){
            int LocalID1 = -1;
            SureFread(&LocalID1,sizeof(int),1,f);
            if(LocalID1>=0) links[m_links].Object1 = LocalObjectIDs[LocalID1];
            int LocalID2 = -1;
            SureFread(&LocalID2,sizeof(int),1,f);
            if(LocalID2>=0) links[m_links].Object2 = LocalObjectIDs[LocalID2];
            SureFread(&links[m_links].l,sizeof(double),1,f);
            SureFread(&links[m_links].k,sizeof(double),1,f);
            SureFread(&links[m_links].Object1Dmper,sizeof(my_double3),1,f);
            SureFread(&links[m_links].Object1Dmper,sizeof(my_double3),1,f);
            SureFread(&links[m_links].type,sizeof(SureLinkType),1,f);
            m_links++;
        };

        delete[] LocalObjectIDs;
        reset = true;
        fclose(f);
        sprintf(LogLine,"Успешно загружена сцена %s",name);
        Log->AddLine(LogLine);
    }else{ // if(StateDataVersion==2)...
        char LogString[100];
        sprintf(LogString,"Версия файла сцены №%i не поддерживается",SceneDataVersion);
        Log->AddLine(LogString);
    };
}

void SureData::SaveOptions(const char *name)
{
    char fname[100];
    sprintf(fname,"./%s.SureOptions",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int OptionsDataVersion = 2;
    fwrite(&OptionsDataVersion,sizeof(int),1,f);

    fwrite(&r_type,sizeof(int),1,f);
    fwrite(&GPUData.r_maxiters,sizeof(cl_uchar),1,f);
    fwrite(&GPUData.r_rechecks,sizeof(cl_uchar),1,f);
    fwrite(&GPUData.r_backlight,sizeof(cl_float),1,f);
    fwrite(&GPUData.CameraInfo.xy_h,sizeof(float),1,f);
    fwrite(&GPUData.CameraInfo.subp_rnd,sizeof(bool),1,f);
    fwrite(&GPUData.SAA,sizeof(int),1,f);
    fwrite(&ImageScale,sizeof(int),1,f);
    fwrite(&DrawDebugFPS,sizeof(bool),1,f);
    fwrite(&DrawDebugTraces,sizeof(bool),1,f);
    fwrite(&DrawDebugSelectedObject,sizeof(bool),1,f);
    fwrite(&DrawDebugAllObjects,sizeof(bool),1,f);
    fwrite(&DrawDebugMode,sizeof(bool),1,f);
    fwrite(&DrawDebugPhysicsInfo,sizeof(bool),1,f);
    fwrite(&DrawDebugCursorInfo,sizeof(bool),1,f);
    fwrite(&DrawDebugPhysicsTetrs,sizeof(bool),1,f);
    fwrite(&DrawDebugLinks,sizeof(bool),1,f);

    fclose(f);
    sprintf(LogLine,"Настройки %s сохранены",name);
    Log->AddLine(LogLine);
}

void SureData::LoadOptions(const char *name)
{
    char fname[100];
    sprintf(fname,"./%s.SureOptions",name);
    FILE *f = NULL;
    f = fopen(fname,"rb");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int OptionsDataVersion = 0;
    SureFread(&OptionsDataVersion,sizeof(int),1,f);

    if(OptionsDataVersion==1){
        sprintf(LogLine,"Загружаем настройки %s...",name);
        Log->AddLine(LogLine);

        SureFread(&r_type,sizeof(int),1,f);
        SureFread(&GPUData.r_maxiters,sizeof(cl_uchar),1,f);
        SureFread(&GPUData.r_rechecks,sizeof(cl_uchar),1,f);
        SureFread(&GPUData.r_backlight,sizeof(cl_float),1,f);
        SureFread(&GPUData.CameraInfo.xy_h,sizeof(float),1,f);
        SureFread(&GPUData.CameraInfo.subp_rnd,sizeof(bool),1,f);
        SureFread(&GPUData.SAA,sizeof(int),1,f);
        SureFread(&ImageScale,sizeof(int),1,f);
        SureFread(&DrawDebugFPS,sizeof(bool),1,f);
        SureFread(&DrawDebugTraces,sizeof(bool),1,f);
        SureFread(&DrawDebugSelectedObject,sizeof(bool),1,f);
        SureFread(&DrawDebugAllObjects,sizeof(bool),1,f);
        SureFread(&DrawDebugMode,sizeof(bool),1,f);
        SureFread(&DrawDebugPhysicsInfo,sizeof(bool),1,f);
        SureFread(&DrawDebugCursorInfo,sizeof(bool),1,f);
        SureFread(&DrawDebugPhysicsTetrs,sizeof(bool),1,f);

        reset = true;
        fclose(f);
        sprintf(LogLine,"Настройки %s загружены",name);
        Log->AddLine(LogLine);
    }else if(OptionsDataVersion==2){
        sprintf(LogLine,"Загружаем настройки %s...",name);
        Log->AddLine(LogLine);

        SureFread(&r_type,sizeof(int),1,f);
        SureFread(&GPUData.r_maxiters,sizeof(cl_uchar),1,f);
        SureFread(&GPUData.r_rechecks,sizeof(cl_uchar),1,f);
        SureFread(&GPUData.r_backlight,sizeof(cl_float),1,f);
        SureFread(&GPUData.CameraInfo.xy_h,sizeof(float),1,f);
        SureFread(&GPUData.CameraInfo.subp_rnd,sizeof(bool),1,f);
        SureFread(&GPUData.SAA,sizeof(int),1,f);
        SureFread(&ImageScale,sizeof(int),1,f);
        SureFread(&DrawDebugFPS,sizeof(bool),1,f);
        SureFread(&DrawDebugTraces,sizeof(bool),1,f);
        SureFread(&DrawDebugSelectedObject,sizeof(bool),1,f);
        SureFread(&DrawDebugAllObjects,sizeof(bool),1,f);
        SureFread(&DrawDebugMode,sizeof(bool),1,f);
        SureFread(&DrawDebugPhysicsInfo,sizeof(bool),1,f);
        SureFread(&DrawDebugCursorInfo,sizeof(bool),1,f);
        SureFread(&DrawDebugPhysicsTetrs,sizeof(bool),1,f);
        SureFread(&DrawDebugLinks,sizeof(bool),1,f);

        reset = true;
        fclose(f);
        sprintf(LogLine,"Настройки %s загружены",name);
        Log->AddLine(LogLine);
    }else{ // if(OptionsDataVersion==2)...
        char LogString[100];
        sprintf(LogString,"Версия файла настроек №%i не поддерживается",OptionsDataVersion);
        Log->AddLine(LogString);
    };
}

void SureData::Stop()
{
    ((QWidget*)widget)->close();
}

void SureData::SelectObjectByScreenTrace(int x,int y)
{
    cl_uchar* Textures = TexturesData;
    SureGPUData* GPUData = &this->GPUData;
    SelectedObject = -1;
    #define __SELECT_OBJECT
    #define SURE_RLEVEL 10
    #include <trace_common.c>
    #undef __SELECT_OBJECT
}

void SureData::AddTraceLog(int x,int y,bool OnlyVisible)
{
    cl_uchar* Textures = TexturesData;
    SureGPUData* GPUData = &this->GPUData;
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

void SureData::SinglePointTrace(int x,int y)
{
    cl_uchar* Textures = TexturesData;
    SureGPUData* GPUData = &this->GPUData;
    #define SURE_RLEVEL 100
    #include <trace_common.c>
}

void SureData::ActionThrowTemplateObject(){
    TemplateObject.ox = GPUData.CameraInfo.cam_vec;
    TemplateObject.oz = GPUData.CameraInfo.cam_upvec;
    TemplateObject.oy = cross(GPUData.CameraInfo.cam_vec,GPUData.CameraInfo.cam_upvec);
    __VTYPE3 X = GPUData.CameraInfo.cam_x;
    uint o = CreateObjectFromTemplate(&X);
    ObjByID(o)->push(ObjByID(o)->X,GPUData.CameraInfo.cam_vec,3.0);
}

void SureData::onLoading(){}
void SureData::onPhysics(){
    Test_Truck->PushByCaterlillars();
    Test_Truck->SetCaterpillars();
    Test_Truck->PushCaterpillars(-0.3f);
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
