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
        Log = new SureLog("engine");

        LoadTexture("parket");
        LoadTexture("earth_adv");
        LoadTexture("colstones");
        LoadTexture("golem");
        LoadTexture("golem_adv");
        LoadTexture("earth");
        LoadTexture("test_adv");

        LoadModel("golem");
        GenModel("cube",0);
        GenModel("tetr",1);
// Шаблонный объект
        TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
        TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
        TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;

        TemplateObject.X.x = 0; TemplateObject.X.y = 0; TemplateObject.X.z = 0;
        TemplateObject.lx = 1.0; // длина
        TemplateObject.ly = 1.0; // ширина
        TemplateObject.lz = 1.0; // высота
        TemplateObject.lp = 1.7; // размер инерциоида
        TemplateObject.movable = true;
        TemplateObject.collidable = true;
        TemplateObject.drawable.X  = TemplateObject.X;
        TemplateObject.drawable.ox = TemplateObject.ox;
        TemplateObject.drawable.oy = TemplateObject.oy;
        TemplateObject.drawable.oz = TemplateObject.oz;
        TemplateObject.drawable.lx = TemplateObject.lx;
        TemplateObject.drawable.ly = TemplateObject.ly;
        TemplateObject.drawable.lz = TemplateObject.lz;
        TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
        TemplateObject.drawable.radiance = 0.0; // свечение
        TemplateObject.drawable.transp = 0;//0.8+0.3*(float)rand()/(float)RAND_MAX; // прозрачность
        TemplateObject.drawable.transp_i = 0.1;//+0.6*(float)rand()/(float)RAND_MAX; // прозрачность
        TemplateObject.drawable.refr = 1.49; // Коэффициент преломления
        TemplateObject.drawable.dist_type = SURE_D_NORM; // тип рандомизации
        TemplateObject.drawable.dist_sigma = 3.0; // sigma рандомизации
        TemplateObject.drawable.dist_m = 0 ; // матожидание рандомизации
        TemplateObject.drawable.rgb.s[0] = 200.0; // цвет
        TemplateObject.drawable.rgb.s[1] = 20.0;// цвет
        TemplateObject.drawable.rgb.s[2] = 20.0; // цвет
        TemplateObject.drawable.sided = true;
        TemplateObject.drawable.map_id = GetTexture("earth");
        TemplateObject.drawable.advmap_id = GetTexture("earth_adv");
        TemplateObject.initp4();



  //Scene_cube(0,0,20,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(20,-30,20,19,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ);
  //Scene_cube(-20,20,15,14,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_YZ);
  Scene_box();
  //Scene_floor();
  //Scene_mirrors();
  //Scene_ManyTetrs();

  //Scene_tetrs();
  //Scene_tetra(30,30,30,40,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);
  //Scene_tetra(25,15,30,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);

  //Scene_tetra(-50,10,20,80,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);
  //Scene_tetra(-25,-30,20,70,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);

  Scene_golem();
  //Scene_metaball(-10,-10,30,25,SURE_NORMALS_SHPERICAL);
  //Scene_metaball(30,30,20,20,SURE_NORMALS_DEFAULT);
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

        fprintf(f,"ModelName_collider=%s \n",objects[o].ModelName_collider);
        fprintf(f,"ModelName_drawable=%s \n",objects[o].ModelName_drawable);
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
};

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
        if(fscanf(f,"type=%u\n",&objects[m_objects].type)<1)Log->AddLine("Ошибка чтения");
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

        if(fscanf(f,"ModelName_collider=%s \n",objects[m_objects].ModelName_collider)<1)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"ModelName_drawable=%s \n",objects[m_objects].ModelName_drawable)<1)Log->AddLine("Ошибка чтения");
        objects[m_objects].ModelID_collider=GetModel(objects[m_objects].ModelName_collider);
        objects[m_objects].ModelID_drawable=GetModel(objects[m_objects].ModelName_drawable);

        READ_FLOAT_VECTOR_DRAWABLE(X);
        READ_FLOAT_VECTOR_DRAWABLE(ox);
        READ_FLOAT_VECTOR_DRAWABLE(oy);
        READ_FLOAT_VECTOR_DRAWABLE(oz);

        if(fscanf(f,"dr_Lxyz=(%f;%f;%f)\n",&objects[m_objects].drawable.lx,&objects[m_objects].drawable.ly,&objects[m_objects].drawable.lz)<3)Log->AddLine("Ошибка чтения");
        if(fscanf(f,"dr_type=%u\n",&objects[m_objects].drawable.type)<1)Log->AddLine("Ошибка чтения");
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
};

#include <func_common.c>
#include <func_CPU.c>
