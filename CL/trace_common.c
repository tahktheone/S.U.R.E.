    // ускоряем работу с памятью на GPU: забираем информацию о камере в локальную память:
    __SURE_STRUCT SureCameraInfo CameraInfo = GPUData->CameraInfo;

    // drawable, с которым встретился луч (для хранения параметров изменения траектории)
    __SURE_STRUCT SureDrawable DrawableCollided;

    //__SURE_GLOBAL __SURE_STRUCT SureDrawable *lv_cur; // drawable в котором находимся на начало шага трассировки
    //__SURE_GLOBAL __SURE_STRUCT SureDrawable *cur; // drawable в котором находимся на конец шага трассировки
    //__SURE_GLOBAL __SURE_STRUCT SureDrawable *col; // drwable который обнаружен на пути трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *DrawableIter; // drawable который сейчас анализируется
    // Это нужно, чтобы корректно отслеживать переход луча между средами разной плотности:
    // (для корректного отражения преломления, например, в стеклянном шаре в воде)
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *DrawableCurrent;
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *DrawableNext;

    //Основные переменные с информацией о трассировке
    __VTYPE  TraceLength = 0; // общий путь трассировки -- до SURE_R_MAXLENGTH
    __VTYPE3 TraceFade   = {1.0,1.0,1.0};
    __VTYPE3 TraceColor  = {0,0,0};

    // Инициализируем рандом
    uint r = InitRandom(&x,&y);

    __VTYPE3 TracePoint = CameraInfo.cam_x;
    __VTYPE3 TraceVector;
    if(CameraInfo.subp_rnd){
        TraceVector = DetermineTraceVectorSAA(x,y,&CameraInfo,Randomf,&r);
    }else{
        TraceVector = DetermineTraceVector(x,y,&CameraInfo);
    };

    //cur = &Drawables[0];

    uint MaximumIterrations = GPUData->r_maxiters;
    #if SURE_RLEVEL>20
    uint MaximumRechecks = GPUData->r_rechecks;
    #endif
    #if SURE_RLEVEL<70
        MaximumIterrations = 4;
    #endif // SURE_RLEVEL

    #ifdef __LOGGING
        SureTraceLog *TraceLog = &EngineData->TraceLogs[EngineData->TraceLogsCount];
        TraceLog->ItemsCount = 0;
    #endif // __LOGGING

    for(uint Iterration = 0;Iterration<MaximumIterrations;Iterration++)
    {

        if(TraceLength > SURE_R_MAXDISTANCE)continue;
        if((TraceFade.x+TraceFade.y+TraceFade.z) < SURE_R_FADELIMIT)continue;

        DrawableCurrent = &Drawables[0];// Объект внутри которого находится луч трассировки
        DrawableNext = &Drawables[0]; // Объект, внутри которого находится объект DrawableCurrent

        // параметры пересечения луча с объектом
        __VTYPE3 collision_point;
        __VTYPE3 collision_normal;
        __VTYPE intersect_dist = SURE_R_MAXDISTANCE;
        bool collision_found = false;
        #if SURE_RLEVEL>90
        bool collision_inner = false;
        #endif

        #ifdef __SELECT_OBJECT
            int FoundDrawable = -1;
        #endif // __SELECT_OBJECT

        for(int DrawablesIndex=1;DrawablesIndex<GPUData->m_drawables;++DrawablesIndex)
        {
            #ifdef __LOGGING
                TraceLog->Items[TraceLog->ItemsCount].TraceVector = TraceVector;
                TraceLog->Items[TraceLog->ItemsCount].TracePoint = TracePoint;
            #endif // __LOGGING
            DrawableIter = &Drawables[DrawablesIndex];
            switch(DrawableIter->type)
            {
                case SURE_DR_SPHERE:
                {
                    bool InSphere = false;

                    __VTYPE3 VectorToCenter = TracePoint-DrawableIter->X;
                    __VTYPE B = dot(VectorToCenter,TraceVector);
                    __VTYPE C = __MAD(-DrawableIter->lx,DrawableIter->lx,dot(VectorToCenter,VectorToCenter));
                    __VTYPE D = __MAD(B,B,-C);
                    __VTYPE2 t;

                    if(D<0.0f)break;
                        __VTYPE kd = __SQRT(D);
                        __VTYPE t1 = -B+kd;
                        __VTYPE t2 = -B-kd;
                        t.x = __SURE_MIN(t1,t2);
                        t.y = __SURE_MAX(t1,t2);

                        if((t.x<SURE_R_DELTA)&&(t.y>SURE_R_DELTA)){ // проверка на нахождение в сфере
                            // если на входе в сферу - граница вклчена
                            // если на выходе - не включена
                            SET_CURRENT;
                        };

                        if(t.x>intersect_dist) break; // отсеиваем случаи когда ближайшая точка сферы дальше чем текущий intersect_dist
                        if(t.y<SURE_R_DELTA) break; // отсеиваем случаи когда сфера "сзади"
                        InSphere = t.x<SURE_R_DELTA ? true : false; // определяем мы внутри или снаружи
                        if(InSphere){
                            if(t.y<SURE_R_DELTA)break;
                            if(t.y>intersect_dist)break;
                            intersect_dist = t.y;//-SURE_R_DELTA_GPU_FIX;
                        }else{
                            intersect_dist = t.x;//-SURE_R_DELTA_GPU_FIX;
                        };
                       if(DrawableIter->sided||!InSphere)
                       {
                           SET_COLLISION_OUTSIDE;
                       }else{
                           SET_COLLISION_INSIDE
                       };

                        #ifdef __SELECT_OBJECT
                            FoundDrawable = DrawablesIndex;
                        #endif
                        collision_point = __MAD(intersect_dist,TraceVector,TracePoint);

                        // Корректируем точку пересечения -- помещаем ее ровно на шар:
                        collision_point = __MAD(DrawableIter->lx,__NORMALIZE(collision_point-DrawableIter->X),DrawableIter->X);

                        if((DrawableIter->map_id>=0)||(DrawableIter->advmap_id>=0)){
                            __VTYPE3 UVCoordinates;
                            GET_SPHERICAL_UV_COORDINATES(UVCoordinates);
                            UVCoordinates.x *= SURE_R_TEXRES;
                            UVCoordinates.y *= SURE_R_TEXRES;
                            if(DrawableIter->map_id>=0)
                            {
                                __GET_TEXTURE(UVCoordinates.x,
                                              UVCoordinates.y,
                                              DrawableIter->map_id);
                            };
                            if(DrawableIter->advmap_id>=0)
                            {
                                __GET_ADVMAP(UVCoordinates.x,
                                             UVCoordinates.y,
                                             DrawableIter->advmap_id);
                            };
                        }; // if((DrawableIter->map_id>=0)||(DrawableIter->advmap_id>=0))
                        if(InSphere)
                        {
                            collision_normal = __NORMALIZE(DrawableIter->X - collision_point);
                        }else{
                            collision_normal = __NORMALIZE(collision_point - DrawableIter->X);
                        };
                    break;
                }; // Сфера

                case SURE_DR_SQUARE:
                {
                    // Расстояние от точки старта луча до плоскости
                    __VTYPE3 VectorToDrawableCenter = TracePoint-DrawableIter->X;
                    __VTYPE DistanceToPlane = dot(DrawableIter->oz,VectorToDrawableCenter);
                    // Проекция вектора трассировки на нормаль
                    __VTYPE TraceVecByNormal = dot(DrawableIter->oz,TraceVector);

                    // Условия нахождения "внутри" объекта:
                    // 1. координаты проекции точки трассировки на плоскость - внутри квадрата.
                    // 2.1 ИЛИ расстояние до плоскости < дельты И вектор трассировки неколинеарен нормали
                    // 2.2 ИЛИ расстояние до плоскости <-дельты И вектор трассировки колинеарен нормали
                    // 3. Расстояние до плоскости < дельты и >-lz

                    if((DistanceToPlane<SURE_R_DELTA)&&
                       (DistanceToPlane>(-DrawableIter->lz))){
                        if((TraceVecByNormal<0.0f)||(DistanceToPlane<-SURE_R_DELTA)){
                            __VTYPE2 Coordinates = {dot(DrawableIter->ox,VectorToDrawableCenter),
                                                    dot(DrawableIter->oy,VectorToDrawableCenter)};
                            // Нет попадания в квадрат -- игнорируем
                            if( Coordinates.x <  DrawableIter->lx&&
                                Coordinates.x > -DrawableIter->lx&&
                                Coordinates.y <  DrawableIter->ly&&
                                Coordinates.y > -DrawableIter->ly){
                                SET_CURRENT;
                            }; // попадание в координаты
                        }; // проверки на границе с плоскостью
                    }; // точка трассировки под плоскостью

                    // Если точка старта луча вплотную к плоскости -- игнорируем ее
                    // (чтобы отраженый от плоскости луч из-за округлений не сталкивался с той же плоскостью):
                    if(DistanceToPlane<SURE_R_DELTA&&DistanceToPlane>-SURE_R_DELTA)break;
                    // Расстояние до точки пересечения луча с плоскостью
                    __VTYPE TraceDistance = - __DIVIDE(DistanceToPlane,TraceVecByNormal);
                    // Если дальше чем уже найденное пересечение -- выходим
                    if(TraceDistance>intersect_dist)break;
                    // Если плоскость сзади, за стартом луча -- выходим.
                    if(TraceDistance<SURE_R_DELTA)break;
                    // Точка пересечения луча с плоскостью
                    __VTYPE3 CollisionPointCandidate = __MAD(TraceDistance,TraceVector,TracePoint);
                    // Вектор От чентра объекта (внутренние координаты 0 0) к точке пересечения
                    __VTYPE3 LocalVectorToTracePoint = DrawableIter->X - CollisionPointCandidate;
                    // Локальные координаты точки пересечения
                    __VTYPE2 LocalCoordinates = {dot(DrawableIter->ox,LocalVectorToTracePoint),
                                                 dot(DrawableIter->oy,LocalVectorToTracePoint)};
                    // Нет попадания в квадрат -- игнорируем
                    if(LocalCoordinates.x >  DrawableIter->lx||
                       LocalCoordinates.x < -DrawableIter->lx||
                       LocalCoordinates.y >  DrawableIter->ly||
                       LocalCoordinates.y < -DrawableIter->ly)break;
                    // Луч гарантированно столкнулся с квадратом.
                    // Обрабатываем столкновение
                    intersect_dist = TraceDistance;

                    // Корректируем точку пересечения:
                    // смешаем ее к плоскости (вдоль нормали) на расстояние от нее до плоскости
                    // (проекция на нормаль вектора от точки на плоскости до точки пересечения)
                    collision_point = __MAD(-DrawableIter->oz,dot(CollisionPointCandidate-DrawableIter->X,DrawableIter->oz),CollisionPointCandidate);

                    if(TraceVecByNormal>0.0f)
                    {  // с внутренней стороны
                        if(DrawableIter->sided)
                        { // материал двухсторонний
                            SET_COLLISION_OUTSIDE;
                        }else{  // материал односторонний
                            SET_COLLISION_INSIDE;
                        };
                        #ifdef __SELECT_OBJECT
                            FoundDrawable = DrawablesIndex;
                        #endif
                        GET_TEXTURE_SQUARE;
                        collision_normal = -DrawableIter->oz;
                    }else{ // с внешней стороны
                        SET_COLLISION_OUTSIDE;
                        #ifdef __SELECT_OBJECT
                            FoundDrawable = DrawablesIndex;
                        #endif
                        GET_TEXTURE_SQUARE;
                        collision_normal = DrawableIter->oz;
                    };
                    break;
                }; // Квадрат
                case SURE_DR_MESH:
                {

                    __VTYPE3 LocalTraceVector;
                    LocalTraceVector.x = dot(TraceVector,DrawableIter->ox);
                    LocalTraceVector.y = dot(TraceVector,DrawableIter->oy);
                    LocalTraceVector.z = dot(TraceVector,DrawableIter->oz);

                    __VTYPE3 LocalTracePoint;
                    __VTYPE3 VectorToTracePoint = TracePoint-DrawableIter->X;
                    LocalTracePoint.x = dot(VectorToTracePoint,DrawableIter->ox);
                    LocalTracePoint.y = dot(VectorToTracePoint,DrawableIter->oy);
                    LocalTracePoint.z = dot(VectorToTracePoint,DrawableIter->oz);

                    // Проверка на пересечение луча с AABB (в локальных координатах)
                    // AABB - Axis Aligned Bounding Box

                    __VTYPE AABB_FarestIn = -SURE_R_MAXDISTANCE;
                    __VTYPE AABB_NearestOut = SURE_R_MAXDISTANCE;
                    __VTYPE DistanceToSide = 0;

                    AABB_CHECK_AXIS(x,lx);
                    AABB_CHECK_AXIS(y,ly);
                    AABB_CHECK_AXIS(z,lz);

                    if(AABB_FarestIn>AABB_NearestOut||AABB_NearestOut<SURE_R_DELTA)break;

                    // Приводим размеры вектора трассировки к размерам объекта
                    // Потому что все координаты Vertex'ов в mesh'ах - от -1.0 до 1.0
                    // И мы не хотим переводить их все в глобальные координаты
                    __VTYPE3 LocalSizeMultiplier = { __INV(DrawableIter->lx),
                                                     __INV(DrawableIter->ly),
                                                     __INV(DrawableIter->lz)};
                    LocalTraceVector.x*=LocalSizeMultiplier.x;
                    LocalTraceVector.y*=LocalSizeMultiplier.y;
                    LocalTraceVector.z*=LocalSizeMultiplier.z;
                    LocalTracePoint.x*=LocalSizeMultiplier.x;
                    LocalTracePoint.y*=LocalSizeMultiplier.y;
                    LocalTracePoint.z*=LocalSizeMultiplier.z;

                    // Коэффициент показывающий отношение размера объекта к размерам мира
                    // Нужен для пересчета длины вектора трассировки
                    __VTYPE ResizingKoeff = __INV(__LENGTH(LocalTraceVector));
                    LocalTraceVector = __NORMALIZE(LocalTraceVector);

                    // SURE_R_DELTA тоже должна подгоняться под локальные размеры
                    // чтобы избежать искажений в мелких деталях
                    __VTYPE LocalRenderDelta = SURE_R_DELTA*ResizingKoeff;

                    // для определения того, находимся ли мы "внутри"
                    __VTYPE FirstIn = SURE_R_MAXDISTANCE;
                    __VTYPE FirstOut = SURE_R_MAXDISTANCE;

                    for(uint MeshIndex = 0;MeshIndex<DrawableIter->mesh_count;++MeshIndex)
                    { // Для каждой meshины

                        // переменные для алгоритма поиска пересечения луча и треугольника
                        __VTYPE3 pvec; __VTYPE3 tvec; __VTYPE det; __VTYPE inv_det;
                        __VTYPE u; __VTYPE v; __VTYPE3 qvec; __VTYPE TraceDistance;

                        uint CurrentMesh = DrawableIter->mesh_start + MeshIndex;
                        __SURE_VINT4 MeshData;
                        __VTYPE3 LocalVertex1;
                        __VTYPE3 LocalVertex2;
                        __VTYPE3 LocalVertex3;
                        __GET_MESH(MeshData,CurrentMesh);
                        __GET_VERTEX(LocalVertex1,MeshData.x);
                        __GET_VERTEX(LocalVertex2,MeshData.y);
                        __GET_VERTEX(LocalVertex3,MeshData.z);

                        // Алгоритм Моллера — Трумбора
                        // in < LocalVertex1-3
                        // in < LocalTracePoint
                        // in < LocalTraceVector
                        // Если нет пересечения - continue
                        // out > TaraceDistance
                        // out > u
                        // out > v
                        SURE_MOLLER_TRUMBOR;
                        // Я не стал разбираться как он работает.
                        // Главное -- он работает, и говорят, что это самый быстрый вариант решения
                        // для поиска пересечения луча и треугольника.

                        //__VTYPE GlobalTraceDistance = TraceDistance * ResizingKoeff;

                        // Сзади или слишком близко -- выходим:
                        if(TraceDistance<LocalRenderDelta)continue;

                        __VTYPE3 LocalTrueNormal = __NORMALIZE(cross(LocalVertex2-LocalVertex1,LocalVertex3-LocalVertex1));

                        bool CollisionIsInner = (dot(LocalTraceVector,LocalTrueNormal)>0.0f);

                        if(CollisionIsInner){
                            if(TraceDistance<FirstOut)FirstOut=TraceDistance;
                        }else{
                            if(TraceDistance<FirstIn)FirstIn=TraceDistance;
                        };

                        // Дальше чем уже найденная точка пересечения
                        if(TraceDistance>intersect_dist)continue;

                        // точка пересечения
                        __VTYPE3 LocalCollisionPoint = __MAD(LocalTraceVector,__DIVIDE(TraceDistance,ResizingKoeff),LocalTracePoint);

                        // читаем нормали по vertex'ам
                        __VTYPE3 n1; __VTYPE3 n2; __VTYPE3 n3;
                        __GET_NORMALS_INDEX(CurrentMesh);
                        __GET_NORMALS_BYINDEX(n1,n2,n3);

                        // Нормаль в точке -- усредненная по нормалям на vertex'ах
                        // с учетом uv-координат (которые вернул алгоритм поиска пересечения)
                        __VTYPE3 LocalNormal = __MAD(n1,(1.0f-u-v),__MAD(n2,u,n3*v));
                        LocalNormal = __NORMALIZE(LocalNormal);

                        // переводим нормаль в глобальные координаты
                        collision_normal = __NORMALIZE(__MAD(LocalNormal.z*DrawableIter->oz,__INV(DrawableIter->lz),
                                                             __MAD(LocalNormal.y*DrawableIter->oy,__INV(DrawableIter->ly),
                                                                   LocalNormal.x*DrawableIter->ox*__INV(DrawableIter->lx))));

                        // Корректируем точку пересечения:
                        // смешаем ее к плоскости (вдоль нормали) на расстояние от нее до плоскости
                        // (проекция на нормаль вектора от точки на плоскости до точки пересечения)
                        //LocalCollisionPoint = __MAD(-LocalTrueNormal,dot(LocalCollisionPoint-LocalVertex1,LocalTrueNormal),LocalCollisionPoint);
                        collision_point = __MAD(LocalCollisionPoint.z*DrawableIter->oz,DrawableIter->lz,
                                                __MAD(LocalCollisionPoint.y*DrawableIter->oy,DrawableIter->ly,
                                                      __MAD(LocalCollisionPoint.x*DrawableIter->ox,DrawableIter->lx,
                                                             DrawableIter->X)));

                        if(CollisionIsInner)
                        { // с внутренней стороны
                            if(DrawableIter->sided)
                            { // материал двухсторонний
                                SET_COLLISION_OUTSIDE;
                            }else{  // материал односторонний
                                SET_COLLISION_INSIDE;
                            };
                            collision_normal = -collision_normal;
                            SET_COLLISION_MESH;
                        }else{ // с внешней стороны
                            SET_COLLISION_OUTSIDE
                            SET_COLLISION_MESH;
                        };
                        #ifdef __SELECT_OBJECT
                            FoundDrawable = DrawablesIndex;
                        #endif
                    }; // Для каждой meshины
                    // определяем, не находится ли точка трассировки внутри mesh
                    if(FirstOut<FirstIn){
                        SET_CURRENT;
                    };
                    break;
                }; // Mesh
                default:
                    break;
            }; // switch(DrawableIter->type)
        };// Цикл по Drawables

        // Тут мы прошлись по всем Drawables и нашли (или нет) с чем пересекается луч.
        #if SURE_RLEVEL>90
        // Добавлем рассеивание средой
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rr = Randomf[r]*SURE_R_MAXDISTANCE*DrawableCurrent->transp_i;
        if(rr<intersect_dist){
            DrawableIter = DrawableCurrent;
            SET_COLLISION_OUTSIDE;
            intersect_dist = rr;
            collision_normal = -TraceVector;
            collision_point = __MAD(intersect_dist,TraceVector,TracePoint);
            collision_inner = true;
        };
        #endif // SURE_RLEVEL>90

        #ifdef __LOGGING
            TraceLog->Items[TraceLog->ItemsCount].CollisionNomal = collision_normal;
            TraceLog->Items[TraceLog->ItemsCount].IntersectDistance = intersect_dist;
            TraceLog->Items[TraceLog->ItemsCount].iter = Iterration;
            TraceLog->Items[TraceLog->ItemsCount].transp_i = DrawableCurrent->transp_i;
            TraceLog->Items[TraceLog->ItemsCount].rgb_current = DrawableCurrent->rgb;
        #endif // __LOGGING

         // луч улетел в никуда -- выходим из цикла
        if(!collision_found)break;

        #ifdef __SELECT_OBJECT
        if((EngineData->SelectedObject<0)&&(FoundDrawable>=0)){
            int FoundID = -1;
            for(int oid = 0;oid<EngineData->m_objects;++oid){
                if(EngineData->objects[oid].DrawableGPUID==FoundDrawable){
                    FoundID = oid;
                };
            };
            EngineData->SelectedObject = FoundID;
        };
        break;
        #endif // __SELECT_OBJECT

        if(DrawableCollided.radiance>0.0f)
        {
            #if SURE_RLEVEL<60
                TraceColor.x += 255.0f*TraceFade.x*DrawableCollided.rgb.__XX;
                TraceColor.y += 255.0f*TraceFade.y*DrawableCollided.rgb.__YY;
                TraceColor.z += 255.0f*TraceFade.z*DrawableCollided.rgb.__ZZ;
            #else
                TraceColor.x += TraceFade.x*DrawableCollided.rgb.__XX;
                TraceColor.y += TraceFade.y*DrawableCollided.rgb.__YY;
                TraceColor.z += TraceFade.z*DrawableCollided.rgb.__ZZ;
            #endif // SURE_RLEVEL<60
            #ifdef __LOGGING
                TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                TraceLog->Items[TraceLog->ItemsCount].Color.x = DrawableCollided.rgb.__XX;
                TraceLog->Items[TraceLog->ItemsCount].Color.y = DrawableCollided.rgb.__YY;
                TraceLog->Items[TraceLog->ItemsCount].Color.z = DrawableCollided.rgb.__ZZ;
                TraceLog->Items[TraceLog->ItemsCount].rechecks = 0;
                ++TraceLog->ItemsCount;
            #endif // __LOGGING
            break;
        };

        #if SURE_RLEVEL>20
        bool NeedToRecheck = true;
        uint RecheckCounter = 0;
        while( NeedToRecheck && ((++RecheckCounter)<MaximumRechecks) ){
            #if SURE_RLEVEL>90
            if(!collision_inner)
            { // Не рассеивание -- отражение/преломление
            #endif // SURE_RLEVEL
                #if SURE_RLEVEL<50
                __VTYPE TransparenceRandomizer = 0.9f;
                #else
                if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                __VTYPE TransparenceRandomizer = Randomf[r];
                #endif
                NeedToRecheck = false;
                if(TransparenceRandomizer<DrawableCollided.transp)
                { // сработала прозрачность -- считаем преломление
                    __VTYPE3 CollisionNormalRandomized;
                    SURE_RANDOMIZE(CollisionNormalRandomized);
                    __VTYPE TraceVecProjection = dot(TraceVector,CollisionNormalRandomized);
                    if( TraceVecProjection > 0.0f){
                            NeedToRecheck = true;
                    }else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        __VTYPE3 ReflectedVector;
                        __VTYPE RefractionAngleSinus = 0.0f;
                        if(DrawableCollided.transp>1.0f)
                        {
                            ReflectedVector = TraceVector;
                        }else{
                            __VTYPE3 A = DrawableCollided.refr*(TraceVector-TraceVecProjection*CollisionNormalRandomized);
                            RefractionAngleSinus = __LENGTH(A);  // Синус угла преломления
                            ReflectedVector = A-__SQRT(1.0f-RefractionAngleSinus*RefractionAngleSinus)*CollisionNormalRandomized;
                        };
                        if((dot(ReflectedVector,collision_normal)>=0.0f||RefractionAngleSinus>1.0f)&&DrawableCollided.transp<1.0f)
                        {   // Oтражение (в т ч внутреннее)
                            // 2 длины проекции вектора трассировки на нормаль
                            // Помимо проверки на коллинеарность используется дальше
                            // как часть вычислений отраженного вектора
                            ReflectedVector = __MAD(-2.0f*TraceVecProjection,CollisionNormalRandomized,TraceVector);
                            if(dot(ReflectedVector,collision_normal)<0.0f){
                                    NeedToRecheck=true;
                            }else{ // результат отражение направлен от плоскости
                                SET_TRACE_POINT_MINUS;
                                TraceVector = __NORMALIZE(ReflectedVector);
                                TraceLength += intersect_dist;
                                TraceFade.x *= __DIVIDE(DrawableCollided.rgb.__XX,255.0f);
                                TraceFade.y *= __DIVIDE(DrawableCollided.rgb.__YY,255.0f);
                                TraceFade.z *= __DIVIDE(DrawableCollided.rgb.__ZZ,255.0f);
                                #ifdef __LOGGING
                                    TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                                    TraceLog->Items[TraceLog->ItemsCount].rechecks = RecheckCounter;
                                    ++TraceLog->ItemsCount;
                                #endif // __LOGGING
                            };// // результат отражение направлен от плоскости
                        }else{ // преломление
                            SET_TRACE_POINT_PLUS;
                            TraceVector = __NORMALIZE(ReflectedVector);
                            collision_normal = -collision_normal;
                            TraceLength += intersect_dist;
                            #if SURE_RLEVEL>90
                            if(DrawableCollided.transp<1.0f)
                            {
                            #endif // SURE_RLEVEL>90
                                TraceFade.x *= __DIVIDE(DrawableCollided.rgb.__XX,255.0f);
                                TraceFade.y *= __DIVIDE(DrawableCollided.rgb.__YY,255.0f);
                                TraceFade.z *= __DIVIDE(DrawableCollided.rgb.__ZZ,255.0f);
                            #if SURE_RLEVEL>90
                            };
                            #endif // SURE_RLEVEL>90
                                #ifdef __LOGGING
                                    TraceLog->Items[TraceLog->ItemsCount].NormalRandomized = CollisionNormalRandomized;
                                    TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                                    TraceLog->Items[TraceLog->ItemsCount].rechecks = RecheckCounter;
                                    ++TraceLog->ItemsCount;
                                #endif // __LOGGING
                        };  // Внутреннее отражение /  преломление
                    };// if(l>0) модифицированная нормаль не коллинеарна вектору трассировки
                }else{ // не сработала прозрачность -- считаем отражение
                    #if SURE_RLEVEL<60
                    if((DrawableCollided.dist_type==SURE_D_EQUAL)||(DrawableCollided.dist_type==SURE_D_NORM&&DrawableCollided.dist_sigma>0.1f))
                    { // рассеивающая поверхность
                        __VTYPE shade = 255.0f*dot(collision_normal,TraceVector);
                        if(shade<0.0f)shade=-shade;
                        TraceColor.x = TraceFade.x*shade*DrawableCollided.rgb.__XX;
                        TraceColor.y = TraceFade.y*shade*DrawableCollided.rgb.__YY;
                        TraceColor.z = TraceFade.z*shade*DrawableCollided.rgb.__ZZ;
                        #ifdef __LOGGING
                            TraceLog->Items[TraceLog->ItemsCount].NormalRandomized = collision_normal;
                            TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                            TraceLog->Items[TraceLog->ItemsCount].rechecks = RecheckCounter;
                            ++TraceLog->ItemsCount;
                        #endif // __LOGGING
                        break;
                    };
                    #endif // SURE_RLEVEL<60
                    __VTYPE3 CollisionNormalRandomized;
                    SURE_RANDOMIZE(CollisionNormalRandomized);
                    __VTYPE l = -2.0f*dot(TraceVector,CollisionNormalRandomized);
                    __VTYPE3 ReflectedVector = __MAD(l,CollisionNormalRandomized,TraceVector);
                    if(dot(ReflectedVector,collision_normal)>0.0f)
                    { // Результат отражения направлен от объекта
                        SET_TRACE_POINT_MINUS;
                        TraceVector = __NORMALIZE(ReflectedVector);
                        TraceLength += intersect_dist;
                        TraceFade.x *= __DIVIDE(DrawableCollided.rgb.__XX,255.0f);
                        TraceFade.y *= __DIVIDE(DrawableCollided.rgb.__YY,255.0f);
                        TraceFade.z *= __DIVIDE(DrawableCollided.rgb.__ZZ,255.0f);
                        #ifdef __LOGGING
                            TraceLog->Items[TraceLog->ItemsCount].NormalRandomized = CollisionNormalRandomized;
                            TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                            TraceLog->Items[TraceLog->ItemsCount].rechecks = RecheckCounter;
                            ++TraceLog->ItemsCount;
                        #endif // __LOGGING
                    }else{ // результат отражения направлен внутрь объекта
                        NeedToRecheck = true;
                    };
                }; // отражение/преломление
            #if SURE_RLEVEL>90
            }else{ // Рассеивание
                NeedToRecheck = false;
                DrawableCollided.dist_type = SURE_D_EQUAL;
                DrawableCollided.dist_sigma = 1.0f;
                DrawableCollided.dist_m = 0.0f;
                __VTYPE3 CollisionNormalRandomized;
                SURE_RANDOMIZE(CollisionNormalRandomized);
                __VTYPE l = -2.0f*dot(TraceVector,CollisionNormalRandomized);
                if(l>=0.0f)
                {
                    __VTYPE3 ReflectedVector = __MAD(l,CollisionNormalRandomized,TraceVector);
                    SET_TRACE_POINT_MINUS;
                    TraceVector = __NORMALIZE(ReflectedVector);
                    TraceLength += intersect_dist;
                    TraceFade.x *= __DIVIDE((__VTYPE)DrawableCollided.rgb.__XX,255.0f);
                    TraceFade.y *= __DIVIDE((__VTYPE)DrawableCollided.rgb.__YY,255.0f);
                    TraceFade.z *= __DIVIDE((__VTYPE)DrawableCollided.rgb.__ZZ,255.0f);
                    #ifdef __LOGGING
                        TraceLog->Items[TraceLog->ItemsCount].NormalRandomized = CollisionNormalRandomized;
                        TraceLog->Items[TraceLog->ItemsCount].Fade = TraceFade;
                        TraceLog->Items[TraceLog->ItemsCount].rechecks = RecheckCounter;
                        ++TraceLog->ItemsCount;
                    #endif // __LOGGING
                }else{
                    NeedToRecheck = true;
                };
            }; // Рассеивание / отражение / преломление
            #endif // SURE_RLEVEL>90
        }; // while NeedToRecheck
        #else // RLEVEL<20
        __VTYPE shade = -255.0f*dot(collision_normal,TraceVector);
        if(shade<0.0f)shade=0.0f;//-shade;
        TraceColor.x = shade*DrawableCollided.rgb.__XX;
        TraceColor.y = shade*DrawableCollided.rgb.__YY;
        TraceColor.z = shade*DrawableCollided.rgb.__ZZ;
        break;
        #endif // SURE_RLEVEL><20
    }; //WHILE Трассировка


    #ifndef __LOGGING
    #ifndef __SELECT_OBJECT
    // если точка черная -- освещаем ее "глобальным" холодным светом -- это дает подсветку теней
    if((TraceColor.x+TraceColor.y+TraceColor.z)==0)
    {
        TraceColor = __MAD(TraceFade,GPUData->r_backlight,TraceColor);
    };

    uint k = y*SURE_MAXRES_X*3*SURE_FAA*SURE_SCALE+x*3*SURE_SCALE;
    for(int sx=0;sx<SURE_SCALE;++sx)
    for(int sy=0;sy<SURE_SCALE;++sy)
    {
        uint d = k+sy*SURE_MAXRES_X*3*SURE_FAA+sx*3;
        if(GPUData->reset)
        {
            rgbmatrix[d++] = TraceColor.x;
            rgbmatrix[d++] = TraceColor.y;
            rgbmatrix[d++] = TraceColor.z;
        }else{
            rgbmatrix[d++] += TraceColor.x;
            rgbmatrix[d++] += TraceColor.y;
            rgbmatrix[d++] += TraceColor.z;
        };
    };
    #endif // __SELECT_OBJECT
    #endif // ifndef __LOGGING
