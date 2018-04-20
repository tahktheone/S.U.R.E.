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

void SureGJK::Clear()
{
    free(M);
}

void SureGJK::ClearCover()
{
    delete [] C;
    delete [] C_N;
    delete [] incover;
}

void SureGJK::SetupMinkowski(SureObject *o1,SureObject *o2)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    __VTYPE3 p1,p2; // точки из объектов, для получения разности
    __VTYPE3 gp1,gp2; // точки в глобальных координатах
    Collision.Object1 = o1;
    Collision.Object2 = o2;

    // 1. Составляем разность минковского.
    uint l1_limit = EngineData->ModelsInfo[o1->ModelID_collider].vertex_count;
    uint l1_start = EngineData->ModelsInfo[o1->ModelID_collider].vertex_start;
    uint l2_limit = EngineData->ModelsInfo[o2->ModelID_collider].vertex_count;
    uint l2_start = EngineData->ModelsInfo[o2->ModelID_collider].vertex_start;

    M = (my_double3 *)malloc(sizeof(my_double3)*l1_limit*l2_limit);

    my_double3 M_min = {SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE,SURE_R_MAXDISTANCE};
    my_double3 M_max = {-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE,-SURE_R_MAXDISTANCE};

    // для каждой точки
    for(uint i1 = 0;i1<l1_limit;++i1){
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
            if(M[mc].x<M_min.x)M_min.x=M[mc].x;
            if(M[mc].y<M_min.y)M_min.y=M[mc].y;
            if(M[mc].z<M_min.z)M_min.z=M[mc].z;
            if(M[mc].x>M_max.x)M_max.x=M[mc].x;
            if(M[mc].y>M_max.y)M_max.y=M[mc].y;
            if(M[mc].z>M_max.z)M_max.z=M[mc].z;
            ++mc;
        };// каждая точка с каждой точкой
    };

    exit_no_collision = false;
    exit_collision = false;
    if(M_min.x>0||M_min.y>0||M_min.z>0)
        exit_no_collision = true;
    if(M_max.x<0||M_max.y<0||M_max.z<0)
        exit_no_collision = true;
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
    iter = 0; // подсчет итерраций - гарантия от зацикливаний
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
    C = new uint [mc*4];
    C_N = new uint [mc*4];
    incover = new bool [mc];

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

bool SureGJK::FindFarestMinkowskiByVector(my_double3 Vector,uint *e_result)
{
    // ищем дальнюю точку в M в направлении v
    __VTYPE md = dot(Vector,M[C[CollisionFace*3+0]])+SURE_R_DELTA;
    __VTYPE ld;
    bool f = false;
    uint fndi = 0;
    for(uint li = 0;li<mc;++li){// для каждой точки M[i]
        if(!incover[li]){ // если точка не в оболочке уже
            ld = dot(Vector,M[li]);
            if(ld>md){
                md = ld;
                fndi = li;
                f = true;
            };
        }; // если точка не в оболочке уже
    }; // для каждой точки M[i]
    *e_result = fndi;
    return f;
}

bool SureGJK::IsCoverFaceVisibleByPoint(uint f,uint point)
{
    return (dot(cross(M[C[f*3+1]]-M[C[f*3+0]],
                      M[C[f*3+2]]-M[C[f*3+0]]),
                M[point]-M[C[f*3+0]])
            > SURE_R_DELTA);
}

void SureGJK::CheckAndAddFaceToNewCover(uint p1, uint p2, uint p3)
{
// Логика check_cover:
// для всех точек в M[] проверяем, лежат ли они снаружи
// проверяемой грани. Если все внутри --
// добавляем грань в COVER
    bool out = false;
    for(uint cci = 0;cci<mc;++cci)
        if(incover[cci])
            if(dot(cross(M[p2]-M[p1],M[p3]-M[p1]),M[cci]-M[p1])>SURE_R_DELTA)
                out = true;
    if(!out){
        C_N[cc_n*3+0] = p1;
        C_N[cc_n*3+1] = p2;
        C_N[cc_n*3+2] = p3;
        ++cc_n;
    };
}

void SureGJK::AddFaceToNewCover(uint p1, uint p2, uint p3)
{
    C_N[cc_n*3+0] = p1;
    C_N[cc_n*3+1] = p2;
    C_N[cc_n*3+2] = p3;
    ++cc_n;
}

void SureGJK::CheckAndAddFaceToNewCover(uint face,uint fndi)
{
    CheckAndAddFaceToNewCover(C[face*3+0],C[face*3+1],fndi);
    CheckAndAddFaceToNewCover(C[face*3+1],C[face*3+2],fndi);
    CheckAndAddFaceToNewCover(C[face*3+2],C[face*3+0],fndi);
}

void SureGJK::AddFaceToNewCover(uint face)
{
    AddFaceToNewCover(C[face*3+0],C[face*3+1],C[face*3+2]);
}

void SureGJK::ExpandCover(uint fndi)
{
    SetIncover(fndi);
    for(uint aci = 0;aci<cc;++aci)
    { // для каждой грани cover
        // грань видимая?
        if(IsCoverFaceVisibleByPoint(aci,fndi))
        { // грань видимая
            CheckAndAddFaceToNewCover(aci,fndi);
        }else{ // грань невидимая - оставляем в cover
            AddFaceToNewCover(aci);
        }; // грань видимая?
    };// для каждой грани cover
}

void SureGJK::SetCollisionByCover()
{
    //Ищем грань ближайшую к 0 0.
    Collision.CollisionLength = SURE_R_MAXDISTANCE;
    CollisionFace = 0;
    for(uint ci=0;ci<cc;++ci)
    { // для каждой грани cover
    // (помним что для всех граней 0,0 внутри)
        __VTYPE L = fabs(dot(GetCoverFaceNormal(ci),-M[C[ci*3+0]]));
        // расстояние до 0,0
        if(L<Collision.CollisionLength)
        {
            Collision.CollisionLength = L;
            CollisionFace = ci;
        }; // (L<LM)
    }; // для каждой грани cover
    Collision.CollisionVector = GetCoverFaceNormal(CollisionFace);
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

uint SureGJK::GetFarestVertexByObj(SureObject *o,my_double3 i_vector)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    uint l_start = EngineData->ModelsInfo[o->ModelID_collider].vertex_start;
    uint l_limit = l_start + EngineData->ModelsInfo[o->ModelID_collider].vertex_count;
    uint result = 0;
    double LM = -SURE_R_MAXDISTANCE;
    for(uint i = l_start;i<l_limit;++i){
        my_double3 LocalVertex;
        __GET_VERTEX(LocalVertex,i);
        double LC = dot(LocalVertex,i_vector);
        if(LC>LM){
            LM = LC;
            result = i;
        };
    };
    return result;
}

my_double3 SureGJK::GetCollisionPointByObject(SureObject *o,my_double3 i_gvector,double *dist)
{
    cl_int* MeshCLImg = EngineData->MeshCLImg;
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов

    my_double3 i_vector;
    i_vector.x = dot(i_gvector,o->ox)/o->lx;
    i_vector.y = dot(i_gvector,o->oy)/o->ly;
    i_vector.z = dot(i_gvector,o->oz)/o->lz;
    i_vector = __NORMALIZE(i_vector);

    uint FarestVertexIndex = GetFarestVertexByObj(o,i_vector);
    my_double3 FarestVertex;
    __GET_VERTEX(FarestVertex,FarestVertexIndex);

    my_double3 LocalCollisionFace[3];
    uint VertexesInFace = 1;
    LocalCollisionFace[0] = FarestVertex;

    my_double3 CumulatedVertex = FarestVertex;
    double FoundVertexes = 1.0;

    my_double3 Minimum = FarestVertex;
    my_double3 Maximum = FarestVertex;


    uint lv_start = EngineData->ModelsInfo[o->ModelID_collider].vertex_start;
    uint lv_limit = lv_start + EngineData->ModelsInfo[o->ModelID_collider].vertex_count;
    uint l_start = EngineData->ModelsInfo[o->ModelID_collider].mesh_start;
    uint l_limit = l_start + EngineData->ModelsInfo[o->ModelID_collider].mesh_count;

    for(uint vi = lv_start;vi<lv_limit;++vi){ // Для каждого vertex'а
        bool CommonMeshExist = false;
        for(uint mi = l_start;mi<l_limit;++mi){
            __SURE_VINT4 mesh;
            __GET_MESH(mesh,mi);
            uint m1 = mesh.x;
            uint m2 = mesh.y;
            uint m3 = mesh.z;
            if((m1==FarestVertexIndex||m2==FarestVertexIndex||m3==FarestVertexIndex)
               &&(m1==vi||m2==vi||m3==vi)){
                CommonMeshExist = true;
            };
        };
        if(CommonMeshExist){ // Есть общая грань с дальним vertex'ом
            my_double3 LocalVertex;
            __GET_VERTEX(LocalVertex,vi);
            // строим грань
            my_double3 Face = __NORMALIZE(FarestVertex - LocalVertex);
            // cross(грань,i_vector) > 0.99 ?
            if(__LENGTH(cross(Face,i_vector)) > 0.999999){
                // добавляем vertex к расчету средней точки
                CumulatedVertex = CumulatedVertex + LocalVertex;
                FoundVertexes = FoundVertexes + 1.0;
                // добавляем в min, max
                if(LocalVertex.x<Minimum.x)
                    Minimum.x = LocalVertex.x;
                if(LocalVertex.y<Minimum.y)
                    Minimum.y = LocalVertex.y;
                if(LocalVertex.z<Minimum.z)
                    Minimum.z = LocalVertex.z;

                if(LocalVertex.x>Maximum.x)
                    Maximum.x = LocalVertex.x;
                if(LocalVertex.y>Maximum.y)
                    Maximum.y = LocalVertex.y;
                if(LocalVertex.z>Maximum.z)
                    Maximum.z = LocalVertex.z;

                if(VertexesInFace<3){
                    LocalCollisionFace[VertexesInFace] = LocalVertex;
                    ++VertexesInFace;
                };//if(VertexesInFace<3)
            }; //if(__LENGTH(cross(Face,i_vector)) > 0.99)
        };// if(CommonMeshExist;
    }; // Для каждого vertex'а

    // средняя точка = CumulatedVertex/FoundVertexes
    my_double3 result = CumulatedVertex * ( 1.0 / FoundVertexes );

    if(VertexesInFace==3){
        my_double3 CorrectionNormal = __NORMALIZE(cross(LocalCollisionFace[1]-LocalCollisionFace[0],LocalCollisionFace[2]-LocalCollisionFace[0]));
        my_double3 GlobalCorrectionNormal = __NORMALIZE(CorrectionNormal.x * o->ox * o->lx +
                                                        CorrectionNormal.y * o->oy * o->ly +
                                                        CorrectionNormal.z * o->oz * o->lz);
        double Projection = dot(GlobalCorrectionNormal,Collision.CollisionVector);
        if(Projection>0){
            Collision.CollisionVector = GlobalCorrectionNormal;
        }else{
            Collision.CollisionVector = -GlobalCorrectionNormal;
        };
        Collision.CollisionLength = Collision.CollisionLength * fabs(Projection);
    };

    // dist_v = max - min.
    // dist_v в глобальниые координаты
    my_double3 DistVector = Maximum - Minimum;
    if(DistVector.x>0||DistVector.y>0||DistVector.z>0){
        my_double3 GlobalDistVector = DistVector.x * o->ox * o->lx +
                                      DistVector.y * o->oy * o->ly +
                                      DistVector.z * o->oz * o->lz;
        *dist = __LENGTH(GlobalDistVector);
    }else{
        *dist = 0;
    };

    result = o->X +  result.x * o->ox * o->lx +
         result.y * o->oy * o->ly +
         result.z * o->oz * o->lz;
    return result;
}

void SureGJK::SwitchCover()
{
    for(uint icn = 0;icn<cc_n;++icn){
        C[icn*3+0] = C_N[icn*3+0];
        C[icn*3+1] = C_N[icn*3+1];
        C[icn*3+2] = C_N[icn*3+2];
    };
    cc = cc_n;
}

void SureGJK::SetIncover(uint i)
{
    incover[i] = true;
}

void SureGJK::ClearNewCover()
{
    cc_n = 0;
}

my_double3 SureGJK::GetCoverFaceNormal(uint face)
{
    return __NORMALIZE(cross(M[C[face*3+1]]-M[C[face*3+0]],M[C[face*3+2]]-M[C[face*3+0]]));
}

void SureGJK::ExpandCoverLoop()
{
    InitiateCoverLoop();
    while(!CoverExpanded()){
        ClearNewCover();
        SetCollisionByCover();
        uint fndi = 0;
        if(FindFarestMinkowskiByVector(Collision.CollisionVector,&fndi)){
            // cover нужно расширить добавив точку M[fndi]
            ExpandCover(fndi);
            SwitchCover();
            CheckCoverIterration();
        }else{
            SetCoverExpanded();
        }; // поиск расширения для cover
    }; // while (!cover_expanded
    SetCollisionPoint();
    ClearCover();
}

void SureGJK::SetCollisionPoint()
{
    double dist1;
    double dist2;
    my_double3 CollisionPoint1 = GetCollisionPointByObject(Collision.Object1,-Collision.CollisionVector,&dist1);
    my_double3 CollisionPoint2 = GetCollisionPointByObject(Collision.Object2,Collision.CollisionVector,&dist2);
    if(dist1<dist2){
        Collision.CollisionPoint = CollisionPoint1;
    }else{
        Collision.CollisionPoint = CollisionPoint2;
    };
}
