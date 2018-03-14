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

  //Scene_tetrs();
  //Scene_tetra(30,30,30,40,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);
  //Scene_tetra(25,15,30,20,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,true);

  //Scene_tetra(-50,10,20,80,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);
  //Scene_tetra(-25,-30,20,70,SURE_NORMALS_DEFAULT,SURE_MAPPING_PLANAR_XZ,false);

  //Scene_golem();
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
    sprintf(fname,"./%s.state",name);
    FILE *f = NULL;
    f = fopen(fname,"w");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };
    fprintf(f,"r_iters=%i\n",r_iters);
    fprintf(f,"r_rechecks=%i\n",r_rechecks);
    fprintf(f,"r_backlight=%.3f\n",r_backlight);
    fprintf(f,"paused=%i\n",paused);

    fprintf(f,"CameraInfo.cam_x=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_x.s[0],CameraInfo.cam_x.s[1],CameraInfo.cam_x.s[2]);
    fprintf(f,"CameraInfo.cam_vec=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_vec.s[0],CameraInfo.cam_vec.s[1],CameraInfo.cam_vec.s[2]);
    fprintf(f,"CameraInfo.cam_upvec=(%.4f;%.4f;%.4f)\n",CameraInfo.cam_upvec.s[0],CameraInfo.cam_upvec.s[1],CameraInfo.cam_upvec.s[2]);
    fprintf(f,"CameraInfo.xy_h=%.4f\n",CameraInfo.xy_h);
    fprintf(f,"CameraInfo.subp_rnd=%i\n",CameraInfo.subp_rnd);

    fclose(f);
    sprintf(LogLine,"Успешно сохранено состояние %s",name);
    Log->AddLine(LogLine);
};

void SureData::LoadState(const char *name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./%s.state",name);
    FILE *f = NULL;
    f = fopen(fname,"r");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };
    sprintf(LogLine,"Загружается состояние %s...",name);
    Log->AddLine(LogLine);
    if(fscanf(f,"r_iters=%hhu\n",&r_iters)<1) Log->AddLine("Ошибка чтения 1");
    if(fscanf(f,"r_rechecks=%hhu\n",&r_rechecks)<1) Log->AddLine("Ошибка чтения 2");
    if(fscanf(f,"r_backlight=%f\n",&r_backlight)<1) Log->AddLine("Ошибка чтения 3");
    if(fscanf(f,"paused=%hhu\n",&paused)<1) Log->AddLine("Ошибка чтения 5");

    if(fscanf(f,"CameraInfo.cam_x=(%lf;%lf;%lf)\n",&CameraInfo.cam_x.s[0],&CameraInfo.cam_x.s[1],&CameraInfo.cam_x.s[2])<3) Log->AddLine("Ошибка чтения 6");
    if(fscanf(f,"CameraInfo.cam_vec=(%lf;%lf;%lf)\n",&CameraInfo.cam_vec.s[0],&CameraInfo.cam_vec.s[1],&CameraInfo.cam_vec.s[2])<3) Log->AddLine("Ошибка чтения 7");
    if(fscanf(f,"CameraInfo.cam_upvec=(%lf;%lf;%lf)\n",&CameraInfo.cam_upvec.s[0],&CameraInfo.cam_upvec.s[1],&CameraInfo.cam_upvec.s[2])<3) Log->AddLine("Ошибка чтения 8");
    if(fscanf(f,"CameraInfo.xy_h=%lf\n",&CameraInfo.xy_h)<1) Log->AddLine("Ошибка чтения 9");
    if(fscanf(f,"CameraInfo.subp_rnd=%u\n",&CameraInfo.subp_rnd)<1) Log->AddLine("Ошибка чтения 10");

    reset = true;

    fclose(f);
    sprintf(LogLine,"Успешно загружено состояние %s",name);
    Log->AddLine(LogLine);
};

#include <func_common.c>
#include <func_CPU.c>
