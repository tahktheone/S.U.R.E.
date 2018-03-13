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
  // Scene_floor();

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

#include <func_common.c>

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
    if(pl<SURE_R_DELTA)return;
    my_double3 v1 = my_double3{0,0,0};
    my_double3 v2 = my_double3{0,0,0};

    if(o2->movable)o2->push(pp,pd,-pl*0.5);
    if(!o1->movable)o2->push(pp,pd,-pl*0.5);
    if(o1->movable)o1->push(pp,pd,pl*0.5);
    if(!o2->movable)o1->push(pp,pd,pl*0.5);

    if(o1->movable)v1 = o1->velbypp(pp);
    if(o2->movable)v2 = o2->velbypp(pp);
    my_double3 oz = __NORMALIZE(pd);
    double l_v1z = dot(oz,v1);
    double l_v2z = dot(oz,v2);
    my_double3 v1z = oz*l_v1z;
    my_double3 v1xy = v1-v1z;
    my_double3 v2z = oz*l_v2z;
    my_double3 v2xy = v2-v2z;

    if(o2->movable)o2->push(pp,v2xy,-1.0); // трение
    if(o1->movable)o1->push(pp,v1xy,-1.0); // трение

 }
