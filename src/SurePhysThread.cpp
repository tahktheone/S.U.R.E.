#include "SurePhysThread.h"

#include <SureData.h>

//#define COLLISION_LOGGING_BASE
//#define COLLISION_LOGGING_TEXT
//#define COLLISION_LOGGING_SAVE
//#define COLLISION_LOGGING_COVER
//#define COLLISION_LOGGING_00_SAVE
//#define COLLISION_LOGGING_ERR_SAVE_I
//#define COLLISION_LOGGING_ERR_SAVE_F
//#define COLLISION_LOGGING_00
//#define COLLISION_LOGGING_00_SAVE

SurePhysThread::~SurePhysThread()
{

}

void SurePhysThread::run()
{
    #ifdef COLLISION_LOGGING_BASE
    int l_collision_index = 0;
    #endif // COLLISION_LOGGING
    while(m_running)
    {
        clock_gettime(CLOCK_MONOTONIC,&framestart);

        if(!EngineData->paused)
        {

        /*
        tick++;
        if(tick>30)
        {
            int i = EngineData->m_objects;
            EngineData->objects[i].X.x = 40.0*((float)rand()/(float)RAND_MAX - 0.5); //Координаты центра
            EngineData->objects[i].X.y = 40.0*((float)rand()/(float)RAND_MAX - 0.5); //Координаты центра
            EngineData->objects[i].X.z = 10*((float)rand()/(float)RAND_MAX)+80.0; //Координаты центра
            EngineData->objects[i].ox.x = 1;
            EngineData->objects[i].ox.y = 0;
            EngineData->objects[i].ox.z = 0;
            EngineData->objects[i].oy.x = 0;
            EngineData->objects[i].oy.y = 1;
            EngineData->objects[i].oy.z = 0;
            EngineData->objects[i].oz.x = 0;
            EngineData->objects[i].oz.y = 0;
            EngineData->objects[i].oz.z = 1;
            EngineData->objects[i].lx = 3.0+(float)rand()/(float)RAND_MAX; // длина
            EngineData->objects[i].ly = 1.0; // ширина
            EngineData->objects[i].lz = 1.0; // высота
            EngineData->objects[i].lp = EngineData->objects[i].lx*1.8;
            EngineData->objects[i].type = SURE_OBJ_SPHERE;
            EngineData->objects[i].movable = true;
            EngineData->objects[i].collidable = true;
            EngineData->objects[i].drawable.X  = EngineData->objects[i].X;
            EngineData->objects[i].drawable.ox = EngineData->objects[i].ox;
            EngineData->objects[i].drawable.oy = EngineData->objects[i].oy;
            EngineData->objects[i].drawable.oz = EngineData->objects[i].oz;
            EngineData->objects[i].drawable.lx = EngineData->objects[i].lx;
            EngineData->objects[i].drawable.ly = EngineData->objects[i].ly;
            EngineData->objects[i].drawable.lz = EngineData->objects[i].lz;
            EngineData->objects[i].drawable.type = SURE_DR_SPHERE; // форма
            EngineData->objects[i].drawable.radiance = 0.0; // свечение
            EngineData->objects[i].drawable.transp = 1.1;//0.8+0.3*(float)rand()/(float)RAND_MAX; // прозрачность
            EngineData->objects[i].drawable.transp_i = 0.01+0.06*(float)rand()/(float)RAND_MAX; // прозрачность
            EngineData->objects[i].drawable.refr = 1.49; // Коэффициент преломления
            EngineData->objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
            EngineData->objects[i].drawable.dist_sigma = 0.02*(float)rand()/(float)RAND_MAX; // sigma рандомизации
            EngineData->objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
            EngineData->objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
            EngineData->objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX;// цвет
            EngineData->objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
            EngineData->objects[i].drawable.sided = false;
            EngineData->objects[i].initp4();
            EngineData->m_objects++;
            tick = 0;

            for(int iln=1;iln<=phase;++iln)
            {
                int lnk = EngineData->m_links;
                EngineData->links[lnk].o1 = &EngineData->objects[i];
                EngineData->links[lnk].o2 = &EngineData->objects[i-iln];
                EngineData->links[lnk].l = 10;
                EngineData->links[lnk].k = 0.08;
                EngineData->m_links++;
            };
            phase++;
            if(phase==4)phase=0;
        };
        */

        cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
        cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей

            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o = &EngineData->objects[i];
                if(lv_o->movable)
                {
                    lv_o->next_tick();
                    lv_o->push(lv_o->X,my_double3{0,0,-1},0.01);
                };
            };

            for(int k = 0;k<1;++k)
            {

            for(int i = 0;i<EngineData->m_links;++i)
            {
                SureObject* o1 = EngineData->links[i].o1;
                SureObject* o2 = EngineData->links[i].o2;
                my_double3 pd = o1->X-o2->X;
                double d = (EngineData->links[i].l-__LENGTH(pd))*EngineData->links[i].k;
                pd = __NORMALIZE(pd);
                if(!o1->movable)
                {
                    o2->push(o2->X,pd,-d);
                }
                else if(!o2->movable)
                {
                    o1->push(o1->X,pd,d);
                }
                else{
                    o1->push(o1->X,pd,d*0.5);
                    o2->push(o2->X,pd,-d*0.5);
                }; // проверка на movable
            };


            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o = &EngineData->objects[i];
                if(lv_o->movable)
                {
                    lv_o->align_p4();
                };
            };

            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o1 = &EngineData->objects[i];
                if(lv_o1->collidable)
                {
                    for(int j = i+1;j<EngineData->m_objects;++j)
                    {
                        SureObject* lv_o2 = &EngineData->objects[j];
                        if((lv_o2->collidable)&&(lv_o2->movable||lv_o1->movable))
                        {
                            SureObject* o1;
                            SureObject* o2;
                            if(lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_SPHERE)
                            { // шарик с шариком
                                o1=lv_o1; o2=lv_o2;
                                my_double3 pd = o1->X-o2->X;
                                double d = __LENGTH(pd);
                                if(d<(o1->lx+o2->lx))
                                { // есть пересечение
                                    pd = __NORMALIZE(pd); // Penetration Direction
                                    my_double3 pp = (o1->X+o2->X)*0.5; // Penetration point
                                    double pl = o1->lx+o2->lx-d; // Penetration Length
                                    ObjCollide(o1,o2,pp,pd,pl);
                                };// есть пересечение
                            };  // шарик с шариком
                            if((lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE))
                            {   // шарик с плоскостью
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
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
                                        if(fabs(lx)<o2->lx)
                                        { // в пределах оси x -- столкновение с гранью y
                                            my_double3 pp;
                                            double cd = sqrt(dist*dist+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                                            if(cd<o1->lx)
                                            {
                                                if(ly<0)
                                                {
                                                        pp = o2->X+o2->ox*lx-o2->oy*o2->ly;
                                                }else
                                                {
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
                                                if(lx<0)
                                                {
                                                        pp = o2->X+o2->oy*ly-o2->ox*o2->lx;
                                                }else
                                                {
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
                                                if(lx<0)
                                                {
                                                    pp -= o2->ox*o2->lx;
                                                }else{
                                                    pp += o2->ox*o2->lx;
                                                };
                                                if(ly<0)
                                                {
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
                            }; // шарик с плоскостью

                            if((lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_SPHERE)||
                               (lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_SPHERE))
                            {   // mesh с шариком
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_MESH)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
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
                                        __VTYPE3 n = vp.x*__FCONV3(o2->ox)+ \
                                                     vp.y*__FCONV3(o2->oy)+ \
                                                     vp.z*__FCONV3(o2->oz); \
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
                                    __VTYPE3 n = cln.x*__FCONV3(o2->ox)+ \
                                                 cln.y*__FCONV3(o2->oy)+ \
                                                 cln.z*__FCONV3(o2->oz); \
                                    collision_normal = __NORMALIZE(n); \
                                    collision_point = o1->X-collision_normal*cdl; \
                                };

                                __VTYPE3 ltp; // координаты шара в локальных координатах mesh'ы
                                ltp.x = dot(o1->X-__FCONV3(o2->X),__FCONV3(o2->ox));
                                ltp.y = dot(o1->X-__FCONV3(o2->X),__FCONV3(o2->oy));
                                ltp.z = dot(o1->X-__FCONV3(o2->X),__FCONV3(o2->oz));

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
                                    if(fabs(dd)<=o1->lx)
                                    {
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
                                                        __VTYPE3 n = cn.x*__FCONV3(o2->ox)+
                                                                     cn.y*__FCONV3(o2->oy)+
                                                                     cn.z*__FCONV3(o2->oz); // нормаль в глобальных координатах
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
                            }; // mesh сталкивается с шаром

                            if((lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_PLANE))
                            {   // mesh с плоскостью
                                if(lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_PLANE)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
                                // mesh с плоскостью
                                // o1 - mesh
                                // o2 - плоскость

                                // определяем точки "под"
                                // отсеиваем те что не в квадрате
                                // определяем самую глубокую (и расстояние)
                                // определяем точку (как среднее)

                                __VTYPE3 ltp; // переводим данные о плоскости в локальные координаты meshы
                                ltp.x = dot(o2->X-__FCONV3(o1->X),__FCONV3(o1->ox));
                                ltp.y = dot(o2->X-__FCONV3(o1->X),__FCONV3(o1->oy));
                                ltp.z = dot(o2->X-__FCONV3(o1->X),__FCONV3(o1->oz));

                                __VTYPE3 lox;
                                lox.x = dot(o2->ox,__FCONV3(o1->ox));
                                lox.y = dot(o2->ox,__FCONV3(o1->oy));
                                lox.z = dot(o2->ox,__FCONV3(o1->oz));
                                lox = __NORMALIZE(lox);

                                __VTYPE3 loy;
                                loy.x = dot(o2->oy,__FCONV3(o1->ox));
                                loy.y = dot(o2->oy,__FCONV3(o1->oy));
                                loy.z = dot(o2->oy,__FCONV3(o1->oz));
                                loy = __NORMALIZE(loy);

                                __VTYPE3 loz;
                                loz.x = dot(o2->oz,__FCONV3(o1->ox));
                                loz.y = dot(o2->oz,__FCONV3(o1->oy));
                                loz.z = dot(o2->oz,__FCONV3(o1->oz));
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
                                    collision_point = cp.x*__FCONV3(o1->ox)+
                                                      cp.y*__FCONV3(o1->oy)+
                                                      cp.z*__FCONV3(o1->oz)+
                                                      o1->X;
                                    ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
                                }; // столкновение есть
                            }; // mesh с плоскостью

                            if((lv_o1->type==SURE_OBJ_CREAT&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_CREAT&&lv_o1->type==SURE_OBJ_PLANE))
                            {   // шарик с плоскостью
                                if(lv_o2->type==SURE_OBJ_CREAT&&lv_o1->type==SURE_OBJ_PLANE)
                                {
                                    o1=lv_o2; o2=lv_o1;
                                }else{
                                    o1=lv_o1; o2=lv_o2;
                                };
                                // шарик с плоскостью
                                // o1 - creature
                                // o2 - плоскость

                                // пересечение шара с плоскостью
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
                                        if(fabs(lx)<o2->lx)
                                        { // в пределах оси x -- столкновение с гранью y
                                            my_double3 pp;
                                            double cd = sqrt(dist*dist+(fabs(ly)-o2->ly)*(fabs(ly)-o2->ly));
                                            if(cd<o1->lx)
                                            {
                                                if(ly<0)
                                                {
                                                        pp = o2->X+o2->ox*lx-o2->oy*o2->ly;
                                                }else
                                                {
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
                                                if(lx<0)
                                                {
                                                        pp = o2->X+o2->oy*ly-o2->ox*o2->lx;
                                                }else
                                                {
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
                                                if(lx<0)
                                                {
                                                    pp -= o2->ox*o2->lx;
                                                }else{
                                                    pp += o2->ox*o2->lx;
                                                };
                                                if(ly<0)
                                                {
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

                            };

                            if(lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_MESH)
                            { // mesh'ы
                                o1=lv_o1; o2=lv_o2;
// ============================================================================================
// ====  Алгоритм Гилберта-Джонсона-Кёрти =====================================================
// ============================================================================================

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

#ifdef COLLISION_LOGGING_BASE

#ifdef COLLISION_LOGGING_COVER
bool l_first = true;
#endif // COLLISION_LOGGING_COVER

QImage *l_image;
QPainter *l_painter;
QBrush *l_brush;

#define LOG_PICWID 400.0
#define LOG_PICHIG 400.0
#define LOG_PICWID_A LOG_PICWID*2.0
#define LOG_PICHIG_A LOG_PICHIG*2.0
#define LOG_AMT 12

l_image = new QImage(LOG_PICWID_A,LOG_PICHIG_A*LOG_AMT,QImage::Format_ARGB32);
l_painter = new QPainter();

l_painter->begin(l_image);
l_painter->fillRect(0,0,LOG_PICWID_A,LOG_PICHIG_A*LOG_AMT,Qt::black);

QPen lo_pen;
lo_pen.setColor(Qt::white);
lo_pen.setWidth(1);
l_painter->setPen(lo_pen);

//l_painter->drawText(0,20,"Minkowski:");
//l_painter->drawLine(LOG_PICWID,0,LOG_PICWID,LOG_PICHIG_A); // разделение на 4 области
//l_painter->drawLine(0,LOG_PICHIG,LOG_PICWID_A,LOG_PICHIG); // разделение на 4 области

double min_mc_x = SURE_R_MAXDISTANCE;
double max_mc_x = -SURE_R_MAXDISTANCE;
double min_mc_y = SURE_R_MAXDISTANCE;
double max_mc_y = -SURE_R_MAXDISTANCE;
double min_mc_z = SURE_R_MAXDISTANCE;
double max_mc_z = -SURE_R_MAXDISTANCE;

for(int i = 0;i<mc;++i){
    if(min_mc_x>M[i].x)min_mc_x=M[i].x;
    if(max_mc_x<M[i].x)max_mc_x=M[i].x;
    if(min_mc_y>M[i].y)min_mc_y=M[i].y;
    if(max_mc_y<M[i].y)max_mc_y=M[i].y;
    if(min_mc_z>M[i].z)min_mc_z=M[i].z;
    if(max_mc_z<M[i].z)max_mc_z=M[i].z;
};

//   ^ z       |  ^ z
//    > x      |  > y
//------------------------
//   ^ y       |
//    > x      |

max_mc_x+=20.0;
max_mc_y+=20.0;
max_mc_z+=20.0;
min_mc_x-=20.0;
min_mc_y-=20.0;
min_mc_z-=20.0;

double delta_x = max_mc_x - min_mc_x;
double delta_y = max_mc_y - min_mc_y;
double delta_z = max_mc_z - min_mc_z;

float l_x;
float l_y;

int l_log_index = 0;
int l_log_drawline[4];
int l_log_textline = 0;
char l_text[200];

#define LOG_DRAW_AXISES \
l_painter->drawLine(LOG_PICWID,l_log_index*LOG_PICHIG_A+0,LOG_PICWID,l_log_index*LOG_PICHIG_A+LOG_PICHIG_A); \
l_painter->drawLine(0,l_log_index*LOG_PICHIG_A+LOG_PICHIG,LOG_PICWID_A,l_log_index*LOG_PICHIG_A+LOG_PICHIG); \
lo_pen.setWidth(1); \
lo_pen.setColor(Qt::gray); \
l_painter->setPen(lo_pen); \
if(min_mc_x<0&&max_mc_x>0){ \
         float l_px = LOG_PICWID * ( 0.0 - min_mc_x ) / delta_x; \
         int drawx = l_px; \
         l_painter->drawLine(drawx,l_log_index*LOG_PICHIG_A+0,drawx,l_log_index*LOG_PICHIG_A+LOG_PICHIG_A); \
         l_painter->drawLine(drawx,l_log_index*LOG_PICHIG_A+LOG_PICHIG,drawx,l_log_index*LOG_PICHIG_A+LOG_PICHIG_A); \
}; \
if(min_mc_y<0&&max_mc_y>0){ \
         float l_py = LOG_PICWID * ( 0.0 - min_mc_y ) / delta_y; \
         int drawy = l_py; \
         l_painter->drawLine(LOG_PICWID+drawy,l_log_index*LOG_PICHIG_A+0,LOG_PICWID+drawy,l_log_index*LOG_PICHIG_A+LOG_PICHIG); \
         l_py = LOG_PICHIG * ( 0.0 - min_mc_y ) / delta_y; \
         drawy = l_py; \
         l_painter->drawLine(0,l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-drawy,LOG_PICWID,l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-drawy); \
}; \
if(min_mc_z<0&&max_mc_z>0){ \
         float l_pz = LOG_PICHIG * ( 0.0 - min_mc_z ) / delta_z; \
         int drawz = l_pz; \
         l_painter->drawLine(0,l_log_index*LOG_PICHIG_A+LOG_PICHIG-drawz,LOG_PICWID,l_log_index*LOG_PICHIG_A+LOG_PICHIG-drawz); \
         l_painter->drawLine(LOG_PICWID,l_log_index*LOG_PICHIG_A+LOG_PICHIG-drawz,LOG_PICWID_A,l_log_index*LOG_PICHIG_A+LOG_PICHIG-drawz); \
};

#define LOG_MINKOWSKI \
lo_pen.setColor(Qt::yellow); \
lo_pen.setWidth(3); \
l_painter->setPen(lo_pen); \
for(int i = 0;i<mc;++i){ \
         l_x = LOG_PICWID * ( M[i].x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( M[i].z - min_mc_z ) / delta_z; \
         l_painter->drawPoint((int)(l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y)); \
         sprintf(l_text,"%i",i); \
         l_painter->drawText((int)(l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y),l_text); \
         l_x = LOG_PICWID * ( M[i].y - min_mc_y ) / delta_y; \
         l_y = LOG_PICHIG * ( M[i].z - min_mc_z ) / delta_z; \
         l_painter->drawPoint((int)(LOG_PICWID + l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y)); \
         sprintf(l_text,"%i",i); \
         l_painter->drawText((int)(LOG_PICWID + l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y),l_text); \
         l_x = LOG_PICWID * ( M[i].x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( M[i].y - min_mc_y ) / delta_y; \
         l_painter->drawPoint((int)(l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-l_y)); \
         sprintf(l_text,"%i",i); \
         l_painter->drawText((int)(l_x),(int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-l_y),l_text); \
}; \

    #define LOG_LINE(A,B) \
         l_x = LOG_PICWID * ( A.x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( A.z - min_mc_z ) / delta_z; \
         l_log_drawline[0] = (int)(l_x); \
         l_log_drawline[1] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y); \
         l_x = LOG_PICWID * ( B.x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( B.z - min_mc_z ) / delta_z; \
         l_log_drawline[2] = (int)(l_x); \
         l_log_drawline[3] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y); \
         l_painter->drawLine(l_log_drawline[0],l_log_drawline[1],l_log_drawline[2],l_log_drawline[3]); \
         l_x = LOG_PICWID * ( A.y - min_mc_y ) / delta_y; \
         l_y = LOG_PICHIG * ( A.z - min_mc_z ) / delta_z; \
         l_log_drawline[0] = (int)(LOG_PICWID + l_x); \
         l_log_drawline[1] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y); \
         l_x = LOG_PICWID * ( B.y - min_mc_y ) / delta_y; \
         l_y = LOG_PICHIG * ( B.z - min_mc_z ) / delta_z; \
         l_log_drawline[2] = (int)(LOG_PICWID + l_x); \
         l_log_drawline[3] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG-l_y); \
         l_painter->drawLine(l_log_drawline[0],l_log_drawline[1],l_log_drawline[2],l_log_drawline[3]); \
         l_x = LOG_PICWID * ( A.x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( A.y - min_mc_y ) / delta_y; \
         l_log_drawline[0] = (int)(l_x); \
         l_log_drawline[1] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-l_y); \
         l_x = LOG_PICWID * ( B.x - min_mc_x ) / delta_x; \
         l_y = LOG_PICHIG * ( B.y - min_mc_y ) / delta_y; \
         l_log_drawline[2] = (int)(l_x); \
         l_log_drawline[3] = (int)(l_log_index*LOG_PICHIG_A+LOG_PICHIG_A-l_y); \
         l_painter->drawLine(l_log_drawline[0],l_log_drawline[1],l_log_drawline[2],l_log_drawline[3]);

    #define LOG_POINT(A,B) \
    lo_pen.setWidth(1); \
    lo_pen.setColor(Qt::white); \
    l_painter->setPen(lo_pen); \
    sprintf(l_text,A,B.x,B.y,B.z); \
    l_painter->drawText(LOG_PICWID+10,l_log_index*LOG_PICHIG_A+LOG_PICHIG+20+l_log_textline*20,l_text); \
    l_log_textline++; \

    #define LOG_FRIGE(A,I,F1,F2,F3) \
    lo_pen.setWidth(1); \
    lo_pen.setColor(Qt::white); \
    l_painter->setPen(lo_pen); \
    sprintf(l_text,A,I,F1,F2,F3); \
    l_painter->drawText(LOG_PICWID+10,l_log_index*LOG_PICHIG_A+LOG_PICHIG+20+l_log_textline*20,l_text); \
    l_log_textline++; \

    #define LOG_NUMBER(A,B) \
    lo_pen.setWidth(1); \
    lo_pen.setColor(Qt::white); \
    l_painter->setPen(lo_pen); \
    sprintf(l_text,A,B); \
    l_painter->drawText(LOG_PICWID+10,l_log_index*LOG_PICHIG_A+LOG_PICHIG+20+l_log_textline*20,l_text); \
    l_log_textline++; \

    #define LOG_2NUMBERS(A,B,C) \
    lo_pen.setWidth(1); \
    lo_pen.setColor(Qt::white); \
    l_painter->setPen(lo_pen); \
    sprintf(l_text,A,B,C); \
    l_painter->drawText(LOG_PICWID+10,l_log_index*LOG_PICHIG_A+LOG_PICHIG+20+l_log_textline*20,l_text); \
    l_log_textline++; \

    #define LOG_TEXT(A) \
    lo_pen.setWidth(1); \
    lo_pen.setColor(Qt::white); \
    l_painter->setPen(lo_pen); \
    l_painter->drawText(LOG_PICWID+10,l_log_index*LOG_PICHIG_A+LOG_PICHIG+20+l_log_textline*20,A); \
    l_log_textline++; \

    l_log_index = -1;

#endif // COLLISION_LOGGING
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

    #ifdef COLLISION_LOGGING_00
    lo_pen.setColor(Qt::white);
    lo_pen.setWidth(1);
    l_painter->setPen(lo_pen);
    l_painter->drawText(0,l_log_index*LOG_PICHIG_A+20,"Проверка на 0,0:");
    LOG_DRAW_AXISES;
    LOG_MINKOWSKI;

    lo_pen.setColor(Qt::green);
    lo_pen.setWidth(2);
    l_painter->setPen(lo_pen);

    LOG_LINE(M[TI[0]],M[TI[1]]);
    LOG_LINE(M[TI[0]],M[TI[2]]);
    LOG_LINE(M[TI[0]],M[TI[3]]);
    LOG_LINE(M[TI[1]],M[TI[2]]);
    LOG_LINE(M[TI[1]],M[TI[3]]);
    LOG_LINE(M[TI[2]],M[TI[3]]);

    #endif // COLLISION_LOGGING

#ifdef COLLISION_LOGGING_00
     LOG_FRIGE("L1=%.3f,L2=%.3f,L3=%.3f,L4=%.3f,",L1,L2,L3,L4);
#endif // COLLISION_LOGGING_00

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
        #ifdef COLLISION_LOGGING_00
         LOG_FRIGE("LM=%.3f, грань (%i,%i,%i)",LM,TNI[0],TNI[1],TNI[2]);
        #endif // COLLISION_LOGGING

            __VTYPE3 v = __NORMALIZE(cross(M[TNI[1]]-M[TNI[0]],M[TNI[2]]-M[TNI[0]])); // нормаль основания нового тэтраэдра

        #ifdef COLLISION_LOGGING_00
        LOG_POINT("v=(%.4f;%.4f;%.4f)",v);
        #endif // COLLISION_LOGGING

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
                #ifdef COLLISION_LOGGING_00
                LOG_NUMBER("Найдена точка %i",TNI[3]);
                #endif // COLLISION_LOGGING
                TI[0]=TNI[0];
                TI[1]=TNI[2];
                TI[2]=TNI[1];
                TI[3]=TNI[3];
            }else{
                exit_no_collision = true;
                #ifdef COLLISION_LOGGING_00
                LOG_NUMBER("Пусто. Выходим на итеррации %i",iter);
                #endif // COLLISION_LOGGING
            };
            iter++;
            if(iter>20){
                exit_no_collision = true;
                #ifdef COLLISION_LOGGING_ERR_SAVE_F
                    l_collision_index++;
                    QString Fname = "./screenshots/";
                    char FileName[100];
                    sprintf(FileName,"errcollide_f%i.png",l_collision_index);
                    Fname += FileName;
                    l_image->save(Fname);
                #endif // COLLISION_LOGGING_00
                EngineData->Log->AddLine("f!");
            };
        }else{ // расстояние до 0,0 больше преыдущего
            exit_no_collision = true;
            #ifdef COLLISION_LOGGING_00
             LOG_NUMBER("Выходим. LM=%.3f",LM);
             exit_no_collision = true;
        #ifdef COLLISION_LOGGING_00_EXIT_SAVE
        ++l_collision_index;
        QString Fname = "./screenshots/";
        char FileName[100];
        sprintf(FileName,"collide00%i.png",l_collision_index);
        Fname += FileName;
        l_image->save(Fname);
        #endif // COLLISION_LOGGING_00_EXIT_SAVE
            #endif // COLLISION_LOGGING
        };
    }else{
        // тэттраэдр содержит 0,0
        exit_collision = true;
        bool cover_expanded = false;
        collision_found = true;
        // создаем cover для минковского

        #ifdef COLLISION_LOGGING_00
        LOG_TEXT("Hit 0,0 !");
        #endif // COLLISION_LOGGING

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

#ifdef COLLISION_LOGGING_COVER
            l_log_index++;
            l_log_textline = 0;

     if(l_first){
     LOG_FRIGE("L1=%.3f,L2=%.3f,L3=%.3f,L4=%.3f,",L1,L2,L3,L4);
     l_first = false;
    };

        lo_pen.setColor(Qt::white);
    lo_pen.setWidth(1);
    l_painter->setPen(lo_pen);
    l_painter->drawText(0,l_log_index*LOG_PICHIG_A+20,"Expanding cover");
    LOG_DRAW_AXISES;
    LOG_MINKOWSKI;

    for(uint ci=0;ci<cc;++ci)
    {
        lo_pen.setWidth(1);
        lo_pen.setColor(Qt::red);
        l_painter->setPen(lo_pen);
        LOG_LINE(CF0,CF1);
        LOG_LINE(CF0,CF2);
        LOG_LINE(CF1,CF2);
        LOG_FRIGE("Грань %i: %i,%i,%i",ci,C[ci*3+0],C[ci*3+1],C[ci*3+2]);
    };
#endif // COLLISION_LOGGING

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

            #ifdef COLLISION_LOGGING_COVER
            LOG_NUMBER("Collision distance = %.4f",collision_distance);
            LOG_FRIGE("Грань ближайшая к 0,0 -- %i (%i,%i,%i)",cf,C[cf*3+0],C[cf*3+1],C[cf*3+2]);
            #endif // COLLISION_LOGGING

            // в направлении от 0,0 есть еще точки?
            #define CFF0 M[C[cf*3+0]]
            #define CFF1 M[C[cf*3+1]]
            #define CFF2 M[C[cf*3+2]]
            __VTYPE3 v = __NORMALIZE(cross(CFF1-CFF0,CFF2-CFF0));
            collision_normal = v;

            #ifdef COLLISION_LOGGING_COVER
            LOG_POINT("Collision normal = (%.2f,%.2f,%.2f)",collision_normal);
            LOG_POINT("Vector (%.3f;%.3f;%.3f)",v);
            #endif // COLLISION_LOGGING

            // ищем дальнюю точку в M в направлении v
            __VTYPE md = dot(v,CFF0)+SURE_R_DELTA;
            __VTYPE ld;
            bool f = false;
            uint fndi = 0;
#ifdef COLLISION_LOGGING_COVER
            LOG_NUMBER("Ищем в направлении v дальше чем %.3f",md);
#endif // COLLISION_LOGGING
            for(uint li = 0;li<mc;++li)
            {// для каждой точки M[i]
                if(!incover[li])
                { // если точка не в оболочке уже
                    ld = dot(v,M[li]);
#ifdef COLLISION_LOGGING_COVER
                    //LOG_2NUMBERS("Грань %i расстояние %.2f",li,ld);
#endif // COLLISION_LOGGING
                    if(ld>md)
                    {
                        md = ld;
                        fndi = li;
                        f = true;
                    };
#ifdef COLLISION_LOGGING_COVER
                }else{
                    //LOG_NUMBER("Грань %i в оболочке",li);
#endif // COLLISION_LOGGING
                }; // если точка не в оболочке уже
            }; // для каждой точки M[i]

            if(f){

#ifdef COLLISION_LOGGING_COVER
LOG_NUMBER("Найдена точка %i",fndi);
LOG_NUMBER("Расстояние %.3f",md);
LOG_POINT("Добавляем (%.3f;%.3f;%.3f)",M[fndi]);
#endif // COLLISION_LOGGING

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
        #ifdef COLLISION_LOGGING_ERR_SAVE_I
        l_collision_index++;
        QString Fname = "./screenshots/";
        char FileName[100];
        sprintf(FileName,"errcollide_i%i.png",l_collision_index);
        Fname += FileName;
        l_image->save(Fname);
        #endif // COLLISION_LOGGING_ERR_SAVE_I
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

            #ifdef COLLISION_LOGGING_BASE
            LOG_POINT("Collision point = (%.2f,%.2f,%.2f)",collision_point);

            #endif // COLLISION_LOGGING

#ifdef COLLISION_LOGGING_TEXT
        std::wcout << L"====== collide ======" << "\n";
        std::wcout << L"cn=" << collision_normal.x << L"|" << collision_normal.y << L"|" << collision_normal.z << L"\n";
        std::wcout << L"o1=" << o1->X.x << L"|" << o1->X.y << L"|" << o1->X.z << L";" << L"\n";
        std::wcout << L"o2=" << o2->X.x << L"|" << o2->X.y << L"|" << o2->X.z << L";" << L"\n";
        std::wcout << L"cd=" << collision_distance << L"\n";
        std::wcout << L"cp=" << collision_point.x << L"|" << collision_point.y << L"|" << collision_point.z << L"\n";
#endif // COLLISION_LOGGING_TEXT

#ifdef COLLISION_LOGGING_SAVE
        l_collision_index++;
        QString Fname = "./screenshots/";
        char FileName[100];
        sprintf(FileName,"collide%i.png",l_collision_index);
        Fname += FileName;
        l_image->save(Fname);
#endif // COLLISION_LOGGING_SAVE

        if(collision_found)
            ObjCollide(o1,o2,collision_point,collision_normal,collision_distance);
    };  // тэттраэдр содержит 0,0
};// while(!exit)

#ifdef COLLISION_LOGGING_BASE
l_painter->end();
delete l_painter;
delete l_image;
#endif // COLLISION_LOGGING

                            };// mesh'ы
                            if(lv_o1->type==SURE_OBJ_PLANE&&lv_o2->type==SURE_OBJ_PLANE)
                            { // квадратик с квадратиком
                                o1=lv_o1; o2=lv_o2;
                                // Нет. Квадратики у нас -- стены и полы. Им не нужно между собой сталкиваться

                            }; // mesh vs mesh
                        }; // if(lv_o2->collidable&&lv_o2!=lv_o1)
                    }; //for(int j = 0;j<EngineData->m_objects;++j)
                }; // if(lv_o1->movable&&lv_o1->collidable)
            };//  for(int i = 0;i<EngineData->m_objects;++i)
            } // количество итераций

            EngineData->reset = true;
        }; // !paused
        // BEG Поворот и перемещение камеры:
        my_double3 dx = EngineData->CameraInfo.cam_vec;
        my_double3 dz = EngineData->CameraInfo.cam_upvec;
        my_double3 dy;
        dy = cross(dz,dx);

        EngineData->CameraInfo.cam_x.s[0] += dx.x*EngineData->cam_dx.x+dy.x*EngineData->cam_dx.y+dz.x*EngineData->cam_dx.z;
        EngineData->CameraInfo.cam_x.s[1] += dx.y*EngineData->cam_dx.x+dy.y*EngineData->cam_dx.y+dz.y*EngineData->cam_dx.z;
        EngineData->CameraInfo.cam_x.s[2] += dx.z*EngineData->cam_dx.x+dy.z*EngineData->cam_dx.y+dz.z*EngineData->cam_dx.z;

        if(EngineData->cam_dw.z!=0||EngineData->cam_dw.y!=0||EngineData->cam_dw.x!=0)
        {
            my_double3 dx1;
            my_double3 dz1;
            cl_double c0 = cos(EngineData->cam_dw.x);
            cl_double s0 = sin(EngineData->cam_dw.x);
            cl_double c1 = cos(EngineData->cam_dw.y);
            cl_double s1 = sin(EngineData->cam_dw.y);
            cl_double c2 = cos(EngineData->cam_dw.z);
            cl_double s2 = sin(EngineData->cam_dw.z);
            dx1 = dx*c2+dy*s2+dz*s1;
            dz1 = dz*c1-dx*(s1*c0)+dy*(c2*s0)-dx*(s2*s0);

            EngineData->CameraInfo.cam_upvec = __NORMALIZE(dz1);
            EngineData->CameraInfo.cam_vec = __NORMALIZE(dx1);

            EngineData->reset = true;
        };
        //EngineData->cam_dw[0] = 0;
        EngineData->cam_dw.y = 0;
        EngineData->cam_dw.z = 0;

        if(EngineData->cam_dx.x!=0||EngineData->cam_dx.y!=0||EngineData->cam_dx.z!=0)
            EngineData->reset = true;

        // END Поворот и перемещение камеры

        drawscene();// Данные -- в GPU
        clock_gettime(CLOCK_MONOTONIC,&frametime);
        EngineData->frametime = frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );
        if(EngineData->frametime<SURE_P_DELAY)
            msleep((int)(SURE_P_DELAY-EngineData->frametime));
    };
}

void SurePhysThread::drawscene()
{
    GPUData->CameraInfo = EngineData->CameraInfo;
    GPUData->r_maxiters = EngineData->r_iters;
    GPUData->r_rechecks = EngineData->r_rechecks;
    GPUData->r_backlight = EngineData->r_backlight;
    if(EngineData->reset)GPUData->toreset=true;
    EngineData->reset = false;

    int i = 0;
    GPUData->m_drawables = 0;

    // Воздух
    GPUData->Drawables[i].X.s[0] = 0; //Координаты центра
    GPUData->Drawables[i].X.s[1] = 0;
    GPUData->Drawables[i].X.s[2] = 0;
    GPUData->Drawables[i].ox.s[0] = 1; //Локальная ось x
    GPUData->Drawables[i].ox.s[1] = 0;
    GPUData->Drawables[i].ox.s[2] = 0;
    GPUData->Drawables[i].oy.s[0] = 0; //Локальная ось x
    GPUData->Drawables[i].oy.s[1] = 1;
    GPUData->Drawables[i].oy.s[2] = 0;
    GPUData->Drawables[i].oz.s[0] = 0; //Локальная ось x
    GPUData->Drawables[i].oz.s[1] = 0;
    GPUData->Drawables[i].oz.s[2] = 1;
    GPUData->Drawables[i].lx = 5.0; // длина
    GPUData->Drawables[i].ly = 1.0; // ширина
    GPUData->Drawables[i].lz = 1.0; // высота
    GPUData->Drawables[i].type = SURE_DR_NONE; // форма
    GPUData->Drawables[i].radiance = 0.0; // свечение
    GPUData->Drawables[i].transp = 1.5; // прозрачность
    GPUData->Drawables[i].transp_i = 1.1; // прозрачность
    GPUData->Drawables[i].refr = 1.0; // Коэффициент преломления
    GPUData->Drawables[i].dist_type = SURE_D_EQUAL; // тип рандомизации
    GPUData->Drawables[i].dist_sigma = 1.0; // sigma рандомизации
    GPUData->Drawables[i].dist_m = SURE_PI2; // матожидание рандомизации
    GPUData->Drawables[i].rgb.s[0] = 250; // цвет
    GPUData->Drawables[i].rgb.s[1] = 250;
    GPUData->Drawables[i].rgb.s[2] = 250;
    GPUData->Drawables[i].sided = true;
    GPUData->m_drawables++;

    for(int d = 0;d<EngineData->m_objects;++d)
    {
        switch(EngineData->objects[d].type){
            case SURE_OBJ_SPHERE:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X;
                GPUData->Drawables[i].lx = 0.95*EngineData->objects[d].lx;
                GPUData->Drawables[i].ox = EngineData->objects[d].ox;
                GPUData->Drawables[i].oy = EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = EngineData->objects[d].oz;
                GPUData->m_drawables++;
                break;
            };
            /* case SURE_DR_MESH:
            {

                break;
            }; */
            default:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].X = EngineData->objects[d].X;
                GPUData->Drawables[i].lx = EngineData->objects[d].lx;
                GPUData->Drawables[i].ly = EngineData->objects[d].ly;
                GPUData->Drawables[i].lz = EngineData->objects[d].lz;
                GPUData->Drawables[i].ox = EngineData->objects[d].ox;
                GPUData->Drawables[i].oy = EngineData->objects[d].oy;
                GPUData->Drawables[i].oz = EngineData->objects[d].oz;
                GPUData->Drawables[i].mesh_start = EngineData->ModelsInfo[EngineData->objects[d].ModelID_drawable].mesh_start;
                GPUData->Drawables[i].mesh_count = EngineData->ModelsInfo[EngineData->objects[d].ModelID_drawable].mesh_count;
                GPUData->m_drawables++;
                break;
            };
            break;
        }; // case (type)
    };

}
