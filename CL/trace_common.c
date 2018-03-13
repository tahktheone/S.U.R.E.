// Вспомогательные переменные для поиска пересечений
    __VTYPE2 t;
    bool in;
    // поиск столкновения
    __SURE_GLOBAL struct SureDrawable* lv_cur;
    __VTYPE3 cp; // collision point
    __VTYPE3 cn; // collision normal
    __VTYPE id; // intersect dist
    __SURE_GLOBAL struct SureDrawable* cur;
    __SURE_GLOBAL struct SureDrawable* col;
    __SURE_GLOBAL struct SureDrawable* lv_dr;
    __VTYPE col_refr = 0.0;
    __SURE_UCHAR3 col_rgb;
    __SURE_UINT4 col_rgba;
    __VTYPE col_transp = 0.0;
    int col_radiance;

    int col_dt; // тип рандомизации
    __VTYPE col_ds; // sigma рандомизации
    __VTYPE col_dm; // матожидание рандомизации
    bool col_in; // рассеивание
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

    __VTYPE3 tv = DetermineTraceVector(x,y,GPUData,Randomf,&r);
    __VTYPE3 tp = __FCONV3(GPUData->cam_x);
    tv = __NORMALIZE(tv);
    cur = &Drawables[0];
    rgb.x = 0; rgb.y = 0; rgb.z = 0;
    fade.x = 1; fade.y = 1; fade.z = 1;
    tl = 0; ri = 0; in = false;
    uint miters = GPUData->r_maxiters;
    uint mrechecks = GPUData->r_rechecks;

    while((tl<SURE_R_MAXDISTANCE)&&((fade.x+fade.y+fade.z)>SURE_R_FADELIMIT)&&(ri<miters))
    {
        ++ri;
        lv_cur = cur;
        id = SURE_R_MAXDISTANCE;
        col = 0;
        col_in = false;

        for(int i=1;i<GPUData->m_drawables;++i)
        {
            lv_dr = &Drawables[i];
            switch(lv_dr->type)
            {
                case SURE_DR_SPHERE:
                {
                    if(CollideRaySphered(tp,tv,__FCONV3(lv_dr->X),__FCONV(lv_dr->lx),&t,&in,&id))
                    {
                       if(lv_dr->sided||!in)
                       {
                           col = lv_dr;
                           cur = lv_cur;
                           col_radiance = lv_dr->radiance;
                       }else{
                           col = &Drawables[0];
                           cur = lv_dr;
                           col_radiance = 0;
                       };
                        RT_SETCOL;
                        cp = tp + id*tv;
                        if(lv_dr->map_id>=0)
                        {
                            __VTYPE3 lc;
                            __VTYPE3 l_vtcp = cp - __FCONV3(lv_dr->X);             // вектор к точке пересечения
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
                            __VTYPE3 l_vtcp = cp - __FCONV3(lv_dr->X);             // вектор к точке пересечения
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
                        if(in)
                        {
                            cn = __FCONV3(lv_dr->X) - cp;
                        }else{
                            cn = cp - __FCONV3(lv_dr->X);
                        };
                        cn = __NORMALIZE(cn);
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
                    if(t<(__VTYPE)SURE_R_DELTA||t>id)break;                   // Слишком близко или дальше чем уже найденное пересечение
                    __VTYPE3 l_cp = tp+t*tv;                       // точка пересечение
                    __VTYPE3 l_vtcp = __FCONV3(lv_dr->X) - l_cp;             // вектор к точке пересечения
                    __VTYPE lx = dot(__FCONV3(lv_dr->ox),l_vtcp);               // Локальные координаты
                    __VTYPE ly = dot(__FCONV3(lv_dr->oy),l_vtcp);               // Локальные координаты
                    if(lx>lv_dr->lx||lx<-lv_dr->lx||ly>lv_dr->ly||ly<-lv_dr->ly)break; // Нет попадания в область
                    id = t;
                    cp = l_cp;
                    if(dir>0.0) // с внутренней стороны
                    {
                        if(lv_dr->sided)
                        { // материал двухсторонний
                            col = lv_dr;
                            cur = lv_cur;
                            col_radiance = lv_dr->radiance;
                        }else{  // материал односторонний
                            col = &Drawables[0];
                            cur = lv_dr;
                            col_radiance = 0.0;
                        };
                        RT_SETCOL;
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
                        cn = -__FCONV3(lv_dr->oz);
                    }else{ // с внешней стороны
                        col = lv_dr;
                        cur = lv_cur;
                        col_radiance = lv_dr->radiance;
                        RT_SETCOL;
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
                        cn = __FCONV3(lv_dr->oz);
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
                        gm1.x = gm1.x * lv_dr->lx;
                        gm1.y = gm1.y * lv_dr->ly;
                        gm1.z = gm1.z * lv_dr->lz;
                        gm2.x = gm2.x * lv_dr->lx;
                        gm2.y = gm2.y * lv_dr->ly;
                        gm2.z = gm2.z * lv_dr->lz;
                        gm3.x = gm3.x * lv_dr->lx;
                        gm3.y = gm3.y * lv_dr->ly;
                        gm3.z = gm3.z * lv_dr->lz;

                        // Алгоритм Моллера — Трумбора
                        __VTYPE3 pvec = cross(ltv,gm3-gm1);
                        __VTYPE det = dot(gm2-gm1,pvec);
                        if(fabs(det)<SURE_R_DELTA) continue;
                        __VTYPE inv_det = 1.0 / det;
                        __VTYPE3 tvec = ltp - gm1;
                        __VTYPE u = dot(tvec, pvec) * inv_det;
                        if (u < 0.0 || u > 1.0 ) continue;
                        __VTYPE3 qvec = cross(tvec, gm2-gm1);
                        __VTYPE v = dot(ltv, qvec) * inv_det;
                        if (v < 0.0 || (u + v) > 1.0 ) continue;
                        __VTYPE tt = dot(gm3-gm1, qvec) * inv_det;
                        if(tt<(__VTYPE)SURE_R_DELTA||tt>id)continue; // Слишком близко или дальше чем уже найденное пересечение
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
                                col_radiance = lv_dr->radiance;
                            }else{  // материал односторонний
                                #if SURE_RLEVEL<30
                                    continue;
                                #else
                                    col = &Drawables[0];
                                    cur = lv_dr;
                                    col_radiance = 0;
                                #endif
                            };
                            id = tt;
                            cp = l_cp;
                            RT_SETCOL;
                            if(lv_dr->map_id>=0)
                            {
                                __GET_TEXTURE_UV(cm,lv_dr->map_id);
                            };
                            if(lv_dr->advmap_id>=0)
                            {
                                __GET_ADVMAP_UV(cm,lv_dr->advmap_id);
                            };
                            cn = -n;
                        }else{ // с внешней стороны
                            col = lv_dr;
                            cur = lv_cur;
                            col_radiance = lv_dr->radiance;
                            id = tt;
                            cp = l_cp;
                            RT_SETCOL;
                            cn = n;
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
        if(rr<id)
        {
            col = cur;
            col_radiance = 0;
            RT_SETCOL;
            id = rr;
            cn = -tv;
            cp = tp+id*tv;
            col_in =true;
        };
        #endif // SURE_RLEVEL>90

        if(col==0)break; // луч улетел в никуда

        if(col_radiance>0)
        {
            #if SURE_RLEVEL>60
            rgb.x += fade.x*col_rgb.x;
            rgb.y += fade.y*col_rgb.y;
            rgb.z += fade.z*col_rgb.z;
            #else
            rgb.x += 255.0*fade.x*col_rgb.x;
            rgb.y += 255.0*fade.y*col_rgb.y;
            rgb.z += 255.0*fade.z*col_rgb.z;
            #endif // SURE_RLEVEL>60
            break;
        };

        bool recheck = true;
        uint c = 0;

        #if SURE_RLEVEL>20
        while(recheck&&++c<mrechecks)
        {
            #if SURE_RLEVEL>90
            if(!col_in)
            { // Не рассеивание -- отражение/преломление
            #endif // SURE_RLEVEL
                #if SURE_RLEVEL<60
                __VTYPE rr = 0.9;
                #else
                if(++r>=SURE_R_RNDSIZE)r-=SURE_R_RNDSIZE;
                __VTYPE rr = Randomf[r];
                #endif
                recheck = false;
                if(rr<col_transp)
                { // сработала прозрачность -- считаем преломление
                    __VTYPE3 cnr = randomize(cn,col_dt,col_ds,col_dm,&r,Randomf);
                    __VTYPE l = dot(tv,cnr);
                    if(l>0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        __VTYPE3 rv;
                        __VTYPE sTh = 0;
                        if(col_transp>1.0)
                        {
                            rv = tv;
                        }else{
                            __VTYPE3 A = col_refr*(tv-l*cnr);
                            sTh = __LENGTH(A);  // Синус угла преломления
                            __VTYPE cTh = sqrt(1-sTh*sTh); // Косинус угла преломления
                            rv = A-cTh*cnr;
                        };
                        if((dot(rv,cn)>=0||sTh>(__VTYPE)(1.0))&&col_transp<1.0)
                        {   // Oтражение (в т ч внутреннее)
                            l = -2*dot(tv,cnr);
                            if(l<0){recheck = true;}else{ //Только если модифицированная нормаль не коллинеарна вектору трассировки
                                __VTYPE3 rv = l*cnr+tv;
                                if(dot(rv,cn)<0){recheck=true;}else{ // результат отражение направлен от плоскости
                                    tp = cp;
                                    tv = __NORMALIZE(rv);
                                    tl += id;
                                    fade.x *= col_rgb.x/255.0;
                                    fade.y *= col_rgb.y/255.0;
                                    fade.z *= col_rgb.z/255.0;
                                };// // результат отражение направлен от плоскости
                            }; //Только если модифицированная нормаль не коллинеарна вектору трассировки
                        }else{ // преломление
                            tp = cp;
                            tv = __NORMALIZE(rv);
                            cn = -cn;
                            tl += id;
                            #if SURE_RLEVEL>90
                            if(col_transp<1.0)
                            {
                            #endif // SURE_RLEVEL>90
                                fade.x *= col_rgb.x/255.0;
                                fade.y *= col_rgb.y/255.0;
                                fade.z *= col_rgb.z/255.0;
                            #if SURE_RLEVEL>90
                            };
                            #endif // SURE_RLEVEL>90
                            cur = col;
                        };  // Внутреннее отражение /  преломление
                    };// if(l>0) модифицированная нормаль не коллинеарна вектору трассировки
                }else{ // не сработала прозрачность -- считаем отражение
                    #if SURE_RLEVEL<60
                    if((col_dt==SURE_D_EQUAL)||(col_dt==SURE_D_NORM&&col_ds>0.1))
                    { // рассеивающая поверхность
                        __VTYPE shade = 255.0*dot(cn,tv);
                        if(shade<0.0)shade=-shade;
                        rgb.x = fade.x*shade*col_rgb.x;
                        rgb.y = fade.y*shade*col_rgb.y;
                        rgb.z = fade.y*shade*col_rgb.z;
                        break;
                    };
                    #endif // SURE_RLEVEL<60
                    __VTYPE3 cnr = randomize(cn,col_dt,col_ds,col_dm,&r,Randomf);
                    __VTYPE l = -2*dot(tv,cnr);
                    __VTYPE3 rv = __MAD(l,cnr,tv);
                    if(dot(rv,cn)>=0)
                    { // Результат отражения направлен от объекта
                        tp = cp;
                        rv = __NORMALIZE(rv);
                        tv = rv;
                        tl+=id;
                        fade.x *= col_rgb.x/255.0;
                        fade.y *= col_rgb.y/255.0;
                        fade.z *= col_rgb.z/255.0;
                    }else{ // результат отражения направлен внутрь объекта
                        recheck = true;
                    };
                }; // отражение/преломление
            #if SURE_RLEVEL>90
            }else{ // Рассеивание
                recheck = false;
                __VTYPE3 cnr = randomize(cn,SURE_D_EQUAL,1.0,0.0,&r,Randomf);
                __VTYPE l = -2.0*dot(tv,cnr);
                __VTYPE3 rv;
                if(l>=0)
                {
                    rv = tv+l*cnr;
                    tp = cp;
                    tv = __NORMALIZE(rv);
                    tl+=id;
                    fade.x *= col_rgb.x/255.0;
                    fade.y *= col_rgb.y/255.0;
                    fade.z *= col_rgb.z/255.0;
                }else{
                    recheck = true;
                };
            }; // Рассеивание / отражение / преломление
            #endif // SURE_RLEVEL>90
        }; // while recheck
        #else // RLEVEL<20
        __VTYPE shade = 255.0*dot(cn,tv);
        if(shade<0.0)shade=-shade;
        rgb.x = shade*col_rgb.x;
        rgb.y = shade*col_rgb.y;
        rgb.z = shade*col_rgb.z;
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
