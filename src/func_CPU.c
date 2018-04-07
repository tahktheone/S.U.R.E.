
cl_double3 normalize(cl_double3 Vec)
{
    cl_double3 vr;
    double l = sqrt(Vec.s[0]*Vec.s[0] + Vec.s[1]*Vec.s[1] + Vec.s[2]*Vec.s[2]);
    vr.s[0] = Vec.s[0]/l; vr.s[1] = Vec.s[1]/l; vr.s[2] = Vec.s[2]/l;
    return vr;
}

my_double3 normalize(my_double3 Vec)
{
    my_double3 vr;
    double l = sqrt(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
    vr.x = Vec.x/l; vr.y = Vec.y/l; vr.z = Vec.z/l;
    return vr;
}

cl_double3 cross(cl_double3 v1, cl_double3 v2)
{
    cl_double3 vr;
    vr.s[0] = v1.s[1]*v2.s[2] - v1.s[2]*v2.s[1];
    vr.s[1] = v1.s[2]*v2.s[0] - v1.s[0]*v2.s[2];
	vr.s[2] = v1.s[0]*v2.s[1] - v1.s[1]*v2.s[0];
	return vr;
}

cl_double dot(cl_double3 v1, cl_double3 v2)
{
    return v1.s[0]*v2.s[0] + v1.s[1]*v2.s[1] + v1.s[2]*v2.s[2];
}

my_double3 cross(my_double3 v1, my_double3 v2)
{
    my_double3 vr;
    vr.x = v1.y*v2.z - v1.z*v2.y;
    vr.y = - v1.x*v2.z + v1.z*v2.x;
	vr.z = v1.x*v2.y - v1.y*v2.x;
	return vr;
}

double dot(my_double3 v1, my_double3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

uint mad24(uint x,uint y,uint z)
{
    return x*y+z;
}

const my_double3 operator*(my_double3 a, double b)
{ return my_double3(a.x*b,a.y*b,a.z*b); }

const my_double3 operator*(double b, my_double3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); }

const my_double3 operator*(double b, my_uchar3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); }

const my_double3 operator-(my_double3 a, cl_float3 b)
{ return my_double3(a.x-b.s[0],a.y-b.s[1],a.z-b.s[2]); }

const my_double3 operator+(cl_float3 b, my_double3 a)
{ return my_double3(a.x+b.s[0],a.y+b.s[1],a.z+b.s[2]); }

my_double3& operator+=(my_double3 &a,const my_double3 &b)
{ a.x+=b.x;a.y+=b.y;a.z+=b.z; return a; }

my_double3& operator-=(my_double3 &a,const my_double3 &b)
{ a.x-=b.x;a.y-=b.y;a.z-=b.z; return a; }



const cl_float3 operator-(cl_float3 a){
return cl_float3{-a.s[0],-a.s[1],-a.s[2]};
}

//const cl_float3 operator-(cl_float3 a,my_double3 b){
//return cl_float3{a.s[0]-b.x,a.s[1]-b.y,a.s[2]-b.z};
//};

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

void PhysCollideSphereSphere(SureObject* o1,SureObject* o2)
{
    my_double3 pd = o1->X-o2->X;
    double d = __LENGTH(pd);
    if(d<(o1->lx+o2->lx))
    { // есть пересечение
        pd = __NORMALIZE(pd); // Penetration Direction
        my_double3 pp = (o1->X+o2->X)*0.5f; // Penetration point
        double pl = o1->lx+o2->lx-d; // Penetration Length
        ObjCollide(o1,o2,pp,pd,pl);
    };// есть пересечение
}

void PhysCollideSpherePlane(SureObject* o1,SureObject* o2)
{
    // шарик с плоскостью
    // o1 - шарик
    // o2 - плоскость
    my_double3 vtp = o2->X - o1->X; // вектор от центра к точке на плоскость
    double dist = -dot(vtp,o2->oz); // расстояние от центра шара до плоскости
    if(fabs(dist)<o1->lx)
    { // есть пересечение
        double lx = -dot(vtp,o2->ox); // локальная x-координата центра шара
        double ly = -dot(vtp,o2->oy); // локальная y-координата центра шара
        if(fabs(lx)<o2->lx&&fabs(ly)<o2->ly)
        {// центр шара внутри прямоугольника
            my_double3 pd = o2->oz*dist; // Penetration Direction
            double pl = (o1->lx-fabs(dist)); // Penetration Length
            pd = __NORMALIZE(pd);
            my_double3 pp = o1->X-pd*(o1->lx-pl); // Penetration point
            ObjCollide(o1,o2,pp,pd,pl);
        }else{ // центр шара вне прямоугольника
            if(fabs(lx)<o2->lx){
            // в пределах оси x -- столкновение с гранью y
                my_double3 pp;
                double cd = sqrt(dist*dist+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                if(cd<o1->lx)
                {
                    if(ly<0){
                            pp = o2->X+o2->ox*lx-o2->oy*o2->ly;
                    }else{
                            pp = o2->X+o2->ox*lx+o2->oy*o2->ly;
                    };
                    double pl = o1->lx-cd;
                    my_double3 pd = o1->X - pp;
                    pd = __NORMALIZE(pd);
                    ObjCollide(o1,o2,pp,pd,pl);
                };
            }else if(fabs(ly)<o2->ly){
            // в пределах оси y -- столкновение с гранью x
                my_double3 pp;
                double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx));
                if(cd<o1->lx)
                {
                    if(lx<0){
                            pp = o2->X+o2->oy*ly-o2->ox*o2->lx;
                    }else{
                            pp = o2->X+o2->oy*ly+o2->ox*o2->lx;
                    };
                    double pl = o1->lx-cd;
                    my_double3 pd = o1->X - pp;
                    pd = __NORMALIZE(pd);
                    ObjCollide(o1,o2,pp,pd,pl);
                };
            }else{
            // столкновение с углом?
                my_double3 pp;
                double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx)+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                if(cd<o1->lx)
                {
                    pp = o2->X;
                    if(lx<0){
                        pp -= o2->ox*o2->lx;
                    }else{
                        pp += o2->ox*o2->lx;
                    };
                    if(ly<0){
                        pp -= o2->oy*o2->ly;
                    }else{
                        pp += o2->oy*o2->ly;
                    };
                    double pl = o1->lx-cd;
                    my_double3 pd = o1->X - pp;
                    pd = __NORMALIZE(pd);
                    ObjCollide(o1,o2,pp,pd,pl);
                };
            };
        }; // if(fabs(lx)<o2->lx&&fabs(ly)<o2->ly)
    };// есть пересечение
}

void PhysCollideSphereMesh(SureObject* o1,SureObject* o2,SureData* EngineData)
{
                cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
            cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей
    // Параметры для ObjCollide():
    // ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
    // o1 - шарик
    // o2 - mesh
    __VTYPE3 collision_point;
    __VTYPE3 collision_normal;
    __VTYPE collision_distance = 0;
    bool collision_found = false;

    // отталкиваем шарик от грани
    #define __REFL_FR(fr_b,fr_e) \
    __VTYPE3 vo = ltp - fr_b; \
    __VTYPE3 v = __NORMALIZE(fr_e-fr_b); \
    __VTYPE l = dot(v,vo); \
    __VTYPE3 fr = fr_e-fr_b; \
    __VTYPE lf = __LENGTH(fr); \
    __VTYPE3 p = fr_b + v*l; \
    __VTYPE3 dp = ltp-p; \
    __VTYPE pl = __LENGTH(dp); \
    if(pl<=o1->lx&&l>0&&l<lf){ \
        dd = o1->lx - pl; \
        if(dd>=collision_distance) \
        { \
            collision_found = true; \
            collision_distance = dd; \
            __VTYPE3 vp = __NORMALIZE(dp); \
            __VTYPE3 n = vp.x*o2->ox+ \
                         vp.y*o2->oy+ \
                         vp.z*o2->oz; \
            collision_normal = __NORMALIZE(n); \
            collision_point = o1->X-collision_normal*pl; \
        }; \
    };

    // отталкиваем шарик от угла
    #define __REFL_PT(pt) \
    __VTYPE3 cl = ltp - pt; \
    __VTYPE cdl = __LENGTH(cl); \
    __VTYPE cd = o1->lx-cdl; \
    if(cd>=collision_distance) \
    { \
        collision_found = true; \
        collision_distance = cd; \
        __VTYPE3 cln = __NORMALIZE(cl); \
        __VTYPE3 n = cln.x*o2->ox+ \
                     cln.y*o2->oy+ \
                     cln.z*o2->oz; \
        collision_normal = __NORMALIZE(n); \
        collision_point = o1->X-collision_normal*cdl; \
    };

    __VTYPE3 ltp; // координаты шара в локальных координатах mesh'ы
    ltp.x = dot(o1->X-o2->X,o2->ox);
    ltp.y = dot(o1->X-o2->X,o2->oy);
    ltp.z = dot(o1->X-o2->X,o2->oz);

    // для каждой грани:
    uint limit = EngineData->ModelsInfo[o2->ModelID_collider].mesh_count;
    uint lstart = EngineData->ModelsInfo[o2->ModelID_collider].mesh_start;
    for(uint im = 0;im<limit;++im)
    { // для каждой грани:
        uint cm = lstart + im;
        __SURE_VINT4 mesh;
        __VTYPE3 gm1;
        __VTYPE3 gm2;
        __VTYPE3 gm3;
        __GET_MESH(mesh,cm);
        __GET_VERTEX(gm1,mesh.x);
        __GET_VERTEX(gm2,mesh.y);
        __GET_VERTEX(gm3,mesh.z);
        gm1.x = gm1.x * o2->lx;
        gm1.y = gm1.y * o2->ly;
        gm1.z = gm1.z * o2->lz;
        gm2.x = gm2.x * o2->lx;
        gm2.y = gm2.y * o2->ly;
        gm2.z = gm2.z * o2->lz;
        gm3.x = gm3.x * o2->lx;
        gm3.y = gm3.y * o2->ly;
        gm3.z = gm3.z * o2->lz;

        // сфера пересекается с плоскостью?
            // определяем нормаль
        __VTYPE3 cn = __NORMALIZE(cross(gm2-gm1,gm3-gm1));
            // проеккция на нормаль вектора от центра шара к точке на плоскости
        __VTYPE dd = dot(cn,gm1-ltp);
            // длина проекции меньше радиуса шара?
        if(fabs(dd)<=o1->lx){
            // если да
            // ищем точку проекции центра шара на плоскость
            __VTYPE3 cp = ltp-cn*dd;
            __VTYPE3 t1 = cross(gm2-gm1,cp-gm1);
            __VTYPE3 t2 = cross(gm3-gm2,cp-gm2);
            __VTYPE3 t3 = cross(gm1-gm3,cp-gm3);
            if(dot(t1,cn)>0)
            { // первая грань -- "внутри"
                if(dot(t2,cn)>0)
                { // вторая грань -- "внутри"
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        __VTYPE cd = o1->lx-fabs(dd);
                        if(cd>=collision_distance)
                        {
                            collision_found = true;
                            collision_distance = cd;
                            __VTYPE3 n = cn.x*o2->ox+
                                         cn.y*o2->oy+
                                         cn.z*o2->oz; // нормаль в глобальных координатах
                            collision_normal = __NORMALIZE(n);
                            collision_point = o1->X+collision_normal*dd;
                        }; //cd<=collision_distance
                    }else{ // третья грань снаружи
                        // отталкиваемся от третей грани
                        __REFL_FR(gm3,gm1);
                    };  // третья грань
                }else{// вторая грань снаружи
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // Отталкиваемся от второй грани
                        __REFL_FR(gm2,gm3);
                    }else{ // третья грань снаружи
                        // отталкиваемся от угла gm3
                        __REFL_PT(gm3);
                    };  // третья грань
                };  // вторая грань
            }else{ // первая грань снаружи
                if(dot(t2,cn)>0)
                { // вторая грань -- "внутри"
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // Отталкиваемся от первой грани
                        __REFL_FR(gm1,gm2);
                    }else{ // третья грань снаружи
                        // отталкиваемся от угла gm1
                        __REFL_PT(gm1);
                    };  // третья грань
                }else{ // вторая грань снаружи
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // отталкиваемся от угла gm2
                        __REFL_PT(gm2);
                    }else{ // третья грань снаружи
                        // Все грани снаружи?
                        // Пространственный континум разорван,
                        // можно вываливаться в ошибку.
                    };  // третья грань
                }; // вторая грань
            }; // первая грань
        }; // сфера пересеклась с плоскостью
    }; // для каждой грани:
    if(collision_found)
        ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
}

void PhysCollideMeshPlane(SureObject* o1,SureObject* o2,SureData* EngineData)
{
                cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
            cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей
    // mesh с плоскостью
    // o1 - mesh
    // o2 - плоскость

    // определяем точки "под"
    // отсеиваем те что не в квадрате
    // определяем самую глубокую (и расстояние)
    // определяем точку (как среднее)

    __VTYPE3 ltp; // переводим данные о плоскости в локальные координаты meshы
    ltp.x = dot(o2->X-o1->X,o1->ox);
    ltp.y = dot(o2->X-o1->X,o1->oy);
    ltp.z = dot(o2->X-o1->X,o1->oz);

    __VTYPE3 lox;
    lox.x = dot(o2->ox,o1->ox);
    lox.y = dot(o2->ox,o1->oy);
    lox.z = dot(o2->ox,o1->oz);
    lox = __NORMALIZE(lox);

    __VTYPE3 loy;
    loy.x = dot(o2->oy,o1->ox);
    loy.y = dot(o2->oy,o1->oy);
    loy.z = dot(o2->oy,o1->oz);
    loy = __NORMALIZE(loy);

    __VTYPE3 loz;
    loz.x = dot(o2->oz,o1->ox);
    loz.y = dot(o2->oz,o1->oy);
    loz.z = dot(o2->oz,o1->oz);
    loz = __NORMALIZE(loz);

    __VTYPE3 collision_point;
    collision_point.x = collision_point.y = collision_point.z = 0;
    __VTYPE collision_count = 0;
    __VTYPE3 collision_normal;
    __VTYPE collision_distance = 0;
    bool collision_found = false;

    bool l_up = false;
    bool l_down = false;
    // для каждой точки
    uint l_limit = EngineData->ModelsInfo[o1->ModelID_collider].vertex_count;
    uint l_start = EngineData->ModelsInfo[o1->ModelID_collider].vertex_start;
    for(uint iv = 0;iv<l_limit;++iv)
    { // для каждой точки
        uint cv = l_start + iv;
        __VTYPE3 pt;
        __GET_VERTEX(pt,cv);
        pt.x = pt.x * o1->lx;
        pt.y = pt.y * o1->ly;
        pt.z = pt.z * o1->lz;

        // pt - lpt вектор от точки на плоскости к к точке mesh'а
        __VTYPE cd = dot(pt-ltp,loz);
        if(cd<0)
        { // точка "под" плоскостью
            l_down = true;
            __VTYPE3 cpt = pt + loz*cd; // точка на плоскости, проекция точки mesh'а
            __VTYPE llx = dot(cpt-ltp,lox); // x и y на плоскости
            __VTYPE lly = dot(cpt-ltp,loy);

            if(fabs(llx)<o2->lx&&fabs(lly)<o2->ly){
            // точка внутри квадрата
                collision_found = true;
                collision_count = collision_count + 1.0;
                collision_point = collision_point + pt;
                if(fabs(cd)>collision_distance)
                    collision_distance = fabs(cd);
            };// точка внутри квадрата
        }else{ // точка "под" плоскостью
        // точка "над" плоскостью
            l_up = true;
        };
    }; // для каждой точки
    // отталкиваем
    if(l_up&&l_down&&collision_found){
        collision_normal = o2->oz;
        __VTYPE3 cp = collision_point * (1.0/collision_count); // среднее
        collision_point = cp.x*o1->ox+
                          cp.y*o1->oy+
                          cp.z*o1->oz+
                          o1->X;
        ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
    }; // столкновение есть
}

void PhysCollideCreaturePlane(SureObject* o1,SureObject* o2)
{

}

void PhysCollideMeshMesh(SureObject* o1,SureObject* o2,SureData* EngineData)
{
    // ============================================================================================
    // ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
    // ============================================================================================

            cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
            cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей

    __VTYPE3 collision_point;
    __VTYPE3 collision_normal;
    __VTYPE collision_distance = 0;
    bool collision_found = false;

    uint mc = 0; // количество точек в разности минковского
    __VTYPE3 p1,p2; // точки из объектов, для получения разности
    __VTYPE3 gp1,gp2; // точки в глобальных координатах
    __VTYPE3 M[SURE_MINKOWSKI_MAX];  // разность минкосвского

    // 1. Составляем разность минковского.
    // для каждой точки
    uint l1_limit = EngineData->ModelsInfo[o1->ModelID_collider].vertex_count;
    uint l1_start = EngineData->ModelsInfo[o1->ModelID_collider].vertex_start;
    uint l2_limit = EngineData->ModelsInfo[o2->ModelID_collider].vertex_count;
    uint l2_start = EngineData->ModelsInfo[o2->ModelID_collider].vertex_start;

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

    // [Тэтраэдр T. 4 точки. 2 раза -- для текущего шага и для следующего]
    uint TI[4];
    uint TNI[4] = {0,0,0,0};
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

    bool exit_no_collision = false;
    bool exit_collision = false;
    bool incover[SURE_MINKOWSKI_MAX];

    uint iter = 0; // подсчет итерраций - гарантия от зацикливаний

    while(!(exit_no_collision||exit_collision))
    {
        #ifdef COLLISION_LOGGING_00
        l_log_index++;
        l_log_textline = 0;
        #endif // COLLISION_LOGGING

        //Проверяем, содержит ли тэтраэдр 0,0.
        //заодно ищем грань ближайшую к 0,0
        __VTYPE L1 = dot(__NORMALIZE(cross(M[TI[1]]-M[TI[0]],M[TI[2]]-M[TI[0]])),-M[TI[0]]); // грань 0 1 2
        __VTYPE L2 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[0]],M[TI[1]]-M[TI[0]])),-M[TI[0]]); // грань 0 3 1
        __VTYPE L3 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[1]],M[TI[2]]-M[TI[1]])),-M[TI[1]]); // грань 1 3 2
        __VTYPE L4 = dot(__NORMALIZE(cross(M[TI[3]]-M[TI[2]],M[TI[0]]-M[TI[2]])),-M[TI[2]]); // грань 2 3 0

        if(L1>0||L2>0||L3>0||L4>0)
        { // тэтраэдр не содержит 0,0
            bool f = false;
            #define SET_TN(A,B,C) TN[0]=T[A];TN[1]=T[B];TN[2]=T[C];
            #define SET_TNI(A,B,C) TNI[0]=TI[A];TNI[1]=TI[B];TNI[2]=TI[C];
            __VTYPE LM = 0;
            // ищем грань, для которой [0,0] снаружи
            if(L1>LM){ SET_TNI(0,1,2); LM=L1 + SURE_R_DELTA; f = true; };
            if(L2>LM){ SET_TNI(0,3,1); LM=L2 + SURE_R_DELTA; f = true; };
            if(L3>LM){ SET_TNI(1,3,2); LM=L3 + SURE_R_DELTA; f = true; };
            if(L4>LM){ SET_TNI(2,3,0); LM=L4 + SURE_R_DELTA; f = true; };
            if(f)
            {
                __VTYPE3 v = __NORMALIZE(cross(M[TNI[1]]-M[TNI[0]],M[TNI[2]]-M[TNI[0]])); // нормаль основания нового тэтраэдра
                __VTYPE md = dot(v,M[TNI[0]])+SURE_R_DELTA;
                __VTYPE ld;
                f = false;
                for(uint i = 0;i<mc;++i)
                {// для каждой точки M[i] -- есть точка в направлении v?
                    ld = dot(v,M[i]);
                    if(ld>md)
                    {
                        md = ld;
                        TNI[3] = i;
                        f = true;
                    };
                };

                if(f)
                {   // нужно новый тэтраэдр
                    TI[0]=TNI[0];
                    TI[1]=TNI[2];
                    TI[2]=TNI[1];
                    TI[3]=TNI[3];
                }else{
                    exit_no_collision = true;
                };
                iter++;
                if(iter>20){
                    exit_no_collision = true;
                    EngineData->Log->AddLine("f!");
                };
            }else{ // расстояние до 0,0 больше преыдущего
                exit_no_collision = true;
            };
        }else{
            // тэттраэдр содержит 0,0
            exit_collision = true;
            bool cover_expanded = false;
            collision_found = true;
            // создаем cover для минковского

            uint C[SURE_MINKOWSKI_MAX];
            uint cc = 0;
            uint C_N[SURE_MINKOWSKI_MAX];
            uint cc_n = 0;

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
            while(!cover_expanded)
            {
                    #define CF0 M[C[ci*3+0]]
                    #define CF1 M[C[ci*3+1]]
                    #define CF2 M[C[ci*3+2]]

                //Ищем грань ближайшую к 0 0.
                __VTYPE LM = SURE_R_MAXDISTANCE;
                uint cf = 0;
                cc_n = 0;
                //std::cout << "C:" << "\n";
                for(uint ci=0;ci<cc;++ci)
                { // для каждой грани cover
                // (помним что для всех граней 0,0 внутри)
                    __VTYPE L = fabs(dot(__NORMALIZE(cross(CF1-CF0,CF2-CF0)),-CF0));
                    // расстояние до 0,0
                    if(L<LM)
                    {
                        LM = L;
                        cf = ci;
                    }; // (L<LM)
                }; // для каждой грани cover

                collision_distance = LM;

                // в направлении от 0,0 есть еще точки?
                #define CFF0 M[C[cf*3+0]]
                #define CFF1 M[C[cf*3+1]]
                #define CFF2 M[C[cf*3+2]]
                __VTYPE3 v = __NORMALIZE(cross(CFF1-CFF0,CFF2-CFF0));
                collision_normal = v;

                // ищем дальнюю точку в M в направлении v
                __VTYPE md = dot(v,CFF0)+SURE_R_DELTA;
                __VTYPE ld;
                bool f = false;
                uint fndi = 0;
                for(uint li = 0;li<mc;++li){// для каждой точки M[i]
                    if(!incover[li]){ // если точка не в оболочке уже
                        ld = dot(v,M[li]);
                        if(ld>md){
                            md = ld;
                            fndi = li;
                            f = true;
                        };
                    }; // если точка не в оболочке уже
                }; // для каждой точки M[i]

                if(f){
                // cover нужно расширить добавив точку M[fndi]
                    incover[fndi] = true;
                    for(uint aci = 0;aci<cc;++aci)
                    { // для каждой грани cover
                        #define CFA0 M[C[aci*3+0]]
                        #define CFA1 M[C[aci*3+1]]
                        #define CFA2 M[C[aci*3+2]]
                        // грань видимая?
                        if(dot(cross(CFA1-CFA0,CFA2-CFA0),M[fndi]-CFA0)>SURE_R_DELTA)
                        { // грань видимая
                            bool out = false;

                            // Логика check_cover:
                            // для всех точек в M[] проверяем, лежат ли они снаружи
                            // проверяемой грани. Если все внутри --
                            // добавляем грань в COVER
                            #define CHECK_COVER(A0,B0,C0) \
                            out = false; \
                            for(uint cci = 0;cci<mc;++cci) \
                                if(incover[cci]) \
                                    if(dot(cross(M[B0]-M[A0],M[C0]-M[A0]),M[cci]-M[A0])>SURE_R_DELTA) \
                                        out = true; \
                            if(!out) \
                            { \
                                C_N[cc_n*3+0] = A0; \
                                C_N[cc_n*3+1] = B0; \
                                C_N[cc_n*3+2] = C0; \
                                ++cc_n; \
                            };
                            CHECK_COVER(C[aci*3+0],C[aci*3+1],fndi);
                            CHECK_COVER(C[aci*3+1],C[aci*3+2],fndi);
                            CHECK_COVER(C[aci*3+2],C[aci*3+0],fndi);

                        }else{
                         // грань невидимая - оставляем в cover
                            C_N[cc_n*3+0] = C[aci*3+0];
                            C_N[cc_n*3+1] = C[aci*3+1];
                            C_N[cc_n*3+2] = C[aci*3+2];
                            ++cc_n;
                        }; // грань видимая?
                    };// для каждой грани cover

                    for(uint icn = 0;icn<cc_n;++icn)
                    {
                        C[icn*3+0] = C_N[icn*3+0];
                        C[icn*3+1] = C_N[icn*3+1];
                        C[icn*3+2] = C_N[icn*3+2];
                    };
                    cc = cc_n;

                    if(iter>16)
                    {
                        cover_expanded = true;
                        EngineData->Log->AddLine("i!");
                    };
                    ++iter;

                }else{
                    //std::cout << "fin(" << iter << ")" << "\n";
                    cover_expanded = true;

                }; // поиск расширения для cover
            }; // while (!cover_expanded
            // cover максимально расширен в сторону 0,0
            // уже есть collision_normal, collision_distance
            // Для каждого объекта оперделяем "крайние" точки в направлении вектора контакта
            // у нас o2 - o1.
            // это значит нормаль направлена в сторону 1го объекта (так ведь?)
            // для 1го объекта ищем в направлении -n
            // для 2го в направлении n

            #define __RE_MINMAX \
            xmax = -SURE_R_MAXDISTANCE; \
            ymax = -SURE_R_MAXDISTANCE; \
            zmax = -SURE_R_MAXDISTANCE; \
            xmin = SURE_R_MAXDISTANCE; \
            ymin = SURE_R_MAXDISTANCE; \
            zmin = SURE_R_MAXDISTANCE; \
            G_max = -SURE_R_MAXDISTANCE; \
            G_min = SURE_R_MAXDISTANCE;

            #define __GET_VERTEX_BYOBJ_G(l_start,o,id,gvertex) \
            uint cv = l_start + id; \
            __VTYPE3 lvertex; \
            __GET_VERTEX(lvertex,cv); \
            lvertex.x = lvertex.x * o->lx; \
            lvertex.y = lvertex.y * o->ly; \
            lvertex.z = lvertex.z * o->lz; \
            gvertex = o->ox*lvertex.x + o->oy*lvertex.y+o->oz*lvertex.z + o->X;

            #define __GET_MINMAX_BYVEC(o,vec,gmin,gmax,l_start,l_limit) \
            for(uint ii = 0;ii<l_limit;++ii) \
            { \
                __VTYPE3 gp; \
                __GET_VERTEX_BYOBJ_G(l_start,o,ii,gp); \
                __VTYPE l = dot(vec,gp); \
                GL[ii]=l; \
                if(l>gmax)gmax=l; \
                if(l<gmin)gmin=l; \
            };

            #define __GET_AVER_DIS_BYOBJ(o,av,avc,dis,l_start,l_limit) \
            for(uint ii = 0;ii<l_limit;++ii) \
            { \
                if(GL[ii]>GD) \
                { \
                    __VTYPE3 gp; \
                    __GET_VERTEX_BYOBJ_G(l_start,o,ii,gp); \
                    if(xmax<gp.x)xmax=gp.x; \
                    if(ymax<gp.y)ymax=gp.y; \
                    if(zmax<gp.z)zmax=gp.z; \
                    if(xmin>gp.x)xmin=gp.x; \
                    if(ymin>gp.y)ymin=gp.y; \
                    if(zmin>gp.z)zmin=gp.z; \
                    colverts[colverts_c] = gp; \
                    colverts_c++; \
                    av = av + gp; \
                    avc = avc + 1.0; \
                }; \
            }; \
            dis = xmax-xmin + ymax-ymin + zmax-zmin; \
            av = av * (1.0/avc); \

            __VTYPE G_max = -SURE_R_MAXDISTANCE;
            __VTYPE G_min = SURE_R_MAXDISTANCE;
            __VTYPE GD = 0;
            __VTYPE3 n = -collision_normal;
            __VTYPE GL[64]; // список расстояний проекций радиус-векторов
                            // всех точек объекта на верктор столкновения
            __VTYPE xmax;
            __VTYPE ymax;
            __VTYPE zmax;
            __VTYPE xmin;
            __VTYPE ymin;
            __VTYPE zmin;
            __VTYPE3 aver1 = {0,0,0};
            __VTYPE3 aver2 = {0,0,0};
            __VTYPE avc1 = 0;
            __VTYPE avc2 = 0;
            __VTYPE g1dis;
            __VTYPE g2dis;

            __GET_MINMAX_BYVEC(o1,n,G_min,G_max,l1_start,l1_limit); // нашли самую дальюю и самую ближнюю точки
            GD = G_max-G_min;
            GD *= 0.999; // отсекаем только дальние
            GD += G_min;
            __RE_MINMAX;
            __VTYPE3 colverts[16];
            int colverts_c = 0;
            __VTYPE l_dcn;
            __GET_AVER_DIS_BYOBJ(o1,aver1,avc1,g1dis,l1_start,l1_limit); // ищем среднее по дальним 5% точек
            if(colverts_c>=3){
                // если в коллизии учавствует плоскость -- поворачиваем нормаль
                __VTYPE3 tcn = collision_normal;
                collision_normal = __NORMALIZE(cross(colverts[1]-colverts[0],colverts[2]-colverts[0]));
                l_dcn = dot(collision_normal,tcn);
                if(l_dcn<0)
                    collision_normal = - collision_normal;
                collision_distance = collision_distance * fabs(l_dcn);
            };

            __GET_MINMAX_BYVEC(o2,collision_normal,G_min,G_max,l2_start,l2_limit); // нашли самую дальюю и самую ближнюю точки
            GD = G_max-G_min;
            GD *= 0.999; // отсекаем только дальние
            GD += G_min;
            __RE_MINMAX;
            colverts_c = 0;
            __GET_AVER_DIS_BYOBJ(o2,aver2,avc2,g2dis,l2_start,l2_limit); // ищем среднее по дальним 5% точек
            if(colverts_c>=3){
                // если в коллизии учавствует плоскость -- поворачиваем нормаль
                __VTYPE3 tcn = collision_normal;
                collision_normal = __NORMALIZE(cross(colverts[1]-colverts[0],colverts[2]-colverts[0]));
                l_dcn = dot(collision_normal,tcn);
                if(l_dcn<0)
                    collision_normal = - collision_normal;
                collision_distance = collision_distance * fabs(l_dcn);
            };

            if(g1dis<g2dis)
            {
                collision_point = aver1;
            }else{
                collision_point = aver2;
            };

            if(collision_found)
                ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
        };  // тэттраэдр содержит 0,0
    };// while(!exit)
}
