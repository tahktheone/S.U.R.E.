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

  //Scene_cube(0,0,20,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(20,-30,20,19,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(-20,20,15,14,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_YZ);
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

void SureGJK::SetupMinkowski(SureObject *o1,SureObject *o2)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    __VTYPE3 p1,p2; // точки из объектов, для получения разности
    __VTYPE3 gp1,gp2; // точки в глобальных координатах

    // 1. Составляем разность минковского.
    uint l1_limit = EngineData->ModelsInfo[o1->ModelID_collider].vertex_count;
    uint l1_start = EngineData->ModelsInfo[o1->ModelID_collider].vertex_start;
    uint l2_limit = EngineData->ModelsInfo[o2->ModelID_collider].vertex_count;
    uint l2_start = EngineData->ModelsInfo[o2->ModelID_collider].vertex_start;

    // для каждой точки
    for(uint i1 = 0;i1<l1_limit;++i1)
    {
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
            ++mc;
        };// каждая точка с каждой точкой
    };
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
    exit_no_collision = false;
    exit_collision = false;
    iter = 0; // подсчет итерраций - гарантия от зацикливаний
}

uint SureGJK::GetMinkowski(my_double3 *e_M)
{
    for(uint i = 0;i<mc;++i){
        e_M[i] = M[i];
    };
    return mc;
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

void SureGJK::Set_TI(uint *i_TI)
{
    TI[0] = i_TI[0];
    TI[1] = i_TI[1];
    TI[2] = i_TI[2];
    TI[3] = i_TI[3];
}

void SureGJK::Set_TNI(uint *i_TNI)
{
    TNI[0] = i_TNI[0];
    TNI[1] = i_TNI[1];
    TNI[2] = i_TNI[2];
    TNI[3] = i_TNI[3];
}

void SureGJK::Get_TI(uint *e_TI)
{
    e_TI[0] = TI[0];
    e_TI[1] = TI[1];
    e_TI[2] = TI[2];
    e_TI[3] = TI[3];
}

void SureGJK::Get_TNI(uint *e_TNI)
{
    e_TNI[0] = TNI[0];
    e_TNI[1] = TNI[1];
    e_TNI[2] = TNI[2];
    e_TNI[3] = TNI[3];
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

void SureGJK::Get_Cover(uint *e_C,uint *e_cc,bool *e_incover)
{
    for(uint i=0;i<cc;++i){
        e_C[i*3+0] = C[i*3+0];
        e_C[i*3+1] = C[i*3+1];
        e_C[i*3+2] = C[i*3+2];
    };
    for(uint i = 0;i<mc;++i){
        e_incover[i] = incover[i];
    };
    *e_cc = cc;
}

void SureGJK::Set_Cover(uint *i_C,uint *i_cc,bool *i_incover)
{
    cc = *i_cc;
    for(uint i = 0;i<cc;++i){
        C[i*3+0] = i_C[i*3+0];
        C[i*3+1] = i_C[i*3+1];
        C[i*3+2] = i_C[i*3+2];
    };
    for(uint i = 0;i<mc;++i){
        incover[i] = i_incover[i];
    };
}

void SureGJK::CoverFindNearestTo00(double *e_l,uint *e_f)
{
                    #define CVF0 M[C[ci*3+0]]
                    #define CVF1 M[C[ci*3+1]]
                    #define CVF2 M[C[ci*3+2]]
    double LM = SURE_R_MAXDISTANCE;
    uint cf = 0;
    for(uint ci=0;ci<cc;++ci)
    { // для каждой грани cover
    // (помним что для всех граней 0,0 внутри)
        __VTYPE L = fabs(dot(__NORMALIZE(cross(CVF1-CVF0,CVF2-CVF0)),-CVF0));
        // расстояние до 0,0
        if(L<LM)
        {
            LM = L;
            cf = ci;
        }; // (L<LM)
    }; // для каждой грани cover
    *e_l = LM;
    *e_f = cf;
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

#include <func_common.c>
#include <func_CPU.c>
