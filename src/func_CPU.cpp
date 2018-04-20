
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
    bool tmpMovable1 = o1->movable;
    bool tmpMovable2 = o2->movable;
    if(o1->type==SURE_OBJ_PS)o1->movable = false;
    if(o2->type==SURE_OBJ_PS)o2->movable = false;

    float k1 = o2->movable ? o2->mass / (o1->mass + o2->mass) : 1.0f;
    float k2 = o1->movable ? o1->mass / (o1->mass + o2->mass) : 1.0f;

    if(o2->movable)o2->push(pp,pd,-pl*k2);
    if(o1->movable)o1->push(pp,pd,pl*k1);

    my_double3 v1 = my_double3{0,0,0};
    my_double3 v2 = my_double3{0,0,0};
    if(o1->movable)v1 = o1->velbypp(pp);
    if(o2->movable)v2 = o2->velbypp(pp);
    my_double3 oz = __NORMALIZE(pd);
    double l_v1z = dot(oz,v1);
    double l_v2z = dot(oz,v2);
    my_double3 v1z = oz*l_v1z;
    my_double3 v1xy = v1-v1z;
    my_double3 v2z = oz*l_v2z;
    my_double3 v2xy = v2-v2z;

    if(o2->movable)o2->push(pp,v2xy,-k2); // трение
    if(o1->movable)o1->push(pp,v1xy,-k1); // трение
    o1->movable = tmpMovable1;
    o2->movable = tmpMovable2;
 }

 my_double3 RelativeCoordinatesToGlobal(SureObject *o,my_double3 X){
     return o->X + o->ox*X.x*o->lx + o->oy*X.y*o->ly + o->oz*X.z*o->lz;
 }

  my_double3 RelativeVectorToGlobal(SureObject *o,my_double3 X){
     return o->ox*X.x*o->lx + o->oy*X.y*o->ly + o->oz*X.z*o->lz;
 }

my_double3 GlobalCoordinatesToRelative(SureObject *o,my_double3 X)
{
    my_double3 result;
    my_double3 VectorToPoint = X - o->X;
    result.x = dot(VectorToPoint,o->ox)/o->lx;
    result.y = dot(VectorToPoint,o->oy)/o->ly;
    result.z = dot(VectorToPoint,o->oz)/o->lz;
    return result;
}

my_double3 GlobalVectorToRelative(SureObject *o,my_double3 X)
{
    my_double3 result;
    result.x = dot(X,o->ox)/o->lx;
    result.y = dot(X,o->oy)/o->ly;
    result.z = dot(X,o->oz)/o->lz;
    return result;
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
    Collision->CollisionLength = -1.0;

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

bool PhysCollidePSSphere(SureObject* o1,SureObject* o2,SurePhysCollision* Collision)
{
    SurePhysCollision TotalCollision;
    int TotalCollisions = 0;
    for(int ip = 0;ip<o1->ParticlesCounter;++ip){
        // для каждой частицы
        my_double3 GlobalParticleX = RelativeCoordinatesToGlobal(o1,o1->Particles[ip].X);
        my_double3 VecParticleToSphere = GlobalParticleX - o2->X;
        double d = __LENGTH(VecParticleToSphere);
        if(d<(o1->Particles[ip].Size+o2->lx))
        { // есть пересечение
            Collision->CollisionVector = __NORMALIZE(VecParticleToSphere); // Penetration Direction
            Collision->CollisionPoint = (GlobalParticleX + o2->X)*0.5f; // Penetration point
            Collision->CollisionLength = o1->Particles[ip].Size+o2->lx-d; // Penetration Length
            o1->Particles[ip].X += GlobalVectorToRelative(o1,Collision->CollisionVector) * Collision->CollisionLength * 0.3f;
            TotalCollision.CollisionLength += Collision->CollisionLength*o1->mass;
            TotalCollision.CollisionPoint += Collision->CollisionPoint;
            TotalCollision.CollisionVector += Collision->CollisionVector;
            ++TotalCollisions;
        };// есть пересечение
    };// для каждой частицы
    if(TotalCollisions>0){
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        Collision->CollisionLength = TotalCollision.CollisionLength * (1.0 / (double)TotalCollisions);
        Collision->CollisionPoint = TotalCollision.CollisionPoint * (1.0 / (double)TotalCollisions);
        Collision->CollisionVector = __NORMALIZE(TotalCollision.CollisionVector);
        return true;
    };
    return false;
}

bool PhysCollidePSPlane(SureObject* o1,SureObject* o2,SurePhysCollision* Collision)
{
    SurePhysCollision TotalCollision;
    int TotalCollisions = 0;

    for(int ip = 0;ip<o1->ParticlesCounter;++ip){
        SureObject LocalSphere;
        LocalSphere.X = RelativeCoordinatesToGlobal(o1,o1->Particles[ip].X);
        LocalSphere.lx = o1->Particles[ip].Size;
        if(PhysCollideSpherePlane(&LocalSphere,o2,Collision)){
            o1->Particles[ip].X += GlobalVectorToRelative(o1,Collision->CollisionVector) * Collision->CollisionLength * 0.3f;
            TotalCollision.CollisionLength += Collision->CollisionLength*o1->mass;
            TotalCollision.CollisionPoint += Collision->CollisionPoint;
            TotalCollision.CollisionVector += Collision->CollisionVector;
            ++TotalCollisions;
        };
    };
    if(TotalCollisions>0){
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        Collision->CollisionLength = TotalCollision.CollisionLength * (1.0 / (double)TotalCollisions);
        Collision->CollisionPoint = TotalCollision.CollisionPoint * (1.0 / (double)TotalCollisions);
        Collision->CollisionVector = __NORMALIZE(TotalCollision.CollisionVector);
        return true;
    };
    return false;
}

bool PhysCollideAABB(SureObject* o1,SureObject* o2)
{
    my_double3 OABB1[8];
    OABB1[0] = RelativeCoordinatesToGlobal(o1,my_double3( 1, 1, 1));
    OABB1[1] = OABB1[0]-o1->oy*o1->ly*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1,-1, 1));
    OABB1[2] = OABB1[1]-o1->ox*o1->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1,-1, 1));
    OABB1[3] = OABB1[0]-o1->ox*o1->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1, 1, 1));
    OABB1[4] = OABB1[0]-o1->oz*o1->lz*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1, 1,-1));
    OABB1[5] = OABB1[4]-o1->oy*o1->ly*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1,-1,-1));
    OABB1[6] = OABB1[5]-o1->ox*o1->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1,-1,-1));
    OABB1[7] = OABB1[4]-o1->ox*o1->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1, 1,-1));

    my_double3 min_1 = {SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE};
    my_double3 max_1 = {-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE};
    for(int i = 0;i<8;++i){
      if(OABB1[i].x<min_1.x)min_1.x=OABB1[i].x;
      if(OABB1[i].y<min_1.y)min_1.y=OABB1[i].y;
      if(OABB1[i].z<min_1.z)min_1.z=OABB1[i].z;
      if(OABB1[i].x>max_1.x)max_1.x=OABB1[i].x;
      if(OABB1[i].y>max_1.y)max_1.y=OABB1[i].y;
      if(OABB1[i].z>max_1.z)max_1.z=OABB1[i].z;
    };

    my_double3 OABB2[8];
    OABB2[0] = RelativeCoordinatesToGlobal(o2,my_double3( 1, 1, 1));
    OABB2[1] = OABB2[0]-o2->oy*o2->ly*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1,-1, 1));
    OABB2[2] = OABB2[1]-o2->ox*o2->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1,-1, 1));
    OABB2[3] = OABB2[0]-o2->ox*o2->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1, 1, 1));
    OABB2[4] = OABB2[0]-o2->oz*o2->lz*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1, 1,-1));
    OABB2[5] = OABB2[4]-o2->oy*o2->ly*2.0;//RelativeCoordinatesToGlobal(o1,my_double3( 1,-1,-1));
    OABB2[6] = OABB2[5]-o2->ox*o2->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1,-1,-1));
    OABB2[7] = OABB2[4]-o2->ox*o2->lx*2.0;//RelativeCoordinatesToGlobal(o1,my_double3(-1, 1,-1));

    my_double3 min_2 = {SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE};
    my_double3 max_2 = {-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE};
    for(int i = 0;i<8;++i){
      if(OABB2[i].x<min_2.x)min_2.x=OABB2[i].x;
      if(OABB2[i].y<min_2.y)min_2.y=OABB2[i].y;
      if(OABB2[i].z<min_2.z)min_2.z=OABB2[i].z;
      if(OABB2[i].x>max_2.x)max_2.x=OABB2[i].x;
      if(OABB2[i].y>max_2.y)max_2.y=OABB2[i].y;
      if(OABB2[i].z>max_2.z)max_2.z=OABB2[i].z;
    };

    if((min_1.x<max_2.x)&&(min_2.x<max_1.x))
    if((min_1.y<max_2.y)&&(min_2.y<max_1.y))
    if((min_1.z<max_2.z)&&(min_2.z<max_1.z))
        return true;

    return false;
}

bool PhysCollidePSMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision* Collision)
{
    SurePhysCollision TotalCollision;
    int TotalCollisions = 0;
    for(int ip = 0;ip<o1->ParticlesCounter;++ip){
        SureObject LocalSphere;
        LocalSphere.X = RelativeCoordinatesToGlobal(o1,o1->Particles[ip].X);
        LocalSphere.lx = o1->Particles[ip].Size;
        if(PhysCollideSphereMesh(&LocalSphere,o2,EngineData,Collision)){
            o1->Particles[ip].X += GlobalVectorToRelative(o1,Collision->CollisionVector) * Collision->CollisionLength * 0.3f;
            TotalCollision.CollisionLength += Collision->CollisionLength*o1->mass;
            TotalCollision.CollisionPoint += Collision->CollisionPoint;
            TotalCollision.CollisionVector += Collision->CollisionVector;
            ++TotalCollisions;
        };
    };
    if(TotalCollisions>0){
        Collision->Object1 = o1;
        Collision->Object2 = o2;
        Collision->CollisionLength = TotalCollision.CollisionLength * (1.0 / (double)TotalCollisions);
        Collision->CollisionPoint = TotalCollision.CollisionPoint * (1.0 / (double)TotalCollisions);
        Collision->CollisionVector = __NORMALIZE(TotalCollision.CollisionVector);
        return true;
    };
    return false;
}

bool PhysCollideMeshMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision* Collision)
{
    bool CollisionFound = false;
    SureGJK LocalGJK;
    LocalGJK.SetEngine(EngineData);
    LocalGJK.SetupMinkowski(o1,o2);
    LocalGJK.InitiateLoop();
    while(!LocalGJK.LoopExit()){
        if(!LocalGJK.TIContains00()){
            LocalGJK.LoopNextStep();
        }else{
            LocalGJK.ExitLoop_Collision();
            LocalGJK.ExpandCoverLoop();
            if(LocalGJK.CollisionFound()){
                *Collision = LocalGJK.Collision;
                CollisionFound = true;
            };
        };  // тэттраэдр содержит 0,0
    };// while(!exit)
    LocalGJK.Clear();
    return CollisionFound;
}

void SureMatrixToImage(float* rgbmatrix,QImage* image,int mx,int my){
    int mx_scaled = mx/SURE_SCALE;
    int my_scaled = my/SURE_SCALE;
    float lv_max = 0;
    float lv_med = 0;
    for(int y=0;y<my_scaled;++y)
    for(int x=0;x<mx_scaled;++x){
        int k = y*SURE_MAXRES_X*3+x*3;
        lv_max = rgbmatrix[k] > lv_max ? rgbmatrix[k] : lv_max;
        lv_max = rgbmatrix[k+1] > lv_max ? rgbmatrix[k+1] : lv_max;
        lv_max = rgbmatrix[k+2] > lv_max ? rgbmatrix[k+2] : lv_max;
        lv_med += rgbmatrix[k] + rgbmatrix[k+1] + rgbmatrix[k+2];
    };
    lv_med /= mx_scaled*my_scaled*3;
    lv_max /= (lv_max/lv_med)*150.0;

    uint32_t *l_img = (uint32_t*)image->bits();
    #pragma omp parallel for schedule(static,8)
    for(int y=0;y<my_scaled;++y)
    for(int x=0;x<mx_scaled;++x){
        int k = y*SURE_MAXRES_X*3+x*3;
        float CurrentRGB[3] = {rgbmatrix[k]/lv_max,rgbmatrix[k+1]/lv_max,rgbmatrix[k+2]/lv_max};
        if(x<(mx_scaled-1)){k = y*SURE_MAXRES_X*3+(x+1)*3;}else{k = y*SURE_MAXRES_X*3+x*3;};
        float NextRGB_X[3] = {rgbmatrix[k]/lv_max,rgbmatrix[k+1]/lv_max,rgbmatrix[k+2]/lv_max};
        if(y<(my_scaled-1)){k = (y+1)*SURE_MAXRES_X*3+x*3;}else{k = y*SURE_MAXRES_X*3+x*3;};
        float NextRGB_Y[3] = {rgbmatrix[k]/lv_max,rgbmatrix[k+1]/lv_max,rgbmatrix[k+2]/lv_max};
        if((x<(mx_scaled-1))&&(y<(my_scaled-1))){k = (y+1)*SURE_MAXRES_X*3+(x+1)*3;}else{k = y*SURE_MAXRES_X*3+x*3;};
        float NextRGB_XY[3] = {rgbmatrix[k]/lv_max,rgbmatrix[k+1]/lv_max,rgbmatrix[k+2]/lv_max};
        for(int sy = 0;sy<SURE_SCALE;++sy)
        for(int sx = 0;sx<SURE_SCALE;++sx){
            float SetRGB[3] = {  (CurrentRGB[0]*(float)(SURE_SCALE-sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_X[0]*(float)(sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_Y[0]*(float)(SURE_SCALE-sx)*(float)(sy)
                                + NextRGB_XY[0]*(float)(sx)*(float)(sy)) / (SURE_SCALE*SURE_SCALE), //+2*SURE_SCALE
                                 (CurrentRGB[1]*(float)(SURE_SCALE-sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_X[1]*(float)(sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_Y[1]*(float)(SURE_SCALE-sx)*(float)(sy)
                                + NextRGB_XY[1]*(float)(sx)*(float)(sy)) / (SURE_SCALE*SURE_SCALE),
                                 (CurrentRGB[2]*(float)(SURE_SCALE-sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_X[2]*(float)(sx)*(float)(SURE_SCALE-sy)
                                + NextRGB_Y[2]*(float)(SURE_SCALE-sx)*(float)(sy)
                                + NextRGB_XY[2]*(float)(sx)*(float)(sy)) / (SURE_SCALE*SURE_SCALE)};
            if(SetRGB[0]>255){SetRGB[0]=255;}if(SetRGB[0]<0){SetRGB[0]=0;};
            if(SetRGB[1]>255){SetRGB[1]=255;}if(SetRGB[1]<0){SetRGB[1]=0;};
            if(SetRGB[2]>255){SetRGB[2]=255;}if(SetRGB[2]<0){SetRGB[2]=0;};
            uchar *SetPixel = (uchar*)(&l_img[(y*SURE_SCALE+sy)*mx+(x*SURE_SCALE+sx)]);
            *SetPixel = (uchar)SetRGB[2];++SetPixel;
            *SetPixel = (uchar)SetRGB[1];++SetPixel;
            *SetPixel = (uchar)SetRGB[0];++SetPixel;
            *SetPixel = 255;
        };//Scale
    };// for x y
}
