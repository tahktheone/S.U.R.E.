    // ускоряем работу с памятью на GPU: забираем информацию о камере в локальную память:
    __SURE_STRUCT SureCameraInfo CameraInfo = GPUData->CameraInfo;

    // drawable, с которым встретился луч (для хранения параметров изменения траектории)
    __SURE_STRUCT SureDrawable DrawableCollided;

    __SURE_GLOBAL __SURE_STRUCT SureDrawable *lv_cur; // drawable в котором находимся на начало шага трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *cur; // drawable в котором находимся на конец шага трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *col; // drwable который обнаружен на пути трассировки
    __SURE_GLOBAL __SURE_STRUCT SureDrawable *lv_dr; // drawable который сейчас анализируется

    //Вспомогательные переменные для трассировки
    __VTYPE tl;
    uint ri;
    __VTYPE3 fade;
    __VTYPE3 rgb;
    // Инициализируем рандом
    uint xx = x+y;
    uint yy = y;
    uint r = mad24(xx,xx+yy,xx+yy);
    while(r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;

    __VTYPE3 tv;
    if(CameraInfo.subp_rnd){
        tv = DetermineTraceVectorSAA(x,y,&CameraInfo,Randomf,&r);
    }else{
        tv = DetermineTraceVector(x,y,&CameraInfo);
    };

    __VTYPE3 tp = __FCONV3(CameraInfo.cam_x);
    tv = __NORMALIZE(tv);
    cur = &Drawables[0];
    rgb.x = 0; rgb.y = 0; rgb.z = 0;
    fade.x = 1; fade.y = 1; fade.z = 1;
    tl = 0; ri = 0;// in = false;
    uint miters = GPUData->r_maxiters;
    uint mrechecks = GPUData->r_rechecks;
    #if SURE_RLEVEL < 50
        miters = 2;
    #endif // SURE_RLEVEL

    while((tl<SURE_R_MAXDISTANCE)&&((fade.x+fade.y+fade.z)>SURE_R_FADELIMIT)&&(ri<miters))
    {
        ++ri;
        lv_cur = cur;

        // параметры пересечения луча с объектом
        __VTYPE3 collision_point;
        __VTYPE3 collision_normal;
        __VTYPE intersect_dist = SURE_R_MAXDISTANCE;
        bool collision_found = false;
        bool collision_inner = false;

        for(int i=1;i<GPUData->m_drawables;++i)
        {
            lv_dr = &Drawables[i];
            switch(lv_dr->type)
            {
                case SURE_DR_SPHERE:
                {
                    bool InSphere = false;
                    if(CollideRaySphered(tp,tv,__FCONV3(lv_dr->X),__FCONV(lv_dr->lx),&InSphere,&intersect_dist))
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
                        RT_SETCOL_D;
                        collision_point = tp + intersect_dist*tv;
                        if(lv_dr->map_id>=0)
                        {
                            __VTYPE3 lc;
                            __VTYPE3 l_vtcp = collision_point - __FCONV3(lv_dr->X);             // вектор к точке пересечения
                            lc.x = dot(__FCONV3(lv_dr->ox),l_vtcp);               // Локальные координаты
                            lc.y = dot(__FCONV3(lv_dr->oy),l_vtcp);               // Локальные координаты
                            lc.z = dot(__FCONV3(lv_dr->oz),l_vtcp);               // Локальные координаты
                            lc = __NORMALIZE(lc);
                            __VTYPE3 uv;
                            uv.y = 0.5*(lc.z+1.0); // от 0 (низ) до 1 (верх)
                            if(lc.x>0){uv.x = atan(lc.y/lc.x)/M_PI+0.5;}
                            else{uv.x = atan(lc.y/lc.x)/M_PI+1.5;};
                            uv.x*=0.5;
                            __GET_TEXTURE(SURE_R_TEXRES*uv.x,
                                          SURE_R_TEXRES*uv.y,
                                          lv_dr->map_id);
                        };
                        if(lv_dr->advmap_id>=0)
                        {
                            __VTYPE3 lc;
                            __VTYPE3 l_vtcp = collision_point - __FCONV3(lv_dr->X);             // вектор к точке пересечения
                            lc.x = dot(__FCONV3(lv_dr->ox),l_vtcp);               // Локальные координаты
                            lc.y = dot(__FCONV3(lv_dr->oy),l_vtcp);               // Локальные координаты
                            lc.z = dot(__FCONV3(lv_dr->oz),l_vtcp);               // Локальные координаты
                            lc = __NORMALIZE(lc);
                            __VTYPE3 uv;
                            uv.y = 0.5*(lc.z+1.0); // от 0 (низ) до 1 (верх)
                            if(lc.x>0){uv.x = atan(lc.y/lc.x)/M_PI+0.5;}
                            else{uv.x = atan(lc.y/lc.x)/M_PI+1.5;};
                            uv.x*=0.5;
                            __GET_ADVMAP(SURE_R_TEXRES*uv.x,
                                          SURE_R_TEXRES*uv.y,
                                          lv_dr->advmap_id);
                        };
                        if(InSphere)
                        {
                            collision_normal = __FCONV3(lv_dr->X) - collision_point;
                        }else{
                            collision_normal = collision_point - __FCONV3(lv_dr->X);
                        };
                        collision_normal = __NORMALIZE(collision_normal);
                    }; // if(CollideRaySphered)
                    break;
                }; // Сфера

                case SURE_DR_SQUARE:
                {
                    __VTYPE3 vtp = tp-__FCONV3(lv_dr->X);
                    __VTYPE dist = dot(__FCONV3(lv_dr->oz),vtp);                // Расстояние до плоскости
                    if(dist<(__VTYPE)SURE_R_DELTA&&dist>-(__VTYPE)SURE_R_DELTA)break;  // Вплотную -- игнорируем
                    __VTYPE dir = dot(__FCONV3(lv_dr->oz),tv);                  // проекция луча на нормаль
                    __VTYPE t = - dist/dir;                           // Расстояние до точки пересечения
                    if(t<(__VTYPE)SURE_R_DELTA||t>intersect_dist)break;                   // Слишком близко или дальше чем уже найденное пересечение
                    __VTYPE3 l_cp = tp+t*tv;                       // точка пересечение
                    __VTYPE3 l_vtcp = __FCONV3(lv_dr->X) - l_cp;             // вектор к точке пересечения
                    __VTYPE lx = dot(__FCONV3(lv_dr->ox),l_vtcp);               // Локальные координаты
                    __VTYPE ly = dot(__FCONV3(lv_dr->oy),l_vtcp);               // Локальные координаты
                    if(lx>lv_dr->lx||lx<-lv_dr->lx||ly>lv_dr->ly||ly<-lv_dr->ly)break; // Нет попадания в область
                    intersect_dist = t;
                    collision_point = l_cp;
                    if(dir>0.0) // с внутренней стороны
                    {
                        if(lv_dr->sided)
                        { // материал двухсторонний
                            col = lv_dr;
                            cur = lv_cur;
                            DrawableCollided.radiance = lv_dr->radiance;
                        }else{  // материал односторонний
                            col = &Drawables[0];
                            cur = lv_dr;
                            DrawableCollided.radiance = 0.0;
                        };
                        RT_SETCOL_D;
                        if(lv_dr->map_id>=0)
                        {
                            __GET_TEXTURE(SURE_R_TEXRES*0.5*(lx+lv_dr->lx)/lv_dr->lx,
                                          SURE_R_TEXRES*0.5*(ly+lv_dr->ly)/lv_dr->ly,
                                          lv_dr->map_id);
                        };
                        if(lv_dr->advmap_id>=0)
                        {
                            __GET_ADVMAP(SURE_R_TEXRES*0.5*(lx+lv_dr->lx)/lv_dr->lx,
                                          SURE_R_TEXRES*0.5*(ly+lv_dr->ly)/lv_dr->ly,
                                          lv_dr->advmap_id);
                        };
                        collision_normal = -__FCONV3(lv_dr->oz);
                    }else{ // с внешней стороны
                        col = lv_dr;
                        cur = lv_cur;
                        DrawableCollided.radiance = lv_dr->radiance;
                        RT_SETCOL_D;
                        if(lv_dr->map_id>=0)
                        {
                            __GET_TEXTURE(SURE_R_TEXRES*0.5*(lx+lv_dr->lx)/lv_dr->lx,
                                          SURE_R_TEXRES*0.5*(ly+lv_dr->ly)/lv_dr->ly,
                                          lv_dr->map_id);
                        };
                        if(lv_dr->advmap_id>=0)
                        {
                            __GET_ADVMAP(SURE_R_TEXRES*0.5*(lx+lv_dr->lx)/lv_dr->lx,
                                          SURE_R_TEXRES*0.5*(ly+lv_dr->ly)/lv_dr->ly,
                                          lv_dr->advmap_id);
                        };
                        collision_normal = __FCONV3(lv_dr->oz);
                    };
                    break;
                }; // Квадрат
                case SURE_DR_MESH:
                {
                    __VTYPE3 ltv;
                    ltv.x = dot(tv,__FCONV3(lv_dr->ox));
                    ltv.y = dot(tv,__FCONV3(lv_dr->oy));
                    ltv.z = dot(tv,__FCONV3(lv_dr->oz));
                    __VTYPE3 ltp;
                    ltp.x = dot(tp-__FCONV3(lv_dr->X),__FCONV3(lv_dr->ox));
                    ltp.y = dot(tp-__FCONV3(lv_dr->X),__FCONV3(lv_dr->oy));
                    ltp.z = dot(tp-__FCONV3(lv_dr->X),__FCONV3(lv_dr->oz));

                    __VTYPE t_in = -SURE_R_MAXDISTANCE;
                    __VTYPE t_out = SURE_R_MAXDISTANCE;
                    __VTYPE t = 0;

                    t = -(ltp.x-lv_dr->lx)/ltv.x;
                    if((ltv.x)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    t = -(ltp.x+lv_dr->lx)/ltv.x;
                    if((-ltv.x)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    t = -(ltp.y-lv_dr->ly)/ltv.y;
                    if((ltv.y)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    t = -(ltp.y+lv_dr->ly)/ltv.y;
                    if((-ltv.y)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    t = -(ltp.z-lv_dr->lz)/ltv.z;
                    if((ltv.z)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    t = -(ltp.z+lv_dr->lz)/ltv.z;
                    if((-ltv.z)>0.0){ if(t<t_out) t_out = t;}
                    else{ if(t>t_in) t_in = t; };

                    if(t_in>t_out||t_out<SURE_R_DELTA)break;

                    ltv.x/=lv_dr->lx;
                    ltv.y/=lv_dr->ly;
                    ltv.z/=lv_dr->lz;
                    __VTYPE t_k = __LENGTH(tv)/__LENGTH(ltv);
                    ltv = __NORMALIZE(ltv);

                    ltp.x/=lv_dr->lx;
                    ltp.y/=lv_dr->ly;
                    ltp.z/=lv_dr->lz;

                    __VTYPE l_rounding = SURE_R_DELTA / t_k;

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
                        __VTYPE3 pvec = cross(ltv,gm3-gm1);
                        __VTYPE det = dot(gm2-gm1,pvec);
                        if(fabs(det)<l_rounding) continue;
                        __VTYPE inv_det = 1.0 / det;
                        __VTYPE3 tvec = ltp - gm1;
                        __VTYPE u = dot(tvec, pvec) * inv_det;
                        if (u < 0.0 || u > 1.0 ) continue;
                        __VTYPE3 qvec = cross(tvec, gm2-gm1);
                        __VTYPE v = dot(ltv, qvec) * inv_det;
                        if (v < 0.0 || (u + v) > 1.0 ) continue;
                        __VTYPE tt = dot(gm3-gm1, qvec) * inv_det * t_k;
                        if(tt<l_rounding||tt>intersect_dist)continue; // Слишком близко или дальше чем уже найденное пересечение
                        __VTYPE3 l_cp = tp+tv*tt;                    // точка пересечения
                        __VTYPE3 locn = cross(gm2-gm1,gm3-gm1);
                        __VTYPE dir = dot(ltv,locn);
                        __VTYPE3 n1;
                        __VTYPE3 n2;
                        __VTYPE3 n3;
                        __GET_NORMAL1(n1,cm);
                        __GET_NORMAL2(n2,cm);
                        __GET_NORMAL3(n3,cm);
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
                            RT_SETCOL_D;
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
                            RT_SETCOL_D;
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
            RT_SETCOL_D;
            intersect_dist = rr;
            collision_normal = -tv;
            collision_point = tp+intersect_dist*tv;
            collision_inner =true;
        };
        #endif // SURE_RLEVEL>90

        if(!collision_found)break; // луч улетел в никуда

        if(DrawableCollided.radiance>0)
        {
            #if SURE_RLEVEL>60
            rgb.x += fade.x*DrawableCollided.rgb.__XX;
            rgb.y += fade.y*DrawableCollided.rgb.__YY;
            rgb.z += fade.z*DrawableCollided.rgb.__ZZ;
            #else
            rgb.x += 255.0*fade.x*DrawableCollided.rgb.__XX;
            rgb.y += 255.0*fade.y*DrawableCollided.rgb.__YY;
            rgb.z += 255.0*fade.z*DrawableCollided.rgb.__ZZ;
            #endif // SURE_RLEVEL>60
            break;
        };

        bool recheck = true;
        uint c = 0;

        #if SURE_RLEVEL>20
        while(recheck&&++c<mrechecks)
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
                    __VTYPE3 cnr = randomize(collision_normal,DrawableCollided.dist_type,DrawableCollided.dist_sigma,DrawableCollided.dist_m,&r,Randomf);
                    __VTYPE l = dot(tv,cnr);
                    if(l>0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        __VTYPE3 rv;
                        __VTYPE sTh = 0;
                        if(DrawableCollided.transp>1.0)
                        {
                            rv = tv;
                        }else{
                            __VTYPE3 A = __FCONV(DrawableCollided.refr)*(tv-l*cnr);
                            sTh = __LENGTH(A);  // Синус угла преломления
                            __VTYPE cTh = sqrt(1-sTh*sTh); // Косинус угла преломления
                            rv = A-cTh*cnr;
                        };
                        if((dot(rv,collision_normal)>=0||sTh>(__VTYPE)(1.0))&&DrawableCollided.transp<1.0)
                        {   // Oтражение (в т ч внутреннее)
                            l = -2*dot(tv,cnr);
                            if(l<0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                                __VTYPE3 rv = l*cnr+tv;
                                if(dot(rv,collision_normal)<0){recheck=true;}else{ // результат отражение направлен от плоскости
                                    tp = collision_point;
                                    tv = __NORMALIZE(rv);
                                    tl += intersect_dist;
                                    fade.x *= DrawableCollided.rgb.__XX/255.0;
                                    fade.y *= DrawableCollided.rgb.__YY/255.0;
                                    fade.z *= DrawableCollided.rgb.__ZZ/255.0;
                                };// // результат отражение направлен от плоскости
                            }; //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        }else{ // преломление
                            tp = collision_point;
                            tv = __NORMALIZE(rv);
                            collision_normal = -collision_normal;
                            tl += intersect_dist;
                            #if SURE_RLEVEL>90
                            if(DrawableCollided.transp<1.0)
                            {
                            #endif // SURE_RLEVEL>90
                                fade.x *= DrawableCollided.rgb.__XX/255.0;
                                fade.y *= DrawableCollided.rgb.__YY/255.0;
                                fade.z *= DrawableCollided.rgb.__ZZ/255.0;
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
                        __VTYPE shade = 255.0*dot(collision_normal,tv);
                        if(shade<0.0)shade=-shade;
                        rgb.x = fade.x*shade*DrawableCollided.rgb.__XX;
                        rgb.y = fade.y*shade*DrawableCollided.rgb.__YY;
                        rgb.z = fade.y*shade*DrawableCollided.rgb.__ZZ;
                        break;
                    };
                    #endif // SURE_RLEVEL<60
                    __VTYPE3 cnr = randomize(collision_normal,DrawableCollided.dist_type,DrawableCollided.dist_sigma,DrawableCollided.dist_m,&r,Randomf);
                    __VTYPE l = -2*dot(tv,cnr);
                    __VTYPE3 rv = __MAD(l,cnr,tv);
                    if(dot(rv,collision_normal)>=0)
                    { // Результат отражения направлен от объекта
                        tp = collision_point;
                        rv = __NORMALIZE(rv);
                        tv = rv;
                        tl+=intersect_dist;
                        fade.x *= DrawableCollided.rgb.__XX/255.0;
                        fade.y *= DrawableCollided.rgb.__YY/255.0;
                        fade.z *= DrawableCollided.rgb.__ZZ/255.0;
                    }else{ // результат отражения направлен внутрь объекта
                        recheck = true;
                    };
                }; // отражение/преломление
            #if SURE_RLEVEL>90
            }else{ // Рассеивание
                recheck = false;
                __VTYPE3 cnr = randomize(collision_normal,SURE_D_EQUAL,1.0,0.0,&r,Randomf);
                __VTYPE l = -2.0*dot(tv,cnr);
                __VTYPE3 rv;
                if(l>=0)
                {
                    rv = tv+l*cnr;
                    tp = collision_point;
                    tv = __NORMALIZE(rv);
                    tl+=intersect_dist;
                    fade.x *= DrawableCollided.rgb.__XX/255.0;
                    fade.y *= DrawableCollided.rgb.__YY/255.0;
                    fade.z *= DrawableCollided.rgb.__ZZ/255.0;
                }else{
                    recheck = true;
                };
            }; // Рассеивание / отражение / преломление
            #endif // SURE_RLEVEL>90
        }; // while recheck
        #else // RLEVEL<20
        __VTYPE shade = 255.0*dot(collision_normal,tv);
        if(shade<0.0)shade=-shade;
        rgb.x = shade*DrawableCollided.rgb.__XX;
        rgb.y = shade*DrawableCollided.rgb.__YY;
        rgb.z = shade*DrawableCollided.rgb.__ZZ;
        break;
        #endif // SURE_RLEVEL><20
    }; //WHILE Трассировка

    // если точка черная -- освещаем ее "глобальным" холодным светом -- это дает подсветку теней

    if((rgb.x+rgb.y+rgb.z)==0)
    {
        rgb.x += fade.x*GPUData->r_backlight;
        rgb.y += fade.y*GPUData->r_backlight;
        rgb.z += fade.z*GPUData->r_backlight;
    };


    uint k = y*SURE_MAXRES_X*3*SURE_FAA*SURE_SCALE+x*3*SURE_SCALE;
    for(int sx=0;sx<SURE_SCALE;++sx)
    for(int sy=0;sy<SURE_SCALE;++sy)
    {
        uint d = k+sy*SURE_MAXRES_X*3*SURE_FAA+sx*3;
        if(GPUData->reset)
        {
            rgbmatrix[d++] = rgb.x;
            rgbmatrix[d++] = rgb.y;
            rgbmatrix[d++] = rgb.z;
        }else{
            rgbmatrix[d++] += rgb.x;
            rgbmatrix[d++] += rgb.y;
            rgbmatrix[d++] += rgb.z;
        };
    };
