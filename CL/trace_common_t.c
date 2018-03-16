    // ускоряем работу с памятью на GPU: забираем информацию о камере в локальную память:
    __SURE_STRUCT SureCameraInfo CameraInfo = GPUData->CameraInfo;

    // drawable, с которым встретился луч (для хранения параметров изменения траектории)
    __SURE_STRUCT SureDrawable DrawableCollided;

    __SURE_GLOBAL __SURE_STRUCT SureDrawable *lv_cur; // drawable в котором находимся на начало шага трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *cur; // drawable в котором находимся на конец шага трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *col; // drwable который обнаружен на пути трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *lv_dr; // drawable который сейчас анализируется

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
    TraceVector = __NORMALIZE(TraceVector);

    cur = &Drawables[0];
    uint MaximumIterrations = GPUData->r_maxiters;
    uint MaximumRechecks = GPUData->r_rechecks;
    #if SURE_RLEVEL < 50
        MaximumIterrations = 2;
    #endif // SURE_RLEVEL

    for(uint Iterration = 0;Iterration<MaximumIterrations;Iterration++)
    {

        if(TraceLength > SURE_R_MAXDISTANCE)continue;
        if((TraceFade.x+TraceFade.y+TraceFade.z) < SURE_R_FADELIMIT)continue;

        lv_cur = cur;

        // параметры пересечения луча с объектом
        __VTYPE3 collision_point;
        __VTYPE3 collision_normal;
        __VTYPE intersect_dist = SURE_R_MAXDISTANCE;
        bool collision_found = false;
        bool collision_inner = false;

        for(int DrawablesIndex=1;DrawablesIndex<GPUData->m_drawables;++DrawablesIndex)
        {
            lv_dr = &Drawables[DrawablesIndex];
            switch(lv_dr->type)
            {
                case SURE_DR_SPHERE:
                {
                    bool InSphere = false;
                    if(RayAndSphereCollided(TracePoint,TraceVector,__FCONV3(lv_dr->X),__FCONV(lv_dr->lx),&InSphere,&intersect_dist))
                    {
                       if(lv_dr->sided||!InSphere)
                       {
                           col = lv_dr;
                           cur = lv_cur;
                           DrawableCollided.radiance = lv_dr->radiance;
                       }else{
                           col = &Drawables[0];
                           cur = lv_dr;
                           DrawableCollided.radiance = 0;
                       };
                        SET_COLLISION;
                        collision_point = TracePoint + intersect_dist*TraceVector;
                        if((lv_dr->map_id>=0)||(lv_dr->advmap_id>=0)){
                            __VTYPE3 UVCoordinates;
                            GET_SPHERICAL_UV_COORDINATES(UVCoordinates);
                            if(lv_dr->map_id>=0)
                            {
                                __GET_TEXTURE(SURE_R_TEXRES*UVCoordinates.x,
                                              SURE_R_TEXRES*UVCoordinates.y,
                                              lv_dr->map_id);
                            };
                            if(lv_dr->advmap_id>=0)
                            {
                                __GET_ADVMAP(SURE_R_TEXRES*UVCoordinates.x,
                                             SURE_R_TEXRES*UVCoordinates.y,
                                             lv_dr->advmap_id);
                            };
                        }; // if((lv_dr->map_id>=0)||(lv_dr->advmap_id>=0))
                        if(InSphere)
                        {
                            collision_normal = __FCONV3(lv_dr->X) - collision_point;
                        }else{
                            collision_normal = collision_point - __FCONV3(lv_dr->X);
                        };
                        collision_normal = __NORMALIZE(collision_normal);
                    }; // if(RayAndSphereCollided)
                    break;
                }; // Сфера

                case SURE_DR_SQUARE:
                {
                    // Расстояние от точки старта луча до плоскости
                    __VTYPE3 VectorToDrawableCenter = TracePoint-__FCONV3(lv_dr->X);
                    __VTYPE DistanceToPlane = dot(__FCONV3(lv_dr->oz),VectorToDrawableCenter);
                    // Если точка старта луча вплотную к плоскости -- игнорируем ее
                    // (чтобы отраженый от плоскости луч из-за округлений не сталкивался с той же плоскостью):
                    if(DistanceToPlane<SURE_R_DELTA&&DistanceToPlane>-SURE_R_DELTA)break;
                    // Проекция вектора трассировки на нормаль
                    __VTYPE TraceVecByNormal = dot(__FCONV3(lv_dr->oz),TraceVector);
                    // Расстояние до точки пересечения луча с плоскостью
                    __VTYPE TraceDistance = - DistanceToPlane/TraceVecByNormal;
                    // Если дальше чем уже найденное пересечение -- выходим
                    if(TraceDistance>intersect_dist)break;
                    // Если плоскость сзади, за стартом луча -- выходим.
                    if(TraceDistance<SURE_R_DELTA)break;
                    // Точка пересечения луча с плоскостью
                    __VTYPE3 CollisionPointCandidate = TracePoint+TraceDistance*TraceVector;
                    // Вектор От чентра объекта (внутренние координаты 0 0) к точке пересечения
                    __VTYPE3 LocalVectorToTracePoint = __FCONV3(lv_dr->X) - CollisionPointCandidate;
                    // Локальные координаты точки пересечения
                    __VTYPE2 LocalCoordinates = {dot(__FCONV3(lv_dr->ox),LocalVectorToTracePoint),
                                                 dot(__FCONV3(lv_dr->oy),LocalVectorToTracePoint)};
                    // Нет попадания в квадрат -- игнорируем
                    if(LocalCoordinates.x >  lv_dr->lx||
                       LocalCoordinates.x < -lv_dr->lx||
                       LocalCoordinates.y >  lv_dr->ly||
                       LocalCoordinates.y < -lv_dr->ly)break;
                    // Луч гарантированно столкнулся с квадратом.
                    // Обрабатываем столкновение
                    intersect_dist = TraceDistance;
                    collision_point = CollisionPointCandidate;
                    if(TraceVecByNormal>0.0f)
                    {  // с внутренней стороны
                        if(lv_dr->sided)
                        { // материал двухсторонний
                            col = lv_dr;
                            cur = lv_cur;
                            DrawableCollided.radiance = lv_dr->radiance;
                        }else{  // материал односторонний
                            col = &Drawables[0];
                            cur = lv_dr;
                            DrawableCollided.radiance = 0.0f;
                        };
                        SET_COLLISION;
                        GET_TEXTURE_SQUARE;
                        collision_normal = -__FCONV3(lv_dr->oz);
                    }else{ // с внешней стороны
                        col = lv_dr;
                        cur = lv_cur;
                        DrawableCollided.radiance = lv_dr->radiance;
                        SET_COLLISION;
                        GET_TEXTURE_SQUARE;
                        collision_normal = __FCONV3(lv_dr->oz);
                    };
                    break;
                }; // Квадрат
                case SURE_DR_MESH:
                {

                    __VTYPE3 LocalTraceVector;
                    LocalTraceVector.x = dot(TraceVector,__FCONV3(lv_dr->ox));
                    LocalTraceVector.y = dot(TraceVector,__FCONV3(lv_dr->oy));
                    LocalTraceVector.z = dot(TraceVector,__FCONV3(lv_dr->oz));

                    __VTYPE3 LocalTracePoint;
                    __VTYPE3 VectorToTracePoint = TracePoint-__FCONV3(lv_dr->X);
                    LocalTracePoint.x = dot(VectorToTracePoint,__FCONV3(lv_dr->ox));
                    LocalTracePoint.y = dot(VectorToTracePoint,__FCONV3(lv_dr->oy));
                    LocalTracePoint.z = dot(VectorToTracePoint,__FCONV3(lv_dr->oz));

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
                    LocalTraceVector.x/=lv_dr->lx;
                    LocalTraceVector.y/=lv_dr->ly;
                    LocalTraceVector.z/=lv_dr->lz;
                    LocalTracePoint.x/=lv_dr->lx;
                    LocalTracePoint.y/=lv_dr->ly;
                    LocalTracePoint.z/=lv_dr->lz;

                    // Коэффициент показывающий отношение размера объекта к размерам мира
                    // Нужен для пересчета длины вектора трассировки
                    __VTYPE ResizingKoeff = __LENGTH(TraceVector)/__LENGTH(LocalTraceVector);
                    LocalTraceVector = __NORMALIZE(LocalTraceVector);

                    // SURE_R_DELTA тоже должна подгоняться под локальные размеры
                    // чтобы избежать искажений в мелких деталях
                    __VTYPE LocalRenderDelta = SURE_R_DELTA / ResizingKoeff;

                    for(uint im = 0;im<lv_dr->mesh_count;++im)
                    { // Для каждой meshины
                        uint cm = lv_dr->mesh_start + im;
                        __SURE_VINT4 mesh;
                        __VTYPE3 gm1;
                        __VTYPE3 gm2;
                        __VTYPE3 gm3;
                        __GET_MESH(mesh,cm);
                        __GET_VERTEX(gm1,mesh.x);
                        __GET_VERTEX(gm2,mesh.y);
                        __GET_VERTEX(gm3,mesh.z);

                        // Алгоритм Моллера — Трумбора
                        __VTYPE3 pvec = cross(LocalTraceVector,gm3-gm1);
                        __VTYPE det = dot(gm2-gm1,pvec);
                        if(fabs(det)<LocalRenderDelta) continue;
                        __VTYPE inv_det = 1.0 / det;
                        __VTYPE3 TraceVectorec = LocalTracePoint - gm1;
                        __VTYPE u = dot(TraceVectorec, pvec) * inv_det;
                        if (u < 0.0 || u > 1.0 ) continue;
                        __VTYPE3 qvec = cross(TraceVectorec, gm2-gm1);
                        __VTYPE v = dot(LocalTraceVector, qvec) * inv_det;
                        if (v < 0.0 || (u + v) > 1.0 ) continue;
                        __VTYPE tt = dot(gm3-gm1, qvec) * inv_det * ResizingKoeff;
                        if(tt<LocalRenderDelta||tt>intersect_dist)continue; // Слишком близко или дальше чем уже найденное пересечение
                        __VTYPE3 l_cp = TracePoint+TraceVector*tt;                    // точка пересечения
                        __VTYPE3 locn = cross(gm2-gm1,gm3-gm1);
                        __VTYPE dir = dot(LocalTraceVector,locn);
                        __VTYPE3 n1;
                        __VTYPE3 n2;
                        __VTYPE3 n3;
                        __GET_NORMALS_INDEX(cm);
                        __GET_NORMALS_BYINDEX(n1,n2,n3);

                        __VTYPE3 ln = n1*(1-u-v)+n2*u+n3*v;
                        __VTYPE3 n = ln.x*__FCONV3(lv_dr->ox)+
                                     ln.y*__FCONV3(lv_dr->oy)+
                                     ln.z*__FCONV3(lv_dr->oz);
                        n = __NORMALIZE(n);
                        if(dir>0.0) // с внутренней стороны
                        {
                            if(lv_dr->sided)
                            { // материал двухсторонний
                                col = lv_dr;
                                cur = lv_cur;
                                DrawableCollided.radiance = lv_dr->radiance;
                            }else{  // материал односторонний
                                #if SURE_RLEVEL<30
                                    continue;
                                #else
                                    col = &Drawables[0];
                                    cur = lv_dr;
                                    DrawableCollided.radiance = 0;
                                #endif
                            };
                            intersect_dist = tt;
                            collision_point = l_cp;
                            //collision_found = SetCollision(cur,col,&DrawableCollided);
                            SET_COLLISION;
                            if(lv_dr->map_id>=0)
                            {
                                __GET_TEXTURE_UV(cm,lv_dr->map_id);
                            };
                            if(lv_dr->advmap_id>=0)
                            {
                                __GET_ADVMAP_UV(cm,lv_dr->advmap_id);
                            };
                            collision_normal = -n;
                        }else{ // с внешней стороны
                            col = lv_dr;
                            cur = lv_cur;
                            DrawableCollided.radiance = lv_dr->radiance;
                            intersect_dist = tt;
                            collision_point = l_cp;
                            //collision_found = SetCollision(cur,col,&DrawableCollided);
                            SET_COLLISION;
                            collision_normal = n;
                            if(lv_dr->map_id>=0)
                            {
                                __GET_TEXTURE_UV(cm,lv_dr->map_id);
                            };
                            if(lv_dr->advmap_id>=0)
                            {
                                __GET_ADVMAP_UV(cm,lv_dr->advmap_id);
                            };
                        };
                    }; // Для каждой meshины
                    break;
                }; // Mesh
                default:
                    break;
            }; // switch(lv_dr->type)
        };// Цикл по Drawables
        #if SURE_RLEVEL>90
        if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
        __VTYPE rr = Randomf[r]*SURE_R_MAXDISTANCE*cur->transp_i;
        if(rr<intersect_dist)
        {
            col = cur;
            collision_found = true;
            DrawableCollided.radiance = 0;
            //collision_found = SetCollision(cur,col,&DrawableCollided);
            SET_COLLISION;
            intersect_dist = rr;
            collision_normal = -TraceVector;
            collision_point = TracePoint+intersect_dist*TraceVector;
            collision_inner =true;
        };
        #endif // SURE_RLEVEL>90

        if(!collision_found)break; // луч улетел в никуда

        if(DrawableCollided.radiance>0)
        {
            #if SURE_RLEVEL>60
            TraceColor.x += TraceFade.x*DrawableCollided.rgb.__XX;
            TraceColor.y += TraceFade.y*DrawableCollided.rgb.__YY;
            TraceColor.z += TraceFade.z*DrawableCollided.rgb.__ZZ;
            #else
            TraceColor.x += 255.0*TraceFade.x*DrawableCollided.rgb.__XX;
            TraceColor.y += 255.0*TraceFade.y*DrawableCollided.rgb.__YY;
            TraceColor.z += 255.0*TraceFade.z*DrawableCollided.rgb.__ZZ;
            #endif // SURE_RLEVEL>60
            break;
        };

        bool recheck = true;
        uint c = 0;

        #if SURE_RLEVEL>20
        while(recheck&&++c<MaximumRechecks)
        {
            #if SURE_RLEVEL>90
            if(!collision_inner)
            { // Не рассеивание -- отражение/преломление
            #endif // SURE_RLEVEL
                #if SURE_RLEVEL<60
                __VTYPE rr = 0.9;
                #else
                if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                __VTYPE rr = Randomf[r];
                #endif
                recheck = false;
                if(rr<DrawableCollided.transp)
                { // сработала прозрачность -- считаем преломление
                    __VTYPE3 cnr;
                    SURE_RANDOMIZE(cnr);
                    __VTYPE l = dot(TraceVector,cnr);
                    if(l>0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        __VTYPE3 rv;
                        __VTYPE sTh = 0;
                        if(DrawableCollided.transp>1.0)
                        {
                            rv = TraceVector;
                        }else{
                            __VTYPE3 A = __FCONV(DrawableCollided.refr)*(TraceVector-l*cnr);
                            sTh = __LENGTH(A);  // Синус угла преломления
                            __VTYPE cTh = sqrt(1-sTh*sTh); // Косинус угла преломления
                            rv = A-cTh*cnr;
                        };
                        if((dot(rv,collision_normal)>=0||sTh>(__VTYPE)(1.0))&&DrawableCollided.transp<1.0)
                        {   // Oтражение (в т ч внутреннее)
                            l = -2*dot(TraceVector,cnr);
                            if(l<0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                                __VTYPE3 rv = l*cnr+TraceVector;
                                if(dot(rv,collision_normal)<0){recheck=true;}else{ // результат отражение направлен от плоскости
                                    TracePoint = collision_point;
                                    TraceVector = __NORMALIZE(rv);
                                    TraceLength += intersect_dist;
                                    TraceFade.x *= DrawableCollided.rgb.__XX/255.0;
                                    TraceFade.y *= DrawableCollided.rgb.__YY/255.0;
                                    TraceFade.z *= DrawableCollided.rgb.__ZZ/255.0;
                                };// // результат отражение направлен от плоскости
                            }; //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        }else{ // преломление
                            TracePoint = collision_point;
                            TraceVector = __NORMALIZE(rv);
                            collision_normal = -collision_normal;
                            TraceLength += intersect_dist;
                            #if SURE_RLEVEL>90
                            if(DrawableCollided.transp<1.0)
                            {
                            #endif // SURE_RLEVEL>90
                                TraceFade.x *= DrawableCollided.rgb.__XX/255.0;
                                TraceFade.y *= DrawableCollided.rgb.__YY/255.0;
                                TraceFade.z *= DrawableCollided.rgb.__ZZ/255.0;
                            #if SURE_RLEVEL>90
                            };
                            #endif // SURE_RLEVEL>90
                            cur = col;
                        };  // Внутреннее отражение /  преломление
                    };// if(l>0) модифицированная нормаль не коллинеарна вектору трассировки
                }else{ // не сработала прозрачность -- считаем отражение
                    #if SURE_RLEVEL<60
                    if((DrawableCollided.dist_type==SURE_D_EQUAL)||(DrawableCollided.dist_type==SURE_D_NORM&&DrawableCollided.dist_sigma>0.1))
                    { // рассеивающая поверхность
                        __VTYPE shade = 255.0*dot(collision_normal,TraceVector);
                        if(shade<0.0)shade=-shade;
                        TraceColor.x = TraceFade.x*shade*DrawableCollided.rgb.__XX;
                        TraceColor.y = TraceFade.y*shade*DrawableCollided.rgb.__YY;
                        TraceColor.z = TraceFade.z*shade*DrawableCollided.rgb.__ZZ;
                        break;
                    };
                    #endif // SURE_RLEVEL<60
                    //__VTYPE3 cnr = randomize(collision_normal,DrawableCollided.dist_type,DrawableCollided.dist_sigma,DrawableCollided.dist_m,&r,Randomf);
                    __VTYPE3 cnr;
                    SURE_RANDOMIZE(cnr);
                    __VTYPE l = -2*dot(TraceVector,cnr);
                    __VTYPE3 rv = __MAD(l,cnr,TraceVector);
                    if(dot(rv,collision_normal)>=0)
                    { // Результат отражения направлен от объекта
                        TracePoint = collision_point;
                        rv = __NORMALIZE(rv);
                        TraceVector = rv;
                        TraceLength+=intersect_dist;
                        TraceFade.x *= DrawableCollided.rgb.__XX/255.0;
                        TraceFade.y *= DrawableCollided.rgb.__YY/255.0;
                        TraceFade.z *= DrawableCollided.rgb.__ZZ/255.0;
                    }else{ // результат отражения направлен внутрь объекта
                        recheck = true;
                    };
                }; // отражение/преломление
            #if SURE_RLEVEL>90
            }else{ // Рассеивание
                recheck = false;
                //__VTYPE3 cnr = randomize(collision_normal,SURE_D_EQUAL,1.0,0.0,&r,Randomf);
                DrawableCollided.dist_type = SURE_D_EQUAL;
                DrawableCollided.dist_sigma = 1.0f;
                DrawableCollided.dist_m = 0.0f;
                __VTYPE3 cnr;
                SURE_RANDOMIZE(cnr);
                __VTYPE l = -2.0*dot(TraceVector,cnr);
                __VTYPE3 rv;
                if(l>=0)
                {
                    rv = TraceVector+l*cnr;
                    TracePoint = collision_point;
                    TraceVector = __NORMALIZE(rv);
                    TraceLength+=intersect_dist;
                    TraceFade.x *= DrawableCollided.rgb.__XX/255.0;
                    TraceFade.y *= DrawableCollided.rgb.__YY/255.0;
                    TraceFade.z *= DrawableCollided.rgb.__ZZ/255.0;
                }else{
                    recheck = true;
                };
            }; // Рассеивание / отражение / преломление
            #endif // SURE_RLEVEL>90
        }; // while recheck
        #else // RLEVEL<20
        __VTYPE shade = 255.0*dot(collision_normal,TraceVector);
        if(shade<0.0)shade=-shade;
        TraceColor.x = shade*DrawableCollided.rgb.__XX;
        TraceColor.y = shade*DrawableCollided.rgb.__YY;
        TraceColor.z = shade*DrawableCollided.rgb.__ZZ;
        break;
        #endif // SURE_RLEVEL><20
    }; //WHILE Трассировка

    // если точка черная -- освещаем ее "глобальным" холодным светом -- это дает подсветку теней

    if((TraceColor.x+TraceColor.y+TraceColor.z)==0)
    {
        TraceColor.x += TraceFade.x*GPUData->r_backlight;
        TraceColor.y += TraceFade.y*GPUData->r_backlight;
        TraceColor.z += TraceFade.z*GPUData->r_backlight;
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
