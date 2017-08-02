#ifndef SUREDATA_H
#define SUREDATA_H

#include <SureDefines.h>

#define SURE_RLEVEL 10
// 90 и ниже -- отключает рассеивание.
// 60 и ниже -- без рандомизации и без теней
// 20 и ниже -- без отражений и преломлений

// define'ы ниже не вошли в SureDefines.h, так как они свои у GPU-кода
#define __VTYPE double
#define __VTYPE2 my_double2
#define __VTYPE3 my_double3
#define __FCONV3(A) (my_double3)A
#define __FCONV(A) A
#define __NORMALIZE(A) normalize(A)
#define __SURE_GLOBAL
#define __SURE_CONSTANT
#define __SURE_UCHAR3 my_uchar3
#define __SURE_UCHAR4 my_uchar4
#define __SURE_UINT4 my_uchar4
#define __SURE_VINT4 my_int4
#define __SURE_GLOBAL_MX amx
#define __SURE_GLOBAL_MY amy
#define __SURE_MIN(A,B) A<B?A:B
#define __SURE_MAX(A,B) A>B?A:B
#define __LENGTH(A) sqrt(A.x*A.x+A.y*A.y+A.z*A.z)
#define __MAD(A,B,C) A*B+C

#define __VERTEX_X(A) VrtxCLImg[A*4]
#define __VERTEX_Y(A) VrtxCLImg[A*4+1]
#define __VERTEX_Z(A) VrtxCLImg[A*4+2]
#define __VERTEX_SET(A,B) __VERTEX_X(A) = B.x; __VERTEX_Y(A) = B.y;  __VERTEX_Z(A) = B.z;
#define __MESH_V1(i) MeshCLImg[i*CLSIZE_VERTEX_DIM  ]
#define __MESH_V2(i) MeshCLImg[i*CLSIZE_VERTEX_DIM+1]
#define __MESH_V3(i) MeshCLImg[i*CLSIZE_VERTEX_DIM+2]

#define __MESH_UV1_U(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM                    ]
#define __MESH_UV2_U(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 1*CLSIZE_MESH_DIM]
#define __MESH_UV3_U(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 2*CLSIZE_MESH_DIM]

#define __MESH_UV1_V(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM                     + 1]
#define __MESH_UV2_V(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 1*CLSIZE_MESH_DIM + 1]
#define __MESH_UV3_V(i) UVMap[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 2*CLSIZE_MESH_DIM + 1]

#define __NORMAL1_X(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM                    ]
#define __NORMAL2_X(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 1*CLSIZE_MESH_DIM]
#define __NORMAL3_X(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 2*CLSIZE_MESH_DIM]
#define __NORMAL1_Y(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM                     + 1]
#define __NORMAL2_Y(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 1*CLSIZE_MESH_DIM + 1]
#define __NORMAL3_Y(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 2*CLSIZE_MESH_DIM + 1]
#define __NORMAL1_Z(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM                     + 2]
#define __NORMAL2_Z(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 1*CLSIZE_MESH_DIM + 2]
#define __NORMAL3_Z(i) Normals[i*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM + 2*CLSIZE_MESH_DIM + 2]

#define __GET_NORMAL1(P,VID) \
        P.x = __NORMAL1_X(VID); \
        P.y = __NORMAL1_Y(VID); \
        P.z = __NORMAL1_Z(VID);

#define __GET_NORMAL2(P,VID) \
        P.x = __NORMAL2_X(VID); \
        P.y = __NORMAL2_Y(VID); \
        P.z = __NORMAL2_Z(VID);

#define __GET_NORMAL3(P,VID) \
        P.x = __NORMAL3_X(VID); \
        P.y = __NORMAL3_Y(VID); \
        P.z = __NORMAL3_Z(VID);

#define __GET_VERTEX(P,VID) \
        P.x = __VERTEX_X(VID); \
        P.y = __VERTEX_Y(VID); \
        P.z = __VERTEX_Z(VID);

#define __GET_MESH(P,VID) \
        P.x = __MESH_V1(VID); \
        P.y = __MESH_V2(VID); \
        P.z = __MESH_V3(VID); \

#define __GET_TEXTURE(ix,iy,id) \
        uint iix = ix; \
        uint iiy = iy+SURE_R_TEXRES*id; \
        uchar *tex = &Textures[SURE_R_TEXRES*4*iiy+4*iix]; \
        col_rgba.z = *tex; tex++; \
        col_rgba.y = *tex; tex++; \
        col_rgba.x = *tex; tex++; \
        col_rgba.w = *tex; \
        col_transp = 1.01 - (col_rgba.w / 255.0); \
        col_rgb.x = col_rgba.x; \
        col_rgb.y = col_rgba.y; \
        col_rgb.z = col_rgba.z; \
        if(col_transp>0.5)col_dt=SURE_D_NORM;

#define __GET_ADVMAP(ix,iy,id) \
        uint iix = ix; \
        uint iiy = iy+SURE_R_TEXRES*id; \
        uchar *tex = &Textures[SURE_R_TEXRES*4*iiy+4*iix]; \
        col_radiance = *tex; tex++; \
        col_ds = *tex/20.0;

#define __GET_TEXTURE_UV(cm,id) \
__VTYPE map_px = __MESH_UV1_U(cm)+(__MESH_UV2_U(cm)-__MESH_UV1_U(cm))*u + \
                                  (__MESH_UV3_U(cm)-__MESH_UV1_U(cm))*v; \
__VTYPE map_py = __MESH_UV1_V(cm)+(__MESH_UV2_V(cm)-__MESH_UV1_V(cm))*u + \
                                  (__MESH_UV3_V(cm)-__MESH_UV1_V(cm))*v; \
__GET_TEXTURE(map_px,map_py,id); \

#define __GET_ADVMAP_UV(cm,id) \
__VTYPE map_px = __MESH_UV1_U(cm)+(__MESH_UV2_U(cm)-__MESH_UV1_U(cm))*u + \
                                  (__MESH_UV3_U(cm)-__MESH_UV1_U(cm))*v; \
__VTYPE map_py = __MESH_UV1_V(cm)+(__MESH_UV2_V(cm)-__MESH_UV1_V(cm))*u + \
                                  (__MESH_UV3_V(cm)-__MESH_UV1_V(cm))*v; \
__GET_ADVMAP(map_px,map_py,id); \

#include <QtCore/QtCore>
#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QtGui>

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <CL/cl.h>

// Математика для CPU-части:
// my_double3 имитирует cl_double3
 struct my_double3 {
   double x, y, z;
   my_double3(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; }
   inline my_double3 (cl_double3 b) {x=b.s[0]; y=b.s[1]; z=b.s[2];};
   inline operator cl_double3() const { cl_double3 d; d.s[0] = x;d.s[1] = y;d.s[2] = z; return d;};
   inline my_double3 operator+(const my_double3 &b) const { return my_double3(x+b.x,y+b.y,z+b.z); }
   inline my_double3 operator-(const my_double3 &b) const { return my_double3(x-b.x,y-b.y,z-b.z); }
   inline my_double3 operator-() const{return my_double3(-x,-y,-z);}
 };

 struct my_uchar3 {
   double x, y, z;
   my_uchar3(uchar x_=0, uchar y_=0, uchar z_=0){ x=x_; y=y_; z=z_; }
   my_uchar3 (cl_uchar3 b) {x=b.s[0]; y=b.s[1]; z=b.s[2];};
   operator cl_uchar3() const { cl_uchar3 d; d.s[0] = x;d.s[1] = y;d.s[2] = z; return d;};
};

 struct my_uchar4 {
   double x, y, z, w;
   my_uchar4(uchar x_=0, uchar y_=0, uchar z_=0, uchar w_=0){ x=x_; y=y_; z=z_; w=w_; }
   my_uchar4 (cl_uchar4 b) {x=b.s[0]; y=b.s[1]; z=b.s[2]; w=b.s[3];};
   operator cl_uchar4() const { cl_uchar4 d; d.s[0] = x;d.s[1] = y;d.s[2] = z; d.s[3] = w; return d;};
};

 struct my_int4 {
   int x, y, z, w;
   my_int4(int x_=0, int y_=0, int z_=0, int w_=0){ x=x_; y=y_; z=z_; w=w_; }
   my_int4 (cl_int4 b) {x=b.s[0]; y=b.s[1]; z=b.s[2]; w=b.s[3];};
   operator cl_int4() const { cl_int4 d; d.s[0] = x;d.s[1] = y;d.s[2] = z; d.s[3] = w; return d;};
};


 struct my_double2 {
   double x, y;
   my_double2(double x_=0, double y_=0){ x=x_; y=y_;}
   my_double2 operator+(const my_double2 &b) const { return my_double2(x+b.x,y+b.y);}
   my_double2 operator-(const my_double2 &b) const { return my_double2(x-b.x,y-b.y);}
 };

const my_double3 operator*(my_double3 a, double b);
const my_double3 operator*(double b, my_double3 a);
const my_double3 operator*(double b, my_uchar3 a);
my_double3& operator+=(my_double3 &a,const my_double3 &b);
my_double3& operator-=(my_double3 &a,const my_double3 &b);

// Эти функции дублируют встроенные функции OpenCL:
cl_double3 normalize(cl_double3);
my_double3 normalize(my_double3);
cl_double3 cross(cl_double3,cl_double3);
my_double3 cross(my_double3,my_double3);
cl_double dot(cl_double3,cl_double3);
double dot(my_double3,my_double3);
uint mad24(uint,uint,uint);

#include <SureGPUData.h>

struct SureObject
{
    my_double3 X;  //Координаты центра
    my_double3 ox; //Локальная ось x
    my_double3 oy; //Локальная ось y
    my_double3 oz; //Нормаль (Локальная ось z)
    double lx; // длина
    double ly; // ширина
    double lz; // высота
    double lp = 4.0; // величина иннерциального тетраэдра
    double rig = 0.7; // жесткость
    int type; // тип
    bool movable; // может двигаться
    bool collidable;
    bool mesh_changed;
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
    SureDrawable drawable;
    cl_uint mesh_start;
    cl_uint mesh_count;
    cl_uint vertex_start = 0;
    cl_uint vertex_count = 0;
    void initp4(){p1 = X+ox*(SURE_P4_X*lp)-oz*(SURE_P4_Y*lp);
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
        p1o=p1;p2o=p2;p3o=p3;p4o=p4;
        p1+=v1; p2+=v2; p3+=v3; p4+=v4;
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

void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl);

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

class SureData
{
    public:
        SureData();
        virtual ~SureData();
        void SureClear();
        // камера
        my_double3 cam_x = my_double3{-30,0,5};
        my_double3 cam_vec = my_double3{1,0,0};
        my_double3 cam_upvec = my_double3{0,0,1};
        my_double3 cam_dx = my_double3{0,0,0};
        my_double3 cam_dw = my_double3{0,0,0};
        double xy_h = 3.0;
        cl_uchar r_iters = 10;
        cl_uchar r_rechecks = 10;
        cl_float r_backlight = 0.5;

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
        SureTextureData TexturesInfo[SURE_R_MAXTEX];

        void LoadTexture(const char*);
        int GetTexture(const char*);
        int GenTexture(const char*,int type);

        SureObject objects[SURE_OBJ_MAX];
        int CreateObject(int type);
        SureLink links[SURE_LIN_MAX];
        int m_links = 0;
        int m_objects = 0;

        // Для отладки
        float frametime = 0;
    protected:
    private:
        void SetDefaultRotationBasis(int);
        void ObjCoordsToDrawable(int);
        int AddVertex(double x, double y, double z);
        int AddVertex(my_double3 X);
        int AddMesh(int v1, int v2, int v3);
        void Mesh_GenerateCube(int object,int norm_type);
        void Mesh_GenerateHull(int object,my_double3* vertexes,int vert_count,int norm_type);
        void MapTexture(int object,int type);
        void Mesh_FromFile(int object,const char* name);

        void Scene_box(); // коробка со светящимся потолком
        void Scene_floor(); // Пол и круглая лампа
        void Scene_golem();
        void Scene_metaball(double x, double y, double z, double sz,int nt);
};

struct SureOCLData{
    bool OpenCL = true;
    int rtype = SURE_RT_NOCL;
    cl_kernel* kernel;

    cl_kernel kernel_t;
    cl_kernel kernel_f;
    cl_kernel kernel_d;
    cl_kernel kernel_n;

    size_t sizes[2] = {1920,1080};
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

// Работа с OpenCL из CPU-части:
void ocl_errtext(cl_int);
int SourceFromFile(const char*,char*);
#define OCL_RUN(text,module) if(OCLData.OpenCL){ret=module;if(ret!=CL_SUCCESS){std::cout << "Ошибка OpenCL! " << text << ": "; ocl_errtext(ret) ;std::cout << "\n";OCLData.OpenCL=false;};}
#define OCL_GET(text,item,module) if(OCLData.OpenCL){item=module;if(ret!=CL_SUCCESS){std::cout << "Ошибка OpenCL! " << text << ": "; ocl_errtext(ret) ;std::cout << "\n";OCLData.OpenCL=false;};}
#define OCL_RUN_(text,module) if(OCLData->OpenCL){ret=module;if(ret!=CL_SUCCESS){std::cout << "Ошибка OpenCL! " << text << ": "; ocl_errtext(ret) ;std::cout << "\n";OCLData->OpenCL=false;};}
#define OCL_GET_(text,item,module) if(OCLData->OpenCL){item=module;if(ret!=CL_SUCCESS){std::cout << "Ошибка OpenCL! " << text << ": "; ocl_errtext(ret) ;std::cout << "\n";OCLData->OpenCL=false;};}
#define OCL_PROGRAM(prog,name,params) source_size = SourceFromFile(name,source_str); OCL_GET_("clCreateProgramWithSource",prog,clCreateProgramWithSource (context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret)); \
 OCL_RUN_("clBuildProgram",clBuildProgram(prog,1,&device,params,NULL,NULL)); if(ret!=CL_SUCCESS) { OCLData->OpenCL = true; OCL_RUN_("clGetProgramBuildInfo",clGetProgramBuildInfo \
 (prog,device,CL_PROGRAM_BUILD_LOG,30000,(void*)log_data,&logsize)); if(ret==CL_SUCCESS) { std::cout << "Журнал компиляции " << name << params << ":\n" << log_data << "\n"; }; OCLData->OpenCL = false; }


#endif // SUREDATA_H
