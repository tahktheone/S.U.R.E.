#ifndef SUREDEFINES_H_INCLUDED
#define SUREDEFINES_H_INCLUDED

#define SURE_VERSION "0.0001"
#define SURE_TITLE "SURE"

#define SURE_CL_MAXSOURCE 100000

#define SURE_RT_NOCL    0
#define SURE_RT_F       1
#define SURE_RT_D       2
#define SURE_RT_T       3
#define SURE_RT_N       4

#define SURE_FAA 1
#define SURE_SCALE 2
#define SURE_SMOOTH 1

#define SURE_MAXRES_X 1920
#define SURE_MAXRES_Y 1080

#define SURE_OBJ_MAX     5000
#define SURE_LIN_MAX     5000
#define SURE_OBJ_NONE    0
#define SURE_OBJ_SPHERE  1
#define SURE_OBJ_CUBE    2
#define SURE_OBJ_PLANE   3
#define SURE_OBJ_MESH    4

#define SURE_DR_MAX     5000
#define SURE_DR_NONE    0
#define SURE_DR_SPHERE  1
#define SURE_DR_SQUARE  2
#define SURE_DR_MESH    3

#define SURE_R_DELTA       (1.0/100.0)
#define SURE_R_MAXDISTANCE 1000
#define SURE_R_FADELIMIT   (6.0/100.0)
#define SURE_R_RNDSIZE     12288

#define CLSIZE_VERTEX_DIM  4
#define CLSIZE_MESH_DIM    4
#define CLSIZE_VERTEX_SHF  8
#define CLSIZE_MESH_SHF    9
#define CLSIZE_VERTEX_DIV  256
#define SURE_R_MAXVERTEX   CLSIZE_VERTEX_DIV*CLSIZE_VERTEX_DIV
#define CLSIZE_VERTEX_PTCH CLSIZE_VERTEX_DIV*CLSIZE_VERTEX_DIM
#define CLSIZE_MESH_PTCH   CLSIZE_VERTEX_DIV*CLSIZE_VERTEX_DIM*CLSIZE_MESH_DIM
#define CLSIZE_VERTEX      SURE_R_MAXVERTEX*CLSIZE_VERTEX_DIM

#define SURE_R_MAXMESH     50000
#define SURE_R_MAXTEX      16
#define SURE_R_TEXRES      1024
#define SURE_R_MAXTEXMAP   SURE_R_MAXMESH

#define SURE_MAPPING_PLANAR_XZ  1
#define SURE_MAPPING_PLANAR_YZ  2
#define SURE_MAPPING_SPHERICAL  3
#define SURE_NORMALS_DEFAULT    0
#define SURE_NORMALS_SHPERICAL  1

#define SURE_D_EQUAL 1
#define SURE_D_NORM 2
#define SURE_D_NONE 3

#define SURE_PI 3.1415926536
#define SURE_PI2 1.570796327
#define SURE_P4_X 0.577350269
#define SURE_P4_Y 0.288675135
#define SURE_P4_Z 0.816496581

#define SURE_R_DELAY 20 // время между кадрами -- рендер
#define SURE_P_DELAY 8  // время между кадрами -- физика

#define SURE_P_DELTA       (1.0/10000.0)

#endif // SUREDEFINES_H_INCLUDED
