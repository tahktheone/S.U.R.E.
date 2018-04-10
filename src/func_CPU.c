
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

bool PhysCollideSphereSphere(SureObject* o1,SureObject* o2,SurePhysCollision* Collision)
{
    my_double3 pd = o1->X-o2->X;
    double d = __LENGTH(pd);
    if(d<(o1->lx+o2->lx))
    { // есть пересечение
        Collision->CollisionVector = __NORMALIZE(pd); // Penetration Direction
        Collision->CollisionPoint = (o1->X+o2->X)*0.5f; // Penetration point
        Collision->CollisionLength = o1->lx+o2->lx-d; // Penetration Length
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        return true;
    };// есть пересечение
    return false;
}

bool PhysCollideSpherePlane(SureObject* o1,SureObject* o2,SurePhysCollision* Collision)
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
            Collision->CollisionLength = (o1->lx-fabs(dist)); // Penetration Length
            Collision->CollisionVector = __NORMALIZE(o2->oz*dist);
            Collision->CollisionPoint = o1->X-Collision->CollisionVector*(o1->lx-Collision->CollisionLength); // Penetration point
            Collision->Object1 = o1;
            Collision->Object2 = o2;
            return true;
        }else{ // центр шара вне прямоугольника
            if(fabs(lx)<o2->lx){
            // в пределах оси x -- столкновение с гранью y
                double cd = sqrt(dist*dist+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                if(cd<o1->lx)
                {
                    if(ly<0){
                            Collision->CollisionPoint = o2->X+o2->ox*lx-o2->oy*o2->ly;
                    }else{
                            Collision->CollisionPoint = o2->X+o2->ox*lx+o2->oy*o2->ly;
                    };
                    Collision->CollisionLength = o1->lx-cd;
                    Collision->CollisionVector = __NORMALIZE(o1->X - Collision->CollisionPoint);
                    Collision->Object1 = o1;
                    Collision->Object2 = o2;
                    return true;
                };
            }else if(fabs(ly)<o2->ly){
            // в пределах оси y -- столкновение с гранью x
                double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx));
                if(cd<o1->lx)
                {
                    if(lx<0){
                            Collision->CollisionPoint = o2->X+o2->oy*ly-o2->ox*o2->lx;
                    }else{
                            Collision->CollisionPoint = o2->X+o2->oy*ly+o2->ox*o2->lx;
                    };
                    Collision->CollisionLength = o1->lx-cd;
                    Collision->CollisionVector = __NORMALIZE(o1->X - Collision->CollisionPoint);
                    Collision->Object1 = o1;
                    Collision->Object2 = o2;
                    return true;
                };
            }else{
            // столкновение с углом?
                double cd = sqrt(dist*dist+(fabs(lx)-o2->lx)*(fabs(lx)-o2->lx)+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                if(cd<o1->lx)
                {
                    Collision->CollisionPoint = o2->X;
                    if(lx<0){
                        Collision->CollisionPoint -= o2->ox*o2->lx;
                    }else{
                        Collision->CollisionPoint += o2->ox*o2->lx;
                    };
                    if(ly<0){
                        Collision->CollisionPoint -= o2->oy*o2->ly;
                    }else{
                        Collision->CollisionPoint += o2->oy*o2->ly;
                    };
                    Collision->CollisionLength = o1->lx-cd;
                    Collision->CollisionVector = __NORMALIZE(o1->X - Collision->CollisionPoint);
                    Collision->Object1 = o1;
                    Collision->Object2 = o2;
                    return true;
                };
            };
        }; // if(fabs(lx)<o2->lx&&fabs(ly)<o2->ly)
    };// есть пересечение
    return false;
}


void PhysPeflectSphereByPoint(my_double3 Point,my_double3 SphereCenter,SurePhysCollision* Collision,SureObject *o1,SureObject* o2,bool* collision_found)
{
    my_double3 VectorToSphere = SphereCenter - Point;
    __VTYPE DistanceToCenter = __LENGTH(VectorToSphere);
    __VTYPE LocalCollisionLength = o1->lx-DistanceToCenter;
    if(LocalCollisionLength>=Collision->CollisionLength){
        Collision->CollisionLength = LocalCollisionLength;
        Collision->CollisionVector = __NORMALIZE(VectorToSphere.x*o2->ox+VectorToSphere.y*o2->oy+VectorToSphere.z*o2->oz);
        Collision->CollisionPoint = o1->X-Collision->CollisionVector*DistanceToCenter;
        *collision_found = true;
    };
}

void PhysPeflectSphereByEdge(my_double3 Point1,my_double3 Point2,my_double3 SphereCenter,SurePhysCollision* Collision,SureObject *o1,SureObject* o2,bool* collision_found)
{
    my_double3 EdgeVector = Point2-Point1;
    double EdgeLength = __LENGTH(EdgeVector);
    EdgeVector = __NORMALIZE(EdgeVector);
    double DistanceToProjection = dot(EdgeVector,SphereCenter-Point1);
    my_double3 ProjectionPoint = Point1+EdgeVector*DistanceToProjection;
    my_double3 CollisionVector = SphereCenter-ProjectionPoint;
    double CollisionVectorLength = __LENGTH(CollisionVector);
    if(CollisionVectorLength<=o1->lx&&DistanceToProjection>0&&DistanceToProjection<EdgeLength){
        double LocalCollisionLength = o1->lx-CollisionVectorLength;
        if(LocalCollisionLength>=Collision->CollisionLength){
            Collision->CollisionLength = LocalCollisionLength;
            Collision->CollisionVector = __NORMALIZE(CollisionVector.x*o2->ox+CollisionVector.y*o2->oy+CollisionVector.z*o2->oz);
            Collision->CollisionPoint = o1->X-Collision->CollisionVector*CollisionVectorLength;
            *collision_found = true;
        };
    };
}

bool PhysCollideSphereMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision* Collision)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей
    // o1 - шарик
    // o2 - mesh
    bool collision_found = false;

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
                        if(cd>=Collision->CollisionLength)
                        {
                            collision_found = true;
                            Collision->CollisionLength = cd;
                            Collision->CollisionVector = __NORMALIZE(cn.x*o2->ox+cn.y*o2->oy+cn.z*o2->oz); // нормаль в глобальных координатах
                            Collision->CollisionPoint = o1->X+Collision->CollisionVector*dd;
                        }; //cd<=Collision->CollisionLength
                    }else{ // третья грань снаружи
                        // отталкиваемся от третей грани
                        PhysPeflectSphereByEdge(gm3,gm1,ltp,Collision,o1,o2,&collision_found);
                    };  // третья грань
                }else{// вторая грань снаружи
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // Отталкиваемся от второй грани
                        PhysPeflectSphereByEdge(gm2,gm3,ltp,Collision,o1,o2,&collision_found);
                    }else{ // третья грань снаружи
                        // отталкиваемся от угла gm3
                        PhysPeflectSphereByPoint(gm3,ltp,Collision,o1,o2,&collision_found);
                    };  // третья грань
                };  // вторая грань
            }else{ // первая грань снаружи
                if(dot(t2,cn)>0)
                { // вторая грань -- "внутри"
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // Отталкиваемся от первой грани
                        PhysPeflectSphereByEdge(gm1,gm2,ltp,Collision,o1,o2,&collision_found);
                    }else{ // третья грань снаружи
                        // отталкиваемся от угла gm1
                        PhysPeflectSphereByPoint(gm1,ltp,Collision,o1,o2,&collision_found);
                    };  // третья грань
                }else{ // вторая грань снаружи
                    if(dot(t3,cn)>0)
                    { // третья грань -- "внутри"
                        // отталкиваемся от угла gm2
                        PhysPeflectSphereByPoint(gm2,ltp,Collision,o1,o2,&collision_found);
                    }else{ // третья грань снаружи
                        // Все грани снаружи?
                        // Пространственный континум разорван,
                        // можно вываливаться в ошибку.
                    };  // третья грань
                }; // вторая грань
            }; // первая грань
        }; // сфера пересеклась с плоскостью
    }; // для каждой грани
    if(collision_found)
    {
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        return true;
    };
    return false;
}

bool PhysCollideMeshPlane(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision* Collision)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
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
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        Collision->CollisionLength = collision_distance;
        Collision->CollisionPoint = collision_point;
        Collision->CollisionVector = collision_normal;
        return true;
    }; // столкновение есть
    return false;
}

bool PhysCollideCreaturePlane(SureObject* o1,SureObject* o2,SurePhysCollision* Collision)
{
    return false;
}

bool PhysCollideMeshMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision* Collision)
{
    // ============================================================================================
    // ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
    // ============================================================================================
    SureGJK LocalGJK;
    LocalGJK.SetEngine(EngineData);
    // 1. Составляем разность минковского.
    LocalGJK.SetupMinkowski(o1,o2);
    LocalGJK.InitiateLoop();
    while(!LocalGJK.LoopExit()){
        if(!LocalGJK.TIContains00()){
            // тэтраэдр не содержит 0,0
            LocalGJK.LoopNextStep();
        }else{
            // тэттраэдр содержит 0,0
            LocalGJK.ExitLoop_Collision();
            LocalGJK.InitiateCoverLoop();
            while(!LocalGJK.CoverExpanded()){
                LocalGJK.ClearNewCover();
                LocalGJK.SetCollisionByCover();
                uint fndi = 0;
                if(LocalGJK.FindFarestMinkowskiByVector(LocalGJK.Collision.CollisionVector,&fndi)){
                    // cover нужно расширить добавив точку M[fndi]
                    LocalGJK.ExpandCover(fndi);
                    LocalGJK.SwitchCover();
                    LocalGJK.CheckCoverIterration();
                }else{
                    LocalGJK.SetCoverExpanded();
                }; // поиск расширения для cover
            }; // while (!cover_expanded

            double dist1;
            double dist2;
            my_double3 CollisionPoint1 = LocalGJK.GetCollisionPointByObject(o1,-LocalGJK.Collision.CollisionVector,&dist1);
            my_double3 CollisionPoint2 = LocalGJK.GetCollisionPointByObject(o2,LocalGJK.Collision.CollisionVector,&dist2);
            if(dist1<dist2){
                LocalGJK.Collision.CollisionPoint = CollisionPoint1;
            }else{
                LocalGJK.Collision.CollisionPoint = CollisionPoint2;
            };

            if(LocalGJK.CollisionFound()){
                *Collision = LocalGJK.Collision;
                return true;
            };
        };  // тэттраэдр содержит 0,0
    };// while(!exit)
    return false;
}
