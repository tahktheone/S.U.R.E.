#include "SureData.h"

void SureData::SureClear()
{
    delete TexturesData;
    delete UVMap;
    delete Normals;
    delete VrtxCLImg;
    delete MeshCLImg;
}

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
        //LoadModel("sword");
        LoadModel("teapot");
        LoadModel("monkey");
        LoadModel("ghost_dark");
        GenModel("cube",0);
        GenModel("tetr",1);
        GenModel("pand",2);



  //LoadState("initial");

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
}

SureData::~SureData()
{
    delete Log;
}

void SureData::SaveState(const char *name)
{
    char fname[100];
    char LogLine[200];
    #ifdef _WIN32
        sprintf(fname,"./%s.win.state",name);
    #else
        sprintf(fname,"./%s.nix.state",name);
    #endif // __WIN32
    FILE *f = NULL;
    f = fopen(fname,"w");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };
    fprintf(f,"r_drawdebug=%i\n",r_drawdebug);
    fprintf(f,"r_type=%i\n",r_type);
    fprintf(f,"r_iters=%i\n",r_iters);
    fprintf(f,"r_rechecks=%i\n",r_rechecks);
    fprintf(f,"r_backlight=%.3f\n",r_backlight);
    fprintf(f,"paused=%i\n",paused);

    fprintf(f,"CameraInfo.cam_x=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_x.s[0],CameraInfo.cam_x.s[1],CameraInfo.cam_x.s[2]);
    fprintf(f,"CameraInfo.cam_vec=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_vec.s[0],CameraInfo.cam_vec.s[1],CameraInfo.cam_vec.s[2]);
    fprintf(f,"CameraInfo.cam_upvec=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_upvec.s[0],CameraInfo.cam_upvec.s[1],CameraInfo.cam_upvec.s[2]);
    fprintf(f,"CameraInfo.xy_h=%.4f\n",CameraInfo.xy_h);
    fprintf(f,"CameraInfo.subp_rnd=%i\n",CameraInfo.subp_rnd);

    fprintf(f,"Objects=%i\n",m_objects);
    for(int o = 0;o<m_objects;++o)
    {
        #define SAVE_FLOAT_VECTOR(A,B) \
        fprintf(f,"%s (%.4f;%.4f;%.4f)\n",A,objects[o].B.x,objects[o].B.y,objects[o].B.z);

        #define SAVE_FLOAT_VECTOR_DRAWABLE(A,B) \
        fprintf(f,"%s (%.4f;%.4f;%.4f)\n",A,objects[o].drawable.B.s[0],objects[o].drawable.B.s[1],objects[o].drawable.B.s[2]);

        fprintf(f,"ID=%i\n",objects[o].external_id);
        SAVE_FLOAT_VECTOR("X",X);
        SAVE_FLOAT_VECTOR("ox",ox);
        SAVE_FLOAT_VECTOR("oy",oy);
        SAVE_FLOAT_VECTOR("oz",oz);
        fprintf(f,"Lxyz=(%.4f;%.4f;%.4f)\n",objects[o].lx,objects[o].ly,objects[o].lz);
        fprintf(f,"pl=%.4f\n",objects[o].lp);
        fprintf(f,"rig=%.4f\n",objects[o].rig);
        fprintf(f,"type=%i\n",objects[o].type);
        fprintf(f,"movable=%i\n",objects[o].movable);
        fprintf(f,"collidable=%i\n",objects[o].collidable);

        SAVE_FLOAT_VECTOR("p1",p1);
        SAVE_FLOAT_VECTOR("p2",p2);
        SAVE_FLOAT_VECTOR("p3",p3);
        SAVE_FLOAT_VECTOR("p4",p4);
        SAVE_FLOAT_VECTOR("p1o",p1o);
        SAVE_FLOAT_VECTOR("p2o",p2o);
        SAVE_FLOAT_VECTOR("p3o",p3o);
        SAVE_FLOAT_VECTOR("p4o",p4o);
        SAVE_FLOAT_VECTOR("v1",v1);
        SAVE_FLOAT_VECTOR("v2",v2);
        SAVE_FLOAT_VECTOR("v3",v3);
        SAVE_FLOAT_VECTOR("v4",v4);

        fprintf(f,"ModelName_collider=%s\n",objects[o].ModelName_collider);
        fprintf(f,"ModelName_drawable=%s\n",objects[o].ModelName_drawable);
        /* cl_uint ModelID_collider; cl_uint ModelID_drawable; */

        SAVE_FLOAT_VECTOR_DRAWABLE("dr_X",X);
        SAVE_FLOAT_VECTOR_DRAWABLE("dr_ox",ox);
        SAVE_FLOAT_VECTOR_DRAWABLE("dr_oy",oy);
        SAVE_FLOAT_VECTOR_DRAWABLE("dr_oz",oz);
        fprintf(f,"dr_Lxyz=(%.4f;%.4f;%.4f)\n",objects[o].drawable.lx,objects[o].drawable.ly,objects[o].drawable.lz);
        fprintf(f,"dr_type=%i\n",objects[o].drawable.type);
        fprintf(f,"dr_radiance=%i\n",objects[o].drawable.radiance);
        fprintf(f,"dr_ttr=(%.4f;%.4f;%.4f)\n",objects[o].drawable.transp,objects[o].drawable.transp_i,objects[o].drawable.refr);
        fprintf(f,"dr_dist=(%i;%.4f,%.4f)\n",objects[o].drawable.dist_type,objects[o].drawable.dist_sigma,objects[o].drawable.dist_m);
        fprintf(f,"dr_rgb=(%i;%i;%i)\n",objects[o].drawable.rgb.s[0],objects[o].drawable.rgb.s[1],objects[o].drawable.rgb.s[2]);
        fprintf(f,"dr_sided=%i\n",objects[o].drawable.sided);

        int lv_map = objects[o].drawable.map_id;

        if(lv_map>=0&&lv_map<(int)cur_textures){
            fprintf(f,"dr_map=%s \n",TexturesInfo[lv_map].name);
        }else{
            fprintf(f,"dr_map=NO \n");
        };

        lv_map = objects[o].drawable.advmap_id;

        if(lv_map>=0&&lv_map<(int)cur_textures){
            fprintf(f,"dr_advmap=%s \n",TexturesInfo[lv_map].name);
        }else{
            fprintf(f,"dr_advmap=NO \n");
        };
    };

    fclose(f);
    sprintf(LogLine,"Успешно сохранено состояние %s",name);
    Log->AddLine(LogLine);
}

void SureData::LoadState(const char *name)
{
    char fname[100];
    char LogLine[200];
    #ifdef _WIN32
        sprintf(fname,"./%s.win.state",name);
    #else
        sprintf(fname,"./%s.nix.state",name);
    #endif // __WIN32
    FILE *f = NULL;
    f = fopen(fname,"r");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };
    sprintf(LogLine,"Загружается состояние %s...",name);
    Log->AddLine(LogLine);
    if(fscanf(f,"r_drawdebug=%d\n",&r_drawdebug)<1) Log->AddLine("Ошибка чтения r_drawdebug");
    if(fscanf(f,"r_type=%d\n",&r_type)<1) Log->AddLine("Ошибка чтения r_type");
    if(fscanf(f,"r_iters=%hhu\n",&r_iters)<1) Log->AddLine("Ошибка чтения 1");
    if(fscanf(f,"r_rechecks=%hhu\n",&r_rechecks)<1) Log->AddLine("Ошибка чтения 2");
    if(fscanf(f,"r_backlight=%f\n",&r_backlight)<1) Log->AddLine("Ошибка чтения 3");
    if(fscanf(f,"paused=%hhu\n",(uchar *)&paused)<1) Log->AddLine("Ошибка чтения 5");

    if(fscanf(f,"CameraInfo.cam_x=(%f;%f;%f)\n",&CameraInfo.cam_x.s[0],&CameraInfo.cam_x.s[1],&CameraInfo.cam_x.s[2])<3) Log->AddLine("Ошибка чтения 6");
    if(fscanf(f,"CameraInfo.cam_vec=(%f;%f;%f)\n",&CameraInfo.cam_vec.s[0],&CameraInfo.cam_vec.s[1],&CameraInfo.cam_vec.s[2])<3) Log->AddLine("Ошибка чтения 7");
    if(fscanf(f,"CameraInfo.cam_upvec=(%f;%f;%f)\n",&CameraInfo.cam_upvec.s[0],&CameraInfo.cam_upvec.s[1],&CameraInfo.cam_upvec.s[2])<3) Log->AddLine("Ошибка чтения 8");
    if(fscanf(f,"CameraInfo.xy_h=%f\n",&CameraInfo.xy_h)<1) Log->AddLine("Ошибка чтения 9");
    if(fscanf(f,"CameraInfo.subp_rnd=%u\n",&CameraInfo.subp_rnd)<1) Log->AddLine("Ошибка чтения 10");

    int lv_objects = 0;
    if(fscanf(f,"Objects=%i\n",&lv_objects)<1) Log->AddLine("Ошибка чтения - количесто объектов");
    m_objects = 0;
    for(int o = 0;o<lv_objects;++o)
    {

        char VariableName[20];

        #define READ_FLOAT_VECTOR(B) \
        if(fscanf(f,"%s (%lf;%lf;%lf)\n",VariableName,&objects[m_objects].B.x,&objects[m_objects].B.y,&objects[m_objects].B.z)<4) Log->AddLine("Ошибка чтения файла состояния");

        #define READ_FLOAT_VECTOR_DRAWABLE(B) \
        if(fscanf(f,"%s (%f;%f;%f)\n",VariableName,&objects[m_objects].drawable.B.s[0],&objects[m_objects].drawable.B.s[1],&objects[m_objects].drawable.B.s[2])<4) Log->AddLine("Ошибка чтения файла состояния");


        #define READ_DOUBLE_VECTOR_DRAWABLE(B) \
        if(fscanf(f,"%s (%lf;%lf;%lf)\n",VariableName,&objects[m_objects].drawable.B.s[0],&objects[m_objects].drawable.B.s[1],&objects[m_objects].drawable.B.s[2])<4) Log->AddLine("Ошибка чтения файла состояния");

        if(fscanf(f,"ID=%u\n",&objects[m_objects].external_id)<1)Log->AddLine("Ошибка чтения");

        READ_FLOAT_VECTOR(X);
        READ_FLOAT_VECTOR(ox);
        READ_FLOAT_VECTOR(oy);
        READ_FLOAT_VECTOR(oz);

        if(fscanf(f,"Lxyz=(%lf;%lf;%lf)\n",&objects[m_objects].lx,&objects[m_objects].ly,&objects[m_objects].lz)<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"pl=%lf\n",&objects[m_objects].lp)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"rig=%lf\n",&objects[m_objects].rig)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"type=%i\n",&objects[m_objects].type)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"movable=%hhu\n",(uchar *)&objects[m_objects].movable)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"collidable=%hhu\n",(uchar *)&objects[m_objects].collidable)<1)Log->AddLine("Ошибка чтения");

        READ_FLOAT_VECTOR(p1);
        READ_FLOAT_VECTOR(p2);
        READ_FLOAT_VECTOR(p3);
        READ_FLOAT_VECTOR(p4);
        READ_FLOAT_VECTOR(p1o);
        READ_FLOAT_VECTOR(p2o);
        READ_FLOAT_VECTOR(p3o);
        READ_FLOAT_VECTOR(p4o);
        READ_FLOAT_VECTOR(v1);
        READ_FLOAT_VECTOR(v2);
        READ_FLOAT_VECTOR(v3);
        READ_FLOAT_VECTOR(v4);

        if(fscanf(f,"ModelName_collider=%s\n",objects[m_objects].ModelName_collider)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"ModelName_drawable=%s\n",objects[m_objects].ModelName_drawable)<1)Log->AddLine("Ошибка чтения");
        objects[m_objects].ModelID_collider=GetModel(objects[m_objects].ModelName_collider);
        objects[m_objects].ModelID_drawable=GetModel(objects[m_objects].ModelName_drawable);

        READ_FLOAT_VECTOR_DRAWABLE(X);
        READ_FLOAT_VECTOR_DRAWABLE(ox);
        READ_FLOAT_VECTOR_DRAWABLE(oy);
        READ_FLOAT_VECTOR_DRAWABLE(oz);

        if(fscanf(f,"dr_Lxyz=(%f;%f;%f)\n",&objects[m_objects].drawable.lx,&objects[m_objects].drawable.ly,&objects[m_objects].drawable.lz)<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_type=%i\n",&objects[m_objects].drawable.type)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_radiance=%i\n",&objects[m_objects].drawable.radiance)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_ttr=(%f;%f;%f)\n",&objects[m_objects].drawable.transp,&objects[m_objects].drawable.transp_i,&objects[m_objects].drawable.refr)<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_dist=(%i;%f,%f)\n",&objects[m_objects].drawable.dist_type,&objects[m_objects].drawable.dist_sigma,&objects[m_objects].drawable.dist_m)<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_rgb=(%hhu;%hhu;%hhu)\n",&objects[m_objects].drawable.rgb.s[0],&objects[m_objects].drawable.rgb.s[1],&objects[m_objects].drawable.rgb.s[2])<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_sided=%hhu\n",(uchar *)&objects[m_objects].drawable.sided)<1)Log->AddLine("Ошибка чтения");

        char lv_map[20] = "none";
        if(fscanf(f,"dr_map=%s \n",lv_map)<1)Log->AddLine("Ошибка чтения");
        objects[m_objects].drawable.map_id = GetTexture(lv_map);

        char lv_advmap[20] = "none";
        if(fscanf(f,"dr_advmap=%s \n",lv_advmap)<1)Log->AddLine("Ошибка чтения");
        objects[m_objects].drawable.advmap_id = GetTexture(lv_advmap);

        m_objects++;
    };

    reset = true;

    fclose(f);
    sprintf(LogLine,"Успешно загружено состояние %s",name);
    Log->AddLine(LogLine);
}

SureGJK::SureGJK(SureData *i_engine)
{
    EngineData = i_engine;
}

SureGJK::SureGJK()
{

}

void SureGJK::SetEngine(SureData *i_engine)
{
    EngineData = i_engine;
}

SureGJK::~SureGJK()
{

}

void SureGJK::Clear()
{
    free(M);
}

void SureGJK::ClearCover()
{
    free(C);
    free(C_N);
    free(incover);
}

void SureGJK::SetupMinkowski(SureObject *o1,SureObject *o2)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    __VTYPE3 p1,p2; // точки из объектов, для получения разности
    __VTYPE3 gp1,gp2; // точки в глобальных координатах
    Collision.Object1 = o1;
    Collision.Object2 = o2;

    // 1. Составляем разность минковского.
    uint l1_limit = EngineData->ModelsInfo[o1->ModelID_collider].vertex_count;
    uint l1_start = EngineData->ModelsInfo[o1->ModelID_collider].vertex_start;
    uint l2_limit = EngineData->ModelsInfo[o2->ModelID_collider].vertex_count;
    uint l2_start = EngineData->ModelsInfo[o2->ModelID_collider].vertex_start;

    M = (my_double3 *)malloc(sizeof(my_double3)*l1_limit*l2_limit);

    my_double3 M_min = {SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE};
    my_double3 M_max = {-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE};

    // для каждой точки
    for(uint i1 = 0;i1<l1_limit;++i1){
        uint cv1 = l1_start + i1;
        __GET_VERTEX(p1,cv1);
        p1.x = p1.x * o1->lx;
        p1.y = p1.y * o1->ly;
        p1.z = p1.z * o1->lz;
        gp1 = o1->ox*p1.x + o1->oy*p1.y+o1->oz*p1.z + o1->X;
        for(uint i2 = 0;i2<l2_limit;++i2)
        { // каждая точка с каждой точкой
            uint cv2 = l2_start + i2;
            __GET_VERTEX(p2,cv2);
            p2.x = p2.x * o2->lx;
            p2.y = p2.y * o2->ly;
            p2.z = p2.z * o2->lz;
            gp2 = o2->ox*p2.x + o2->oy*p2.y+o2->oz*p2.z + o2->X;
            M[mc]=gp2-gp1;
            if(M[mc].x<M_min.x)M_min.x=M[mc].x;
            if(M[mc].y<M_min.y)M_min.y=M[mc].y;
            if(M[mc].z<M_min.z)M_min.z=M[mc].z;
            if(M[mc].x>M_max.x)M_max.x=M[mc].x;
            if(M[mc].y>M_max.y)M_max.y=M[mc].y;
            if(M[mc].z>M_max.z)M_max.z=M[mc].z;
            ++mc;
        };// каждая точка с каждой точкой
    };

    exit_no_collision = false;
    exit_collision = false;
    if(M_min.x>0||M_min.y>0||M_min.z>0)
        exit_no_collision = true;
    if(M_max.x<0||M_max.y<0||M_max.z<0)
        exit_no_collision = true;
}

void SureGJK::InitiateLoop()
{
    // [Тэтраэдр T. 4 точки. 2 раза -- для текущего шага и для следующего]
    TI[0] = 0;
    TI[1] = 1;
    TI[2] = 2;
    TI[3] = 3;
    //разворачиваем тэтраэдр наружу нормалями.
    if(dot(cross(M[TI[1]]-M[TI[0]],M[TI[2]]-M[TI[0]]),M[TI[3]]-M[TI[0]])>0) //вершина 4 снаружи T0 T1 T2 ?
    { // T2 <=> T1 (меняем местами)
        uint TTI = TI[2];
        TI[2] = TI[1];
        TI[1] = TTI;
    };
    iter = 0; // подсчет итерраций - гарантия от зацикливаний
}

bool SureGJK::LoopExit()
{
    return (exit_no_collision||exit_collision);
}

void SureGJK::ExitLoop_Collision()
{
    exit_collision = true;
    collision_found = true;
}

void SureGJK::ExitLoop_NoCollision()
{
    exit_no_collision = true;
}

bool SureGJK::CoverExpanded()
{
    return cover_expanded;
}

void SureGJK::SetCoverExpanded()
{
    cover_expanded = true;
}

void SureGJK::InitiateCoverLoop()
{
    C = (uint *)malloc(sizeof(uint)*mc*2);
    C_N = (uint *)malloc(sizeof(uint)*mc*2);
    incover = (bool *)malloc(sizeof(bool)*mc);

    for(uint incb=0;incb<mc;++incb)
        incover[incb]=false;

    C[cc*3+0]=TI[0]; incover[TI[0]]=true;
    C[cc*3+1]=TI[1]; incover[TI[1]]=true;
    C[cc*3+2]=TI[2]; incover[TI[2]]=true;
    ++cc;

    C[cc*3+0]=TI[0]; incover[TI[0]]=true;
    C[cc*3+1]=TI[3]; incover[TI[3]]=true;
    C[cc*3+2]=TI[1]; incover[TI[1]]=true;
    ++cc;

    C[cc*3+0]=TI[1]; incover[TI[1]]=true;
    C[cc*3+1]=TI[3]; incover[TI[3]]=true;
    C[cc*3+2]=TI[2]; incover[TI[2]]=true;
    ++cc;

    C[cc*3+0]=TI[2]; incover[TI[2]]=true;
    C[cc*3+1]=TI[3]; incover[TI[3]]=true;
    C[cc*3+2]=TI[0]; incover[TI[0]]=true;
    ++cc;

    iter = 0;
}

void SureGJK::CheckCoverIterration()
{
    if(iter>16){
        SetCoverExpanded();
        EngineData->Log->AddLine("i!");
    };
    ++iter;
}

bool SureGJK::TIContains00()
{
    //Проверяем, содержит ли тэтраэдр 0,0.
    //заодно ищем грань ближайшую к 0,0
    L1 = dot(__NORMALIZE(cross(M[TI[1]]-M[TI[0]],M[TI[2]]-M[TI[0]])),-M[TI[0]]); // грань 0 1 2
    L2 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[0]],M[TI[1]]-M[TI[0]])),-M[TI[0]]); // грань 0 3 1
    L3 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[1]],M[TI[2]]-M[TI[1]])),-M[TI[1]]); // грань 1 3 2
    L4 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[2]],M[TI[0]]-M[TI[2]])),-M[TI[2]]); // грань 2 3 0

    return (L1<0.0&&L2<0.0&&L3<0.0&&L4<0.0);
}

double SureGJK::GetMaximumDistanceTo00()
{
    #define SET_TN(A,B,C) TN[0]=T[A];TN[1]=T[B];TN[2]=T[C];
    #define SET_TNI(A,B,C) TNI[0]=TI[A];TNI[1]=TI[B];TNI[2]=TI[C];
    // ищем грань, для которой [0,0] снаружи
    double LM = 0;
    if(L1>LM){ SET_TNI(0,1,2); LM=L1 + SURE_R_DELTA;};
    if(L2>LM){ SET_TNI(0,3,1); LM=L2 + SURE_R_DELTA;};
    if(L3>LM){ SET_TNI(1,3,2); LM=L3 + SURE_R_DELTA;};
    if(L4>LM){ SET_TNI(2,3,0); LM=L4 + SURE_R_DELTA;};
    return LM;
}

bool SureGJK::FindNextTNI()
{
    __VTYPE3 v = __NORMALIZE(cross(M[TNI[1]]-M[TNI[0]],M[TNI[2]]-M[TNI[0]])); // нормаль основания нового тэтраэдра
    __VTYPE md = dot(v,M[TNI[0]])+SURE_R_DELTA;
    __VTYPE ld;
    bool f = false;
    for(uint i = 0;i<mc;++i){// для каждой точки M[i] -- есть точка в направлении v?
        ld = dot(v,M[i]);
        if(ld>md){
            md = ld;
            TNI[3] = i;
            f = true;
        };
    };
    return f;
}

bool SureGJK::FindFarestMinkowskiByVector(my_double3 Vector,uint *e_result)
{
    // ищем дальнюю точку в M в направлении v
    __VTYPE md = dot(Vector,M[C[CollisionFace*3+0]])+SURE_R_DELTA;
    __VTYPE ld;
    bool f = false;
    uint fndi = 0;
    for(uint li = 0;li<mc;++li){// для каждой точки M[i]
        if(!incover[li]){ // если точка не в оболочке уже
            ld = dot(Vector,M[li]);
            if(ld>md){
                md = ld;
                fndi = li;
                f = true;
            };
        }; // если точка не в оболочке уже
    }; // для каждой точки M[i]
    *e_result = fndi;
    return f;
}

bool SureGJK::IsCoverFaceVisibleByPoint(uint f,uint point)
{
    return (dot(cross(M[C[f*3+1]]-M[C[f*3+0]],
                      M[C[f*3+2]]-M[C[f*3+0]]),
                M[point]-M[C[f*3+0]])
            > SURE_R_DELTA);
}

void SureGJK::CheckAndAddFaceToNewCover(uint p1, uint p2, uint p3)
{
// Логика check_cover:
// для всех точек в M[] проверяем, лежат ли они снаружи
// проверяемой грани. Если все внутри --
// добавляем грань в COVER
    bool out = false;
    for(uint cci = 0;cci<mc;++cci)
        if(incover[cci])
            if(dot(cross(M[p2]-M[p1],M[p3]-M[p1]),M[cci]-M[p1])>SURE_R_DELTA)
                out = true;
    if(!out){
        C_N[cc_n*3+0] = p1;
        C_N[cc_n*3+1] = p2;
        C_N[cc_n*3+2] = p3;
        ++cc_n;
    };
}

void SureGJK::AddFaceToNewCover(uint p1, uint p2, uint p3)
{
    C_N[cc_n*3+0] = p1;
    C_N[cc_n*3+1] = p2;
    C_N[cc_n*3+2] = p3;
    ++cc_n;
}

void SureGJK::CheckAndAddFaceToNewCover(uint face,uint fndi)
{
    CheckAndAddFaceToNewCover(C[face*3+0],C[face*3+1],fndi);
    CheckAndAddFaceToNewCover(C[face*3+1],C[face*3+2],fndi);
    CheckAndAddFaceToNewCover(C[face*3+2],C[face*3+0],fndi);
}

void SureGJK::AddFaceToNewCover(uint face)
{
    AddFaceToNewCover(C[face*3+0],C[face*3+1],C[face*3+2]);
}

void SureGJK::ExpandCover(uint fndi)
{
    SetIncover(fndi);
    for(uint aci = 0;aci<cc;++aci)
    { // для каждой грани cover
        // грань видимая?
        if(IsCoverFaceVisibleByPoint(aci,fndi))
        { // грань видимая
            CheckAndAddFaceToNewCover(aci,fndi);
        }else{ // грань невидимая - оставляем в cover
            AddFaceToNewCover(aci);
        }; // грань видимая?
    };// для каждой грани cover
}

void SureGJK::SetCollisionByCover()
{
    //Ищем грань ближайшую к 0 0.
    Collision.CollisionLength = SURE_R_MAXDISTANCE;
    CollisionFace = 0;
    for(uint ci=0;ci<cc;++ci)
    { // для каждой грани cover
    // (помним что для всех граней 0,0 внутри)
        __VTYPE L = fabs(dot(GetCoverFaceNormal(ci),-M[C[ci*3+0]]));
        // расстояние до 0,0
        if(L<Collision.CollisionLength)
        {
            Collision.CollisionLength = L;
            CollisionFace = ci;
        }; // (L<LM)
    }; // для каждой грани cover
    Collision.CollisionVector = GetCoverFaceNormal(CollisionFace);
}

void SureGJK::TNI_to_TI()
{
    TI[0]=TNI[0];
    TI[1]=TNI[2];
    TI[2]=TNI[1];
    TI[3]=TNI[3];
}

void SureGJK::LoopNextStep()
{
    double LM = GetMaximumDistanceTo00();
    if(LM>0){
        if(FindNextTNI()){   // нужно новый тэтраэдр
            TNI_to_TI();
        }else{
            ExitLoop_NoCollision();
        };
        iter++;
        if(iter>20){
            ExitLoop_NoCollision();
            EngineData->Log->AddLine("f!");
        };
    }else{ // расстояние до 0,0 больше преыдущего
        ExitLoop_NoCollision();
    };
}

bool SureGJK::CollisionFound()
{
    return collision_found;
}

uint SureGJK::GetFarestVertexByObj(SureObject *o,my_double3 i_vector)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    uint l_start = EngineData->ModelsInfo[o->ModelID_collider].vertex_start;
    uint l_limit = l_start + EngineData->ModelsInfo[o->ModelID_collider].vertex_count;
    uint result = 0;
    double LM = -SURE_R_MAXDISTANCE;
    for(uint i = l_start;i<l_limit;++i){
        my_double3 LocalVertex;
        __GET_VERTEX(LocalVertex,i);
        double LC = dot(LocalVertex,i_vector);
        if(LC>LM){
            LM = LC;
            result = i;
        };
    };
    return result;
}

my_double3 SureGJK::GetCollisionPointByObject(SureObject *o,my_double3 i_gvector,double *dist)
{
    cl_int* MeshCLImg = EngineData->MeshCLImg;
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов

    my_double3 i_vector;
    i_vector.x = dot(i_gvector,o->ox)/o->lx;
    i_vector.y = dot(i_gvector,o->oy)/o->ly;
    i_vector.z = dot(i_gvector,o->oz)/o->lz;
    i_vector = __NORMALIZE(i_vector);

    uint FarestVertexIndex = GetFarestVertexByObj(o,i_vector);
    my_double3 FarestVertex;
    __GET_VERTEX(FarestVertex,FarestVertexIndex);

    my_double3 LocalCollisionFace[3];
    uint VertexesInFace = 1;
    LocalCollisionFace[0] = FarestVertex;

    my_double3 CumulatedVertex = FarestVertex;
    double FoundVertexes = 1.0;

    my_double3 Minimum = FarestVertex;
    my_double3 Maximum = FarestVertex;


    uint lv_start = EngineData->ModelsInfo[o->ModelID_collider].vertex_start;
    uint lv_limit = lv_start + EngineData->ModelsInfo[o->ModelID_collider].vertex_count;
    uint l_start = EngineData->ModelsInfo[o->ModelID_collider].mesh_start;
    uint l_limit = l_start + EngineData->ModelsInfo[o->ModelID_collider].mesh_count;

    for(uint vi = lv_start;vi<lv_limit;++vi){ // Для каждого vertex'а
        bool CommonMeshExist = false;
        for(uint mi = l_start;mi<l_limit;++mi){
            __SURE_VINT4 mesh;
            __GET_MESH(mesh,mi);
            uint m1 = mesh.x;
            uint m2 = mesh.y;
            uint m3 = mesh.z;
            if((m1==FarestVertexIndex||m2==FarestVertexIndex||m3==FarestVertexIndex)
               &&(m1==vi||m2==vi||m3==vi)){
                CommonMeshExist = true;
            };
        };
        if(CommonMeshExist){ // Есть общая грань с дальним vertex'ом
            my_double3 LocalVertex;
            __GET_VERTEX(LocalVertex,vi);
            // строим грань
            my_double3 Face = __NORMALIZE(FarestVertex - LocalVertex);
            // cross(грань,i_vector) > 0.99 ?
            if(__LENGTH(cross(Face,i_vector)) > 0.999999){
                // добавляем vertex к расчету средней точки
                CumulatedVertex = CumulatedVertex + LocalVertex;
                FoundVertexes = FoundVertexes + 1.0;
                // добавляем в min, max
                if(LocalVertex.x<Minimum.x)
                    Minimum.x = LocalVertex.x;
                if(LocalVertex.y<Minimum.y)
                    Minimum.y = LocalVertex.y;
                if(LocalVertex.z<Minimum.z)
                    Minimum.z = LocalVertex.z;

                if(LocalVertex.x>Maximum.x)
                    Maximum.x = LocalVertex.x;
                if(LocalVertex.y>Maximum.y)
                    Maximum.y = LocalVertex.y;
                if(LocalVertex.z>Maximum.z)
                    Maximum.z = LocalVertex.z;

                if(VertexesInFace<3){
                    LocalCollisionFace[VertexesInFace] = LocalVertex;
                    ++VertexesInFace;
                };//if(VertexesInFace<3)
            }; //if(__LENGTH(cross(Face,i_vector)) > 0.99)
        };// if(CommonMeshExist;
    }; // Для каждого vertex'а

    // средняя точка = CumulatedVertex/FoundVertexes
    my_double3 result = CumulatedVertex * ( 1.0 / FoundVertexes );

    if(VertexesInFace==3){
        my_double3 CorrectionNormal = __NORMALIZE(cross(LocalCollisionFace[1]-LocalCollisionFace[0],LocalCollisionFace[2]-LocalCollisionFace[0]));
        my_double3 GlobalCorrectionNormal = __NORMALIZE(CorrectionNormal.x * o->ox * o->lx +
                                                        CorrectionNormal.y * o->oy * o->ly +
                                                        CorrectionNormal.z * o->oz * o->lz);
        double Projection = dot(GlobalCorrectionNormal,Collision.CollisionVector);
        if(Projection>0){
            Collision.CollisionVector = GlobalCorrectionNormal;
        }else{
            Collision.CollisionVector = -GlobalCorrectionNormal;
        };
        Collision.CollisionLength = Collision.CollisionLength * fabs(Projection);
    };

    // dist_v = max - min.
    // dist_v в глобальниые координаты
    my_double3 DistVector = Maximum - Minimum;
    if(DistVector.x>0||DistVector.y>0||DistVector.z>0){
        my_double3 GlobalDistVector = DistVector.x * o->ox * o->lx +
                                      DistVector.y * o->oy * o->ly +
                                      DistVector.z * o->oz * o->lz;
        *dist = __LENGTH(GlobalDistVector);
    }else{
        *dist = 0;
    };

    result = o->X +  result.x * o->ox * o->lx +
         result.y * o->oy * o->ly +
         result.z * o->oz * o->lz;
    return result;
}

void SureGJK::SwitchCover()
{
    for(uint icn = 0;icn<cc_n;++icn){
        C[icn*3+0] = C_N[icn*3+0];
        C[icn*3+1] = C_N[icn*3+1];
        C[icn*3+2] = C_N[icn*3+2];
    };
    cc = cc_n;
}

void SureGJK::SetIncover(uint i)
{
    incover[i] = true;
}

void SureGJK::ClearNewCover()
{
    cc_n = 0;
}

my_double3 SureGJK::GetCoverFaceNormal(uint face)
{
    return __NORMALIZE(cross(M[C[face*3+1]]-M[C[face*3+0]],M[C[face*3+2]]-M[C[face*3+0]]));
}

void SureGJK::ExpandCoverLoop()
{
    InitiateCoverLoop();
    while(!CoverExpanded()){
        ClearNewCover();
        SetCollisionByCover();
        uint fndi = 0;
        if(FindFarestMinkowskiByVector(Collision.CollisionVector,&fndi)){
            // cover нужно расширить добавив точку M[fndi]
            ExpandCover(fndi);
            SwitchCover();
            CheckCoverIterration();
        }else{
            SetCoverExpanded();
        }; // поиск расширения для cover
    }; // while (!cover_expanded
    SetCollisionPoint();
    ClearCover();
}

void SureGJK::SetCollisionPoint()
{
    double dist1;
    double dist2;
    my_double3 CollisionPoint1 = GetCollisionPointByObject(Collision.Object1,-Collision.CollisionVector,&dist1);
    my_double3 CollisionPoint2 = GetCollisionPointByObject(Collision.Object2,Collision.CollisionVector,&dist2);
    if(dist1<dist2){
        Collision.CollisionPoint = CollisionPoint1;
    }else{
        Collision.CollisionPoint = CollisionPoint2;
    };
}

#include <func_common.c>
#include <func_CPU.c>
