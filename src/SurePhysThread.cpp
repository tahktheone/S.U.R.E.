#include "SurePhysThread.h"

#include <SureData.h>

SurePhysThread::~SurePhysThread()
{

}

void SurePhysThread::run()
{
    while(m_running)
    {
        clock_gettime(CLOCK_MONOTONIC,&framestart);

        if(!EngineData->paused)
        {
            for(int i = 0;i<EngineData->m_objects;++i)
            {
                SureObject* lv_o = &EngineData->objects[i];
                if((lv_o->ParentID<0)&&(lv_o->movable))
                {
                    lv_o->next_tick();
                    lv_o->push(lv_o->X,my_double3{0,0,-1},0.03);
                };
            };

            for(int i = 0;i<EngineData->m_links;++i)
            {
                SureObject* o1 = EngineData->links[i].o1;
                SureObject* o2 = EngineData->links[i].o2;
                my_double3 pd = o1->X-o2->X;
                double d = (EngineData->links[i].l-__LENGTH(pd))*EngineData->links[i].k;
                pd = __NORMALIZE(pd);
                if(!o1->movable){
                    o2->push(o2->X,pd,-d);
                }
                else if(!o2->movable){
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
                if((lv_o->movable)&&(lv_o->ParentID<0)){
                    lv_o->align_p4();
                };
                if(lv_o->ParentID>=0){
                    SureObject *lv_parent = EngineData->ObjByID(lv_o->ParentID);
                    lv_o->align_byparent(lv_parent);
                };
            };

            SurePhysCollision Collisions[1000];
            int CollisionsCounter = -1;
            int TotalObjects = EngineData->m_objects;

            #pragma omp parallel for schedule(dynamic,3)
            for(int i = 0;i<TotalObjects;++i){
                SurePhysCollision Collision;
                bool CollisionFound = false;
                SureObject* lv_o1 = &EngineData->objects[i];
                if(lv_o1->collidable){
                    for(int j = i+1;j<TotalObjects;++j){
                        SureObject* lv_o2 = &EngineData->objects[j];

                        if(  (lv_o2->ParentID!=lv_o1->ParentID)
                           ||((lv_o2->ParentID<0)&&(lv_o1->ParentID<0)) )
                        // не должно срабатывать внутри одного составного объекта
                        if((lv_o2->collidable)&&(lv_o2->movable||lv_o1->movable)){
                            if(lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_SPHERE){ // шарик с шариком
                                CollisionFound = PhysCollideSphereSphere(lv_o1,lv_o2,&Collision);
                            };  // шарик с шариком

                            if((lv_o1->type==SURE_OBJ_SPHERE&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE)){   // шарик с плоскостью
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_PLANE){
                                    CollisionFound = PhysCollideSpherePlane(lv_o2,lv_o1,&Collision);
                                }else{
                                    CollisionFound = PhysCollideSpherePlane(lv_o1,lv_o2,&Collision);
                                };
                            }; // шарик с плоскостью

                            if((lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_SPHERE)||
                               (lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_SPHERE)){   // mesh с шариком
                                if(lv_o2->type==SURE_OBJ_SPHERE&&lv_o1->type==SURE_OBJ_MESH)
                                {
                                    CollisionFound = PhysCollideSphereMesh(lv_o2,lv_o1,EngineData,&Collision);
                                }else{
                                    CollisionFound = PhysCollideSphereMesh(lv_o1,lv_o2,EngineData,&Collision);
                                };

                            }; // mesh сталкивается с шаром

                            if((lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_PLANE)||
                               (lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_PLANE)){   // mesh с плоскостью
                                if(lv_o2->type==SURE_OBJ_MESH&&lv_o1->type==SURE_OBJ_PLANE)
                                {
                                    CollisionFound = PhysCollideMeshPlane(lv_o2,lv_o1,EngineData,&Collision);
                                }else{
                                    CollisionFound = PhysCollideMeshPlane(lv_o1,lv_o2,EngineData,&Collision);
                                };
                            }; // mesh с плоскостью

                            if(lv_o1->type==SURE_OBJ_MESH&&lv_o2->type==SURE_OBJ_MESH){ // mesh'ы
                                CollisionFound = PhysCollideMeshMesh(lv_o1,lv_o2,EngineData,&Collision);
                            };// mesh'ы

                            if(CollisionFound){
                                Collisions[++CollisionsCounter] = Collision;
                            };

                        }; // if(lv_o2->collidable&&lv_o2!=lv_o1)
                    }; //for(int j = 0;j<EngineData->m_objects;++j)
                }; // if(lv_o1->movable&&lv_o1->collidable)
            };//  for(int i = 0;i<EngineData->m_objects;++i)

            int *RandomSequence = (int *)malloc(sizeof(int)*(CollisionsCounter + 1));
            for(int i = 0;i<=CollisionsCounter;++i){RandomSequence[i]=i;};
            for(int i = 0;i<=CollisionsCounter;++i){
                int RandomNumber = int(0.5f + (float)CollisionsCounter*((float)rand()/(float)RAND_MAX));
                int LocalBuffer = RandomSequence[i];
                RandomSequence[i]=RandomSequence[RandomNumber];
                RandomSequence[RandomNumber]=LocalBuffer;
            };

            for(int ri = 0;ri<=CollisionsCounter;++ri){
                int i = RandomSequence[ri];
                SureObject *o1;
                if(Collisions[i].Object1->ParentID<0){
                    o1 = Collisions[i].Object1;
                }else{
                    o1 = EngineData->ObjByID(Collisions[i].Object1->ParentID);
                };
                SureObject *o2;
                if(Collisions[i].Object2->ParentID<0){
                    o2 = Collisions[i].Object2;
                }else{
                    o2 = EngineData->ObjByID(Collisions[i].Object2->ParentID);
                };
                ObjCollide(o1,
                           o2,
                           Collisions[i].CollisionPoint,
                           Collisions[i].CollisionVector,
                           Collisions[i].CollisionLength);
            };
            free(RandomSequence);

            EngineData->reset = true;
        }; // !paused

        // BEG Поворот и перемещение камеры:
        my_double3 dx = EngineData->CameraInfo.cam_vec;
        my_double3 dz = EngineData->CameraInfo.cam_upvec;
        my_double3 dy;
        dy = cross(dz,dx);

        EngineData->CameraInfo.cam_x = EngineData->CameraInfo.cam_x + dx*EngineData->cam_dx.x + dy*EngineData->cam_dx.y+dz*EngineData->cam_dx.z;

        if(EngineData->cam_dw.z!=0||EngineData->cam_dw.y!=0||EngineData->cam_dw.x!=0)
        {
            my_double3 dx1;
            my_double3 dz1;
            my_double3 dy1;

            cl_double c0 = cos(EngineData->cam_dw.x);
            cl_double s0 = sin(EngineData->cam_dw.x);
            cl_double c1 = cos(EngineData->cam_dw.y);
            cl_double s1 = sin(EngineData->cam_dw.y);
            cl_double c2 = cos(EngineData->cam_dw.z);
            cl_double s2 = sin(EngineData->cam_dw.z);
            dx1 = dx*c2+dy*s2+dz*s1;
            dz1 = dz*c1-dx*(s1*c0)+dy*(c2*s0)-dx*(s2*s0);
            dy1 = __NORMALIZE(cross(dz1,dx1));

            dx1 = __NORMALIZE(dx1);
            dz1 = __NORMALIZE(cross(dx1,dy1));
            EngineData->CameraInfo.cam_upvec = dz1;
            EngineData->CameraInfo.cam_vec = dx1;

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
        if(EngineData->ShowTemplate)
            if((frametime.tv_sec - EngineData->ShowTemplateTime.tv_sec)>2.0){
                EngineData->ShowTemplate = false;
                EngineData->reset = true;
            };
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

    // Нарисуем TemplateObject:
    if(EngineData->ShowTemplate)
    {
        GPUData->Drawables[++i] = EngineData->TemplateObject.drawable;
        GPUData->Drawables[i].X = EngineData->CameraInfo.cam_x
                                + EngineData->CameraInfo.cam_vec*3.0;
                                //+ EngineData->CameraInfo.cam_upvec*4.5
                                //+ cross(EngineData->CameraInfo.cam_vec,EngineData->CameraInfo.cam_upvec)*6.0;
        GPUData->Drawables[i].ox.s[0] = 1; //Локальная ось x
        GPUData->Drawables[i].ox.s[1] = 0;
        GPUData->Drawables[i].ox.s[2] = 0;
        GPUData->Drawables[i].oy.s[0] = 0; //Локальная ось x
        GPUData->Drawables[i].oy.s[1] = 1;
        GPUData->Drawables[i].oy.s[2] = 0;
        GPUData->Drawables[i].oz.s[0] = 0; //Локальная ось x
        GPUData->Drawables[i].oz.s[1] = 0;
        GPUData->Drawables[i].oz.s[2] = 1;
        if(EngineData->TemplateObject.drawable.type==SURE_DR_MESH){
            GPUData->Drawables[i].mesh_start = EngineData->ModelsInfo[EngineData->TemplateObject.ModelID_drawable].mesh_start;
            GPUData->Drawables[i].mesh_count = EngineData->ModelsInfo[EngineData->TemplateObject.ModelID_drawable].mesh_count;
        };
        GPUData->Drawables[i].lx = 0.5f;
        GPUData->Drawables[i].ly = 0.5f;
        GPUData->Drawables[i].lz = 0.5f;
        GPUData->m_drawables++;
    };

    for(int d = 0;d<EngineData->m_objects;++d)
    {
        EngineData->objects[d].drawable.X = EngineData->objects[d].X;
        EngineData->objects[d].drawable.lx = EngineData->objects[d].lx;
        EngineData->objects[d].drawable.ly = EngineData->objects[d].ly;
        EngineData->objects[d].drawable.lz = EngineData->objects[d].lz;
        EngineData->objects[d].drawable.ox = EngineData->objects[d].ox;
        EngineData->objects[d].drawable.oy = EngineData->objects[d].oy;
        EngineData->objects[d].drawable.oz = EngineData->objects[d].oz;

        EngineData->objects[d].DrawableGPUID = -1;

        switch(EngineData->objects[d].type){
            case SURE_OBJ_NONE:
            {
                break;
            };
            case SURE_OBJ_SPHERE:
            {
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                GPUData->Drawables[i].lx = 0.95*EngineData->objects[d].lx;
                GPUData->m_drawables++;
                EngineData->objects[d].DrawableGPUID = i;
                break;
            };
            default:
            {
                if(EngineData->objects[d].drawable.type==SURE_DR_MESH){
                    EngineData->objects[d].drawable.mesh_start = EngineData->ModelsInfo[EngineData->objects[d].ModelID_drawable].mesh_start;
                    EngineData->objects[d].drawable.mesh_count = EngineData->ModelsInfo[EngineData->objects[d].ModelID_drawable].mesh_count;
                };
                GPUData->Drawables[++i] = EngineData->objects[d].drawable;
                EngineData->objects[d].DrawableGPUID = i;
                GPUData->m_drawables++;
                break;
            };
            break;
        }; // case (type)
    }; // for(int d = 0;d<EngineData->m_objects;++d)
} // drawscene()
