
#define SURE_RLEVEL 99
// 90 и ниже -- отключает рассеивание.
// 60 и ниже -- без рандомизации и без теней
// 20 и ниже -- без отражений и преломлений

// define'ы ниже не вошли в SureDefines.h, так как они свои у GPU-кода
#define __VTYPE double
#define __VTYPE2 my_double2
#define __VTYPE3 my_double3
#define __FCONV3(A) (my_double3)(A)
#define __FCONV(A) A
#define __NORMALIZE(A) normalize(A)
#define __SURE_GLOBAL
#define __SURE_LOCAL
#define __SURE_CONSTANT
#define __SURE_PRIVATE
#define __SURE_STRUCT
#define __SURE_DECLARE_RANDOM
#define __SURE_UCHAR3 my_uchar3
#define __SURE_UCHAR4 my_uchar4
#define __SURE_UINT4 my_uchar4
#define __SURE_VINT4 my_int4
#define __SURE_MIN(A,B) (A<B?A:B)
#define __SURE_MAX(A,B) (A>B?A:B)
#define __LENGTH(A) sqrt(A.x*A.x+A.y*A.y+A.z*A.z)
#define __MAD(A,B,C) (A*B+C)
#define __XX s[0]
#define __YY s[1]
#define __ZZ s[2]

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
        DrawableCollided.rgb.s[2] = *tex; tex++; \
        DrawableCollided.rgb.s[1] = *tex; tex++; \
        DrawableCollided.rgb.s[0] = *tex; tex++; \
        DrawableCollided.transp = 1.01 - ((double)*tex / 255.0); \
        if(DrawableCollided.transp>0.5)DrawableCollided.dist_type=SURE_D_NORM;

#define __GET_ADVMAP(ix,iy,id) \
        uint iix = ix; \
        uint iiy = iy+SURE_R_TEXRES*id; \
        uchar *tex = &Textures[SURE_R_TEXRES*4*iiy+4*iix]; \
        DrawableCollided.radiance = *tex; tex++; \
        DrawableCollided.dist_sigma = *tex/20.0;


#define __GET_TEXTURE_UV(cm,id) \
__VTYPE map_px = __MESH_UV1_U(cm)+(__MESH_UV2_U(cm)-__MESH_UV1_U(cm))*u + \
                                  (__MESH_UV3_U(cm)-__MESH_UV1_U(cm))*v; \
__VTYPE map_py = __MESH_UV1_V(cm)+(__MESH_UV2_V(cm)-__MESH_UV1_V(cm))*u + \
                                  (__MESH_UV3_V(cm)-__MESH_UV1_V(cm))*v; \
__GET_TEXTURE(map_px,map_py,id);

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
#include <cwchar>

#include <CL/cl.h>

#include <SureLog.h>

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