#include "SureWidget.h"

SureWidget::SureWidget()
{
    int mx = rect().right();
    int my = rect().bottom();
    image = new QImage(mx,my,QImage::Format_ARGB32);
    CursorImage.load("./maps/cursor.png");
    this->update();
}

SureWidget::~SureWidget()
{
    delete rgbmatrix;
    delete image;
}

void SureWidget::PlaceMatrixToImage()
{
    int mx_scaled = rect().right()/SURE_SCALE;
    int my_scaled = rect().bottom()/SURE_SCALE;
    int mx = rect().right();
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

my_double3 SureWidget::ConvertCoordinatesGlobalToCamera(my_double3 GlobalPoint,my_double3 *CameraBasis,bool *VisibleIndicator)
{
    my_double3 Result = {0,0,0};
    my_double3 VectorFromPoint = EngineData->CameraInfo.cam_x-GlobalPoint;
    double DistanceToPoint = dot(CameraBasis[2],VectorFromPoint);
    if(DistanceToPoint<0.0f){
        *VisibleIndicator = true;
        Result.x = rect().right()/2 + rect().right()*((dot(CameraBasis[0],VectorFromPoint) / DistanceToPoint) / EngineData->CameraInfo.xy_h);
        Result.y = rect().bottom()/2 + rect().right()*((dot(CameraBasis[1],VectorFromPoint) / DistanceToPoint) / EngineData->CameraInfo.xy_h);
    }else{
        *VisibleIndicator = false;
    };
    return Result;
}

void SureWidget::DrawLineInGlobalCoordinates(my_double3 i_GP1,my_double3 i_GP2,my_double3 *CameraBasis)
{
    bool VisibleIndicator1;
    bool VisibleIndicator2;
    my_double3 LocalPoint1 = ConvertCoordinatesGlobalToCamera(i_GP1,CameraBasis,&VisibleIndicator1);
    my_double3 LocalPoint2 = ConvertCoordinatesGlobalToCamera(i_GP2,CameraBasis,&VisibleIndicator2);
    if(VisibleIndicator1&&VisibleIndicator2){
        painter.drawLine((int)LocalPoint1.x,
                         (int)LocalPoint1.y,
                         (int)LocalPoint2.x,
                         (int)LocalPoint2.y);
    };
}

void SureWidget::DrawOABB(SureDrawable *lv_dr)
{
    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

    __VTYPE3 OABB_1;__VTYPE3 OABB_2;__VTYPE3 OABB_3;__VTYPE3 OABB_4;
    __VTYPE3 OABB_5;__VTYPE3 OABB_6;__VTYPE3 OABB_7;__VTYPE3 OABB_8;

    OABB_1 = lv_dr->X
             -lv_dr->ox*lv_dr->lx
             -lv_dr->oy*lv_dr->ly
             -lv_dr->oz*lv_dr->lz;
    OABB_2 = lv_dr->X
             -lv_dr->ox*lv_dr->lx
             +lv_dr->oy*lv_dr->ly
             -lv_dr->oz*lv_dr->lz;
    OABB_3 = lv_dr->X
             +lv_dr->ox*lv_dr->lx
             +lv_dr->oy*lv_dr->ly
             -lv_dr->oz*lv_dr->lz;
    OABB_4 = lv_dr->X
             +lv_dr->ox*lv_dr->lx
             -lv_dr->oy*lv_dr->ly
             -lv_dr->oz*lv_dr->lz;

    OABB_5 = lv_dr->X
             -lv_dr->ox*lv_dr->lx
             -lv_dr->oy*lv_dr->ly
             +lv_dr->oz*lv_dr->lz;
    OABB_6 = lv_dr->X
             -lv_dr->ox*lv_dr->lx
             +lv_dr->oy*lv_dr->ly
             +lv_dr->oz*lv_dr->lz;
    OABB_7 = lv_dr->X
             +lv_dr->ox*lv_dr->lx
             +lv_dr->oy*lv_dr->ly
             +lv_dr->oz*lv_dr->lz;
    OABB_8 = lv_dr->X
             +lv_dr->ox*lv_dr->lx
             -lv_dr->oy*lv_dr->ly
             +lv_dr->oz*lv_dr->lz;

    DrawLineInGlobalCoordinates(OABB_1,OABB_2,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_2,OABB_3,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_3,OABB_4,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_4,OABB_1,CameraBasis);

    DrawLineInGlobalCoordinates(OABB_5,OABB_6,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_6,OABB_7,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_7,OABB_8,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_8,OABB_5,CameraBasis);

    DrawLineInGlobalCoordinates(OABB_1,OABB_5,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_2,OABB_6,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_3,OABB_7,CameraBasis);
    DrawLineInGlobalCoordinates(OABB_4,OABB_8,CameraBasis);
}

void SureWidget::DrawMeshLines(SureDrawable *lv_dr)
{
    cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
    cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей

    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

    for(uint cm = 0;cm<lv_dr->mesh_count;++cm){
        uint MeshID = lv_dr->mesh_start + cm;
        __SURE_VINT4 MeshData;
        __GET_MESH(MeshData,MeshID);
        my_double3 LocalVertex1;
        my_double3 LocalVertex2;
        my_double3 LocalVertex3;
        __GET_VERTEX(LocalVertex1,MeshData.x);
        __GET_VERTEX(LocalVertex2,MeshData.y);
        __GET_VERTEX(LocalVertex3,MeshData.z);
        __VTYPE3 GlobalVertex1 = EngineData->objects[EngineData->SelectedObject].drawable.X
             + LocalVertex1.x*EngineData->objects[EngineData->SelectedObject].drawable.ox*EngineData->objects[EngineData->SelectedObject].drawable.lx
             + LocalVertex1.y*EngineData->objects[EngineData->SelectedObject].drawable.oy*EngineData->objects[EngineData->SelectedObject].drawable.ly
             + LocalVertex1.z*EngineData->objects[EngineData->SelectedObject].drawable.oz*EngineData->objects[EngineData->SelectedObject].drawable.lz;
        __VTYPE3 GlobalVertex2 = EngineData->objects[EngineData->SelectedObject].drawable.X
             + LocalVertex2.x*EngineData->objects[EngineData->SelectedObject].drawable.ox*EngineData->objects[EngineData->SelectedObject].drawable.lx
             + LocalVertex2.y*EngineData->objects[EngineData->SelectedObject].drawable.oy*EngineData->objects[EngineData->SelectedObject].drawable.ly
             + LocalVertex2.z*EngineData->objects[EngineData->SelectedObject].drawable.oz*EngineData->objects[EngineData->SelectedObject].drawable.lz;
        __VTYPE3 GlobalVertex3 = EngineData->objects[EngineData->SelectedObject].drawable.X
             + LocalVertex3.x*EngineData->objects[EngineData->SelectedObject].drawable.ox*EngineData->objects[EngineData->SelectedObject].drawable.lx
             + LocalVertex3.y*EngineData->objects[EngineData->SelectedObject].drawable.oy*EngineData->objects[EngineData->SelectedObject].drawable.ly
             + LocalVertex3.z*EngineData->objects[EngineData->SelectedObject].drawable.oz*EngineData->objects[EngineData->SelectedObject].drawable.lz;
        DrawLineInGlobalCoordinates(GlobalVertex1,GlobalVertex2,CameraBasis);
        DrawLineInGlobalCoordinates(GlobalVertex2,GlobalVertex3,CameraBasis);
        DrawLineInGlobalCoordinates(GlobalVertex3,GlobalVertex1,CameraBasis);
    };
}

void SureWidget::DrawLinesDrawable(SureDrawable *lv_dr)
{

    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

        QPoint DrawPoint1;
        QPoint DrawPoint2;
        QPoint DrawPoint3;
        painter.setPen(Qt::white);
        switch (lv_dr->type){
            case SURE_DR_MESH:
            {
                if(lv_dr->mesh_count>50){
                    DrawOABB(lv_dr); // если граней больше 50 рисуем OABB
                }else{
                    DrawMeshLines(lv_dr); // рисуем все mesh'ы
                };
            };
            break;
            case SURE_DR_SQUARE:
            {
                    __VTYPE3 OABB_1;
                    __VTYPE3 OABB_2;
                    __VTYPE3 OABB_3;
                    __VTYPE3 OABB_4;

                    OABB_1 = lv_dr->X
                             -lv_dr->ox*lv_dr->lx
                             -lv_dr->oy*lv_dr->ly
                             -lv_dr->oz*lv_dr->lz;
                    OABB_2 = lv_dr->X
                             -lv_dr->ox*lv_dr->lx
                             +lv_dr->oy*lv_dr->ly
                             -lv_dr->oz*lv_dr->lz;
                    OABB_3 = lv_dr->X
                             +lv_dr->ox*lv_dr->lx
                             +lv_dr->oy*lv_dr->ly
                             -lv_dr->oz*lv_dr->lz;
                    OABB_4 = lv_dr->X
                             +lv_dr->ox*lv_dr->lx
                             -lv_dr->oy*lv_dr->ly
                             -lv_dr->oz*lv_dr->lz;

                    DrawLineInGlobalCoordinates(OABB_1,OABB_2,CameraBasis);
                    DrawLineInGlobalCoordinates(OABB_2,OABB_3,CameraBasis);
                    DrawLineInGlobalCoordinates(OABB_3,OABB_4,CameraBasis);
                    DrawLineInGlobalCoordinates(OABB_4,OABB_1,CameraBasis);
            };
            break;
            case SURE_DR_SPHERE:
            {
                QPoint DrawPoint;
                bool DrawPointVisible;

                __VTYPE3 GlobalPoint;
                for(float Angle=0.0;Angle<2*M_PI;Angle+=0.01){
                    GlobalPoint = lv_dr->X
                                 +lv_dr->ox*cos(Angle)*lv_dr->lx
                                 +lv_dr->oy*sin(Angle)*lv_dr->lx;
                    my_double3 LocalPoint = ConvertCoordinatesGlobalToCamera(GlobalPoint,CameraBasis,&DrawPointVisible);
                        if(DrawPointVisible){
                            DrawPoint.setX(LocalPoint.x);
                            DrawPoint.setY(LocalPoint.y);
                            painter.drawPoint(DrawPoint);
                        };
                    GlobalPoint = lv_dr->X
                                 +lv_dr->ox*cos(Angle)*lv_dr->lx
                                 +lv_dr->oz*sin(Angle)*lv_dr->lx;
                    LocalPoint = ConvertCoordinatesGlobalToCamera(GlobalPoint,CameraBasis,&DrawPointVisible);
                        if(DrawPointVisible){
                            DrawPoint.setX(LocalPoint.x);
                            DrawPoint.setY(LocalPoint.y);
                            painter.drawPoint(DrawPoint);
                        };
                    GlobalPoint = lv_dr->X
                                 +lv_dr->oy*cos(Angle)*lv_dr->lx
                                 +lv_dr->oz*sin(Angle)*lv_dr->lx;
                    LocalPoint = ConvertCoordinatesGlobalToCamera(GlobalPoint,CameraBasis,&DrawPointVisible);
                        if(DrawPointVisible){
                            DrawPoint.setX(LocalPoint.x);
                            DrawPoint.setY(LocalPoint.y);
                            painter.drawPoint(DrawPoint);
                        };
                };
            };
            break;
            default:
            break;
        }; // switch (EngineData->objects[EngineData->SelectedObject].drawable.type){
}

void SureWidget::DrawTraceLog(SureTraceLog *i_tl)
{
    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

    QPen l_Pen;
    l_Pen.setWidth(1);
    QColor l_Color;
    l_Pen.setColor(Qt::white);
    painter.setPen(l_Pen);
    for(int TraceItemID = 0;TraceItemID<(int)i_tl->ItemsCount;TraceItemID++){
        SureTraceLogItem *l_item = &i_tl->Items[TraceItemID];

        l_Color.setRed(l_item->rgb_current.x);
        l_Color.setGreen(l_item->rgb_current.y);
        l_Color.setBlue(l_item->rgb_current.z);
        l_Pen.setColor(l_Color);
        painter.setPen(l_Pen);

        __VTYPE3 CollisionPoint = l_item->TracePoint + l_item->TraceVector*l_item->IntersectDistance;
        DrawLineInGlobalCoordinates(l_item->TracePoint,CollisionPoint,CameraBasis);
        l_Pen.setColor(Qt::green);
        painter.setPen(l_Pen);
        __VTYPE3 NormPoint = CollisionPoint+10.0f*l_item->CollisionNomal;
        DrawLineInGlobalCoordinates(NormPoint,CollisionPoint,CameraBasis);
        l_Pen.setColor(Qt::red);
        painter.setPen(l_Pen);
        __VTYPE3 NormPointR = CollisionPoint+10.0f*l_item->NormalRandomized;
        DrawLineInGlobalCoordinates(NormPointR,CollisionPoint,CameraBasis);
        char txt[40];
        sprintf(txt,"%i(%i)",l_item->iter,l_item->rechecks);
        bool VisibleIndicator1;
        my_double3 LocalPoint1 = ConvertCoordinatesGlobalToCamera(CollisionPoint,CameraBasis,&VisibleIndicator1);
        if(VisibleIndicator1){
            painter.drawText((int)LocalPoint1.x,(int)LocalPoint1.y,txt);
        };
    };
}

void SureWidget::paintEvent(QPaintEvent * event)
{
    // засекаем время начала отрисовки
    clock_gettime(CLOCK_MONOTONIC,&framestart);
    // пересоздаем картинку
    delete image;
    int mx = rect().right();
    int my = rect().bottom();
    image = new QImage(mx,my,QImage::Format_ARGB32);
    // обновляем границы отрисовки (на случай если изменился размер окна)
    EngineData->CameraInfo.m_amx = rect().right()/SURE_SCALE;
    EngineData->CameraInfo.m_amy = rect().bottom()/SURE_SCALE;

    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

    PlaceMatrixToImage();

    char TextString[100];

    painter.begin(this);
    painter.drawImage(0,0,*image);

    // Информация о параметрах рендера
    if(EngineData->r_drawdebug>=40){
        painter.setPen(Qt::blue);
        sprintf(TextString,"X=%.4f Y=%.4f FOV=%.2f",EngineData->CameraInfo.cam_x.s[0],EngineData->CameraInfo.cam_x.s[1],EngineData->CameraInfo.xy_h);
        painter.drawText(5,30,TextString);
        if(OCLData->OpenCL){
            sprintf(TextString,"GPU(OpenCL %i)",EngineData->r_type);
        }else{
            sprintf(TextString,"CPU(OpenMP)");
        };
        painter.drawText(5,45,TextString);
        sprintf(TextString,"It=%d R=%d",EngineData->r_iters,EngineData->r_rechecks);
        painter.drawText(rect().right()-100,15,TextString);
        sprintf(TextString,"O=%d L=%d",EngineData->m_objects,EngineData->m_links);
        painter.drawText(rect().right()-100,30,TextString);
        sprintf(TextString,"Backlight=%.1f",EngineData->r_backlight);
        painter.drawText(rect().right()-100,45,TextString);
    };

    // Рисуем курсор
    if(!mousemove) painter.drawImage(QWidget::mapFromGlobal(QCursor::pos()),CursorImage);
    // Информация о курсоре -- направление, кординаты, выбранный объект
    if((EngineData->r_drawdebug>=50)&&(!mousemove)){
        painter.setPen(Qt::blue);
        sprintf(TextString,"Mx=%i My=%i",QWidget::mapFromGlobal(QCursor::pos()).x(),QWidget::mapFromGlobal(QCursor::pos()).y());
        painter.drawText(5,rect().bottom()-25,TextString);
        int x = QWidget::mapFromGlobal(QCursor::pos()).x()/SURE_SCALE;
        int y = QWidget::mapFromGlobal(QCursor::pos()).y()/SURE_SCALE;
        my_double3 tv = DetermineTraceVector(x,y,&EngineData->CameraInfo);
        sprintf(TextString,"TV = (%.3f;%.3f;%.3f)",tv.x,tv.y,tv.z);
        painter.drawText(5,rect().bottom()-15,TextString);
        EngineData->SelectObjectByScreenTrace(x,y,GPUData,Randomf);
        sprintf(TextString,"SelectedObject = %i",EngineData->SelectedObject);
        painter.drawText(5,rect().bottom()-5,TextString);
    };

    // Обводим выбранный объект
    if((EngineData->r_drawdebug>=50)
       &&(EngineData->SelectedObject>=0)
       &&(EngineData->SelectedObject<EngineData->m_objects)){
       DrawLinesDrawable(&EngineData->objects[EngineData->SelectedObject].drawable);
    };

    // Рисуем иннерциоиды
    if(EngineData->r_drawdebug>=70)
    for(int i = 0;i<EngineData->m_objects;++i){
        painter.setPen(Qt::blue);
        SureObject* lv_o = &EngineData->objects[i];
        DrawLineInGlobalCoordinates(lv_o->p1,lv_o->p2,CameraBasis);
        DrawLineInGlobalCoordinates(lv_o->p1,lv_o->p3,CameraBasis);
        DrawLineInGlobalCoordinates(lv_o->p1,lv_o->p4,CameraBasis);
        DrawLineInGlobalCoordinates(lv_o->p2,lv_o->p3,CameraBasis);
        DrawLineInGlobalCoordinates(lv_o->p2,lv_o->p4,CameraBasis);
        DrawLineInGlobalCoordinates(lv_o->p3,lv_o->p4,CameraBasis);
    };

    // Рисуем отладочную трассировку
    if((EngineData->r_drawdebug>=90)&&(EngineData->TraceLogsCount>0))
    for(int TraceLogID = 0;TraceLogID<(int)EngineData->TraceLogsCount;TraceLogID++)
        DrawTraceLog(&EngineData->TraceLogs[TraceLogID]);

    // отсекаем время отрисовки и выводим информацию о быстродействии
    clock_gettime(CLOCK_MONOTONIC,&frametime);
    posttime = frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );
    if(EngineData->r_drawdebug>=40){
        painter.setPen(Qt::blue);
        sprintf(TextString,"DR:%.2f ms PH:%.2f ms PS:%.2f ms",rendertime,EngineData->frametime,posttime);
        painter.drawText(QPoint(5,15),TextString);
    };

    painter.end();
}

void SureWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Plus){
        EngineData->CameraInfo.xy_h /= 1.1;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_Minus){
        EngineData->CameraInfo.xy_h *= 1.1;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_W){
        EngineData->cam_dx.x = 0.5;
    };
    if(event->key()==Qt::Key_S){
        EngineData->cam_dx.x = -0.5;
    };
    if(event->key()==Qt::Key_R){
        EngineData->cam_dx.z = 0.5;
    };
    if(event->key()==Qt::Key_F){
        EngineData->cam_dx.z = -0.5;
    };
    if(event->key()==Qt::Key_A){
        EngineData->cam_dx.y = -0.5;
    };
    if(event->key()==Qt::Key_D){
        EngineData->cam_dx.y = 0.5;
    };
    if(event->key()==Qt::Key_Q){
        EngineData->cam_dw.x = -0.01;
    };
    if(event->key()==Qt::Key_E){
        EngineData->cam_dw.x = 0.01;
    };
    if(event->key()==Qt::Key_Up){
        EngineData->cam_dw.y = 0.5;
    };
    if(event->key()==Qt::Key_Down){
        EngineData->cam_dw.y = -0.5;
    };
    if(event->key()==Qt::Key_Left){
        EngineData->cam_dw.z = -0.5;
    };
    if(event->key()==Qt::Key_Right){
        EngineData->cam_dw.z = 0.5;
    };
    if(event->key()==Qt::Key_8){
        EngineData->r_iters -= 2;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_9){
        EngineData->r_iters += 2;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_5){
        EngineData->r_rechecks -= 2;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_6){
        EngineData->r_rechecks += 2;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_2){
        EngineData->r_backlight -= 0.5;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_3){
        EngineData->r_backlight += 0.5;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_0){
        EngineData->r_drawdebug-=30;
        if(EngineData->r_drawdebug<30)EngineData->r_drawdebug=99;
    };
    if(event->key()==Qt::Key_O){
        QString Fname = "./screenshots/";
        char FileName[100];
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        strftime (FileName,100,"shot%y%m%d_%H%M%S.png",timeinfo);
        Fname += FileName;
        image->save(Fname);
    };
    if(event->key()==Qt::Key_M){
        if(mousemove){
            mousemove = false;
        }else{
            mousemove = true;
        };
    };
    if(event->key()==Qt::Key_P){
        if(EngineData->paused){
            EngineData->paused = false;
        }else{
            EngineData->paused = true;
        };
    };
    if(event->key()==Qt::Key_F5){
        EngineData->SaveState("initial");
    };
    if(event->key()==Qt::Key_F9){
        EngineData->LoadState("initial");
    };
    if(event->key()==Qt::Key_Backspace){
        EngineData->DeleteObject(EngineData->SelectedObject);
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_L){
        EngineData->reset = true;
        switch(EngineData->r_type)
        {
            case SURE_RT_NOCL:
            {
                EngineData->r_type = SURE_RT_D;
                OCLData->OpenCL = true;
                EngineData->Log->AddLine("Переключились на GPU-рендер вариант 1");
                break;
            };
            case SURE_RT_D:
            {
                EngineData->r_type = SURE_RT_T;
                OCLData->OpenCL = true;
                EngineData->Log->AddLine("Переключились на GPU-рендер вариант 2");
                break;
            };
            case SURE_RT_T:
            {
                EngineData->r_type = SURE_RT_F;
                OCLData->OpenCL = true;
                EngineData->Log->AddLine("Переключились на GPU-рендер вариант 3");
                break;
            };
            case SURE_RT_F:
            {
                EngineData->r_type = SURE_RT_N;
                OCLData->OpenCL = true;
                EngineData->Log->AddLine("Переключились на GPU-рендер вариант 4");
                break;
            };
            case SURE_RT_N:
            {
                EngineData->r_type = SURE_RT_NOCL;
                OCLData->OpenCL = false;
                EngineData->Log->AddLine("Переключились на СPU-рендер");
                break;
            };
            default:
            break;
        }; //SWITCH
        EngineData->reset = true;
    }; // Qt::Key_L
}

void SureWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up){
        EngineData->cam_dx.y = 0;
    };
    if(event->key()==Qt::Key_Down){
        EngineData->cam_dx.y = 0;
    };
    if(event->key()==Qt::Key_Left){
        EngineData->cam_dx.x = 0;
    };
    if(event->key()==Qt::Key_Right){
        EngineData->cam_dx.x = 0;
    };
    if(event->key()==Qt::Key_Q){
        EngineData->cam_dw.x = 0;
    };
    if(event->key()==Qt::Key_E){
        EngineData->cam_dw.x = 0;
    };
   if(event->key()==Qt::Key_W){
        EngineData->cam_dx.x = 0;
    };
    if(event->key()==Qt::Key_S){
        EngineData->cam_dx.x = 0;
    };
    if(event->key()==Qt::Key_R){
        EngineData->cam_dx.z = 0;
    };
    if(event->key()==Qt::Key_F){
        EngineData->cam_dx.z = 0;
    };
    if(event->key()==Qt::Key_A){
        EngineData->cam_dx.y = 0;
    };
    if(event->key()==Qt::Key_D){
        EngineData->cam_dx.y = 0;
    };
    if(event->key()==Qt::Key_Z){
        if(EngineData->CameraInfo.subp_rnd) {
            EngineData->CameraInfo.subp_rnd = false;
        } else {
            EngineData->CameraInfo.subp_rnd = true;
        };
    };
}

void SureWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!mousemove)return;
    QRect lv_rect = rect();
    if(last_x==0){
        last_x = event->x();
    };
    if(last_y==0){
        last_y = event->y();
    };
	EngineData->cam_dw.z = (float)(event->x()-last_x)*0.001;
	EngineData->cam_dw.y = (float)(last_y-event->y())*0.001;
	last_x = event->x();
	last_y = event->y();
    QCursor::setPos(QWidget::mapToGlobal(QPoint(lv_rect.width()/2,lv_rect.height()/2)));
    last_x = lv_rect.width()/2;
    last_y = lv_rect.height()/2;
}

void SureWidget::leaveEvent(QEvent *event)
{
    if(!mousemove)return;
    QRect lv_rect = rect();
    if(last_x==0){
        last_x = QCursor::pos().x();
    };
    if(last_y==0){
        last_y = QCursor::pos().y();
    };
	EngineData->cam_dw.z = (float)(QCursor::pos().x()-last_x)*0.001;
	EngineData->cam_dw.y = (float)(last_y-QCursor::pos().y())*0.001;
	last_x = QCursor::pos().x();
	last_y = QCursor::pos().y();
	if(last_x<20||last_x>(lv_rect.width()-20)||last_y<20||last_y>(lv_rect.bottom()-20)){
        QCursor::setPos(QWidget::mapToGlobal(QPoint(lv_rect.width()/2,lv_rect.height()/2)));
        last_x = lv_rect.width()/2;
        last_y = lv_rect.height()/2;
    };

}

void SureWidget::mousePressEvent(QMouseEvent *event)
{
    if(mousemove){
        if (event->button() == Qt::LeftButton) {
            EngineData->TemplateObject.ox = EngineData->CameraInfo.cam_vec;
            EngineData->TemplateObject.oz = EngineData->CameraInfo.cam_upvec;
            EngineData->TemplateObject.oy = cross(EngineData->CameraInfo.cam_vec,EngineData->CameraInfo.cam_upvec);
            __VTYPE3 X = EngineData->CameraInfo.cam_x;
            uint o = EngineData->CreateObjectFromTemplate(&X);
            EngineData->ObjByID(o)->push(EngineData->ObjByID(o)->X,EngineData->CameraInfo.cam_vec,3.0);
        };
        if (event->button() == Qt::RightButton) {
            EngineData->SetNextTemplate();
            EngineData->reset = true;
        };
    }else{ // if mousemove
        if (event->button() == Qt::LeftButton) {
            int x = QWidget::mapFromGlobal(QCursor::pos()).x()/SURE_SCALE;
            int y = QWidget::mapFromGlobal(QCursor::pos()).y()/SURE_SCALE;
            EngineData->AddTraceLog(x,y,GPUData,Randomf,true);
        }; // (event->button() == Qt::LeftButton)
        if (event->button() == Qt::RightButton) {
            int x = QWidget::mapFromGlobal(QCursor::pos()).x()/SURE_SCALE;
            int y = QWidget::mapFromGlobal(QCursor::pos()).y()/SURE_SCALE;
            EngineData->AddTraceLog(x,y,GPUData,Randomf,false);
        }; // (event->button() == Qt::LeftButton)
    }; // if !mousemove
} // mousePressEvent
