#include "SureWidget.h"

SureWidget::SureWidget(SureData* i_engine)
{
    EngineData = i_engine;
    EngineData->widget = this;
    //setWindowFlags(Qt::Popup;

    int LWidth = 800;
    int LHeight = 450;
    EngineData->CurrentWidth=LWidth;
    EngineData->CurrentWidth=LWidth;
    EngineData->CurrentHeight=LHeight;
    resize(LWidth,LHeight);

    image = new QImage(EngineData->CurrentWidth,EngineData->CurrentHeight,QImage::Format_ARGB32);
    CursorImage.load("./maps/cursor.png");
    CheckedImge.load("./maps/checked.png");
    LoadingScreen.load("./maps/loading.png");
    setMouseTracking(true);
    setCursor(QCursor(Qt::BlankCursor));
    this->update();
}

SureWidget::~SureWidget(){
    delete image;
}

void SureWidget::resizeEvent(QResizeEvent* event)
{
    EngineData->reset = true;
}

my_double3 SureWidget::ConvertCoordinatesGlobalToCamera(my_double3 GlobalPoint,my_double3 *CameraBasis,bool *VisibleIndicator)
{
    my_double3 Result = {0,0,0};
    my_double3 VectorFromPoint = EngineData->GPUData.CameraInfo.cam_x-GlobalPoint;
    double DistanceToPoint = dot(CameraBasis[2],VectorFromPoint);
    if(DistanceToPoint<0.0f){
        *VisibleIndicator = true;
        Result.x = EngineData->CurrentWidth/2 + EngineData->CurrentWidth*((dot(CameraBasis[0],VectorFromPoint) / DistanceToPoint) / EngineData->GPUData.CameraInfo.xy_h);
        Result.y = EngineData->CurrentHeight/2 + EngineData->CurrentWidth*((dot(CameraBasis[1],VectorFromPoint) / DistanceToPoint) / EngineData->GPUData.CameraInfo.xy_h);
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
    CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
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
    CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
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
        SureObject* LocalSelectedObject = EngineData->ObjByID(EngineData->SelectedObject);
        __VTYPE3 GlobalVertex1 = LocalSelectedObject->drawable.X
             + LocalVertex1.x*LocalSelectedObject->drawable.ox*LocalSelectedObject->drawable.lx
             + LocalVertex1.y*LocalSelectedObject->drawable.oy*LocalSelectedObject->drawable.ly
             + LocalVertex1.z*LocalSelectedObject->drawable.oz*LocalSelectedObject->drawable.lz;
        __VTYPE3 GlobalVertex2 = LocalSelectedObject->drawable.X
             + LocalVertex2.x*LocalSelectedObject->drawable.ox*LocalSelectedObject->drawable.lx
             + LocalVertex2.y*LocalSelectedObject->drawable.oy*LocalSelectedObject->drawable.ly
             + LocalVertex2.z*LocalSelectedObject->drawable.oz*LocalSelectedObject->drawable.lz;
        __VTYPE3 GlobalVertex3 = LocalSelectedObject->drawable.X
             + LocalVertex3.x*LocalSelectedObject->drawable.ox*LocalSelectedObject->drawable.lx
             + LocalVertex3.y*LocalSelectedObject->drawable.oy*LocalSelectedObject->drawable.ly
             + LocalVertex3.z*LocalSelectedObject->drawable.oz*LocalSelectedObject->drawable.lz;
        DrawLineInGlobalCoordinates(GlobalVertex1,GlobalVertex2,CameraBasis);
        DrawLineInGlobalCoordinates(GlobalVertex2,GlobalVertex3,CameraBasis);
        DrawLineInGlobalCoordinates(GlobalVertex3,GlobalVertex1,CameraBasis);
    };
}

void SureWidget::DrawDebugSelectedObject(SureObject *o)
{
    SureDrawable *lv_dr = &o->drawable;
    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

        QPoint DrawPoint1;
        QPoint DrawPoint2;
        QPoint DrawPoint3;
        painter.setPen(Qt::white);
        if(o->type == SURE_OBJ_PS){
            DrawOABB(lv_dr);
        }else{
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
            }; // switch (LocalSelectedObject->drawable.type){
        }; // объект -- не SURE_OBJ_PS
}

void SureWidget::DrawTraceLog(SureTraceLog *i_tl)
{
    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
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
    clock_gettime(CLOCK_MONOTONIC,&EngineData->framestart);
    // пересоздаем картинку

    int LWidth = rect().width();
    int LHeight = rect().height();
    if((EngineData->CurrentWidth!=LWidth)||(EngineData->CurrentHeight!=LHeight)){
        delete image;
        EngineData->CurrentWidth=LWidth;
        EngineData->CurrentHeight=LHeight;
        image = new QImage(LWidth,LHeight,QImage::Format_ARGB32);
    }

    // обновляем границы отрисовки (на случай если изменился размер окна)
    //EngineData->CameraInfo.m_amx = EngineData->CurrentWidth/EngineData->ImageScale;
    //EngineData->CameraInfo.m_amy = EngineData->CurrentHeight/EngineData->ImageScale;

    if(EngineData->Loading){
        painter.begin(this);
        painter.drawImage(0,0,LoadingScreen.scaled(EngineData->CurrentWidth,EngineData->CurrentHeight));
        painter.end();
        return;
    };

    my_double3 CameraBasis[3];
    CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
    CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
    CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);

    SureMatrixToImage(EngineData->rgbmatrix,image,EngineData->CurrentWidth,EngineData->CurrentHeight,EngineData->ImageScale);

    char TextString[100];

    painter.begin(this);
    painter.drawImage(0,0,*image);

    // Информация о параметрах рендера
    if(EngineData->DrawDebugFPS){

    };
    if(EngineData->DrawDebugMode){
        painter.setPen(Qt::blue);
        sprintf(TextString,"X=%.4f Y=%.4f FOV=%.2f",EngineData->GPUData.CameraInfo.cam_x.s[0],EngineData->GPUData.CameraInfo.cam_x.s[1],EngineData->GPUData.CameraInfo.xy_h);
        painter.drawText(5,30,TextString);
        if(EngineData->OCLData.OpenCL){
            sprintf(TextString,"GPU(OpenCL %i)",EngineData->r_type);
        }else{
            sprintf(TextString,"CPU(OpenMP)");
        };
        painter.drawText(5,45,TextString);
        sprintf(TextString,"It=%d R=%d",EngineData->GPUData.r_maxiters,EngineData->GPUData.r_rechecks);
        painter.drawText(EngineData->CurrentWidth-100,15,TextString);
        sprintf(TextString,"Backlight=%.1f",EngineData->GPUData.r_backlight);
        painter.drawText(EngineData->CurrentWidth-100,45,TextString);
    };

    if(EngineData->DrawDebugPhysicsInfo){
        painter.setPen(Qt::blue);
        sprintf(TextString,"O=%d L=%d",EngineData->m_objects,EngineData->m_links);
        painter.drawText(EngineData->CurrentWidth-100,30,TextString);
    }

    // Информация о курсоре -- направление, кординаты, выбранный объект
    if((EngineData->DrawDebugCursorInfo)&&(!EngineData->mousemove))
    if(EngineData->MenuWindowsCounter==0){
        painter.setPen(Qt::blue);
        sprintf(TextString,"Mx=%i My=%i",QWidget::mapFromGlobal(QCursor::pos()).x(),QWidget::mapFromGlobal(QCursor::pos()).y());
        painter.drawText(5,EngineData->CurrentHeight-25,TextString);
        int x = QWidget::mapFromGlobal(QCursor::pos()).x()/EngineData->ImageScale;
        int y = QWidget::mapFromGlobal(QCursor::pos()).y()/EngineData->ImageScale;
        my_double3 tv = DetermineTraceVector(x,y,&EngineData->GPUData.CameraInfo);
        sprintf(TextString,"TV = (%.3f;%.3f;%.3f)",tv.x,tv.y,tv.z);
        painter.drawText(5,EngineData->CurrentHeight-15,TextString);
    };

    if(EngineData->DrawDebugSelectedObject)
    if(EngineData->MenuWindowsCounter==0)
    if(EngineData->SelectedObject>=0){
        painter.setPen(Qt::blue);
        sprintf(TextString,"SelectedObject = %i",EngineData->SelectedObject);
        painter.drawText(5,EngineData->CurrentHeight-5,TextString);
    };

    // Обводим выбранный объект
    if((EngineData->DrawDebugSelectedObject)
       &&(EngineData->SelectedObject>=0))
       DrawDebugSelectedObject(EngineData->ObjByID(EngineData->SelectedObject));

    if(EngineData->DrawDebugAllObjects)
    for(int objid = 0;objid<EngineData->m_objects;++objid)
       DrawDebugSelectedObject(&EngineData->objects[objid]);

    if(EngineData->DrawDebugLinks){
        my_double3 CameraBasis[3];
        CameraBasis[1] = -EngineData->GPUData.CameraInfo.cam_upvec;
        CameraBasis[2] = EngineData->GPUData.CameraInfo.cam_vec;
        CameraBasis[0] = cross(CameraBasis[2],CameraBasis[1]);
        for(int il=0;il<EngineData->m_links;++il){
            painter.setPen(Qt::yellow);
            SureLink* LocalLink = &EngineData->links[il];
            SureObject* o1 = EngineData->ObjByID(LocalLink->Object1);
            SureObject* o2 = EngineData->ObjByID(LocalLink->Object2);
            DrawLineInGlobalCoordinates(o1->X,o2->X,CameraBasis);
        };
    };

    // Рисуем иннерциоиды
    if(EngineData->DrawDebugPhysicsTetrs)
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
    if((EngineData->DrawDebugTraces)&&(EngineData->TraceLogsCount>0))
    for(int TraceLogID = 0;TraceLogID<(int)EngineData->TraceLogsCount;TraceLogID++)
        DrawTraceLog(&EngineData->TraceLogs[TraceLogID]);

    // отсекаем время отрисовки и выводим информацию о быстродействии
    clock_gettime(CLOCK_MONOTONIC,&EngineData->frametime_time);
    EngineData->posttime = EngineData->frametime_time.tv_sec * 1000.0
               + (float) EngineData->frametime_time.tv_nsec / 1000000.0
               - (  EngineData->framestart.tv_sec*1000.0 + (float) EngineData->framestart.tv_nsec / 1000000.0 );
    if(EngineData->DrawDebugFPS){
        painter.setPen(Qt::blue);
        sprintf(TextString,"DR:%.2f ms PH:%.2f ms PS:%.2f ms",EngineData->rendertime,EngineData->frametime_f,EngineData->posttime);
        painter.drawText(QPoint(5,15),TextString);
    };

    DrawMenu();
    // Рисуем курсор
    if(!EngineData->mousemove) painter.drawImage(QWidget::mapFromGlobal(QCursor::pos()),CursorImage);

    painter.end();
}

void SureWidget::DrawMenu(){
    for(int i = 0;i<EngineData->MenuWindowsCounter;++i){
      SureMenuWindow* LocalWindow = EngineData->MenuWindows[i];
      QColor LocalColor;
      LocalColor.setAlpha(155);
      LocalColor.setRed(150);
      LocalColor.setGreen(150);
      LocalColor.setBlue(150);
      QRect LocalRect;
      LocalRect.setRect(LocalWindow->x,LocalWindow->y,LocalWindow->width,LocalWindow->height);
      painter.fillRect(LocalRect,LocalColor);
      painter.setPen(Qt::white);
      int MaximumBottom = LocalWindow->height;
      int MaximumRight = LocalWindow->width;
      for(int j = 0;j<EngineData->MenuWindows[i]->ElementsCounter;++j){
        SureMenuElement* LocalElement = EngineData->MenuWindows[i]->Elements[j];
        if((LocalWindow->x+LocalElement->x)>MaximumRight)MaximumRight = LocalWindow->x+LocalElement->x;
        if((LocalWindow->y+LocalElement->y)>MaximumBottom)MaximumBottom = LocalWindow->y+LocalElement->y;
        LocalRect.setRect(  LocalWindow->x+LocalElement->x,
                            LocalWindow->y+LocalElement->y-LocalWindow->ScrollPosition,
                            LocalElement->width,
                            LocalElement->height);
        if((LocalRect.top()>(5+LocalWindow->y))&&(LocalRect.bottom()<(LocalWindow->height+LocalWindow->y-10)))
        if((LocalRect.left()>(5+LocalWindow->x))&&(LocalRect.right()<(LocalWindow->width+LocalWindow->x-10))){ // видимый
            if(LocalElement->type==SUREWINDOW_BUTTON){
                painter.fillRect(LocalRect,LocalColor);
                painter.drawText(LocalRect,Qt::AlignCenter,QString::fromWCharArray(LocalElement->text));
                if(LocalElement->selected)
                    painter.drawRect(LocalRect);
            };
            if(LocalElement->type==SUREWINDOW_CHECKBOX){
                painter.fillRect(LocalRect,LocalColor);
                SureMenuCheckbox* LocalCheckbox = (SureMenuCheckbox*)LocalElement;
                if(*LocalCheckbox->EditingValue){
                    painter.drawImage(LocalRect.left(),LocalRect.top(),CheckedImge.scaled(LocalRect.width(),LocalRect.height()));
                };
                if(LocalElement->selected)
                    painter.drawRect(LocalRect);
            };
            if(LocalElement->type==SUREWINDOW_LABEL){
                painter.drawText(LocalRect,Qt::AlignLeft,QString::fromWCharArray(LocalElement->text));
            };
            if(LocalElement->type==SUREWINDOW_NUMBEREDIT){
                painter.fillRect(LocalRect,LocalColor);
                QRect NewLocalRect;
                NewLocalRect.setRect(LocalRect.left()+5,
                                     LocalRect.top()+1,
                                     LocalRect.width()-2*LocalRect.height(),
                                     LocalRect.height()-2);
                painter.drawText(NewLocalRect,Qt::AlignLeft,QString::fromWCharArray(LocalElement->text));
                if(LocalElement->selected)
                    painter.drawRect(LocalRect);
                NewLocalRect.setRect(LocalRect.right()-LocalRect.height(),
                                     LocalRect.top()+1,
                                     LocalRect.height()-2,
                                     LocalRect.height()-2);
                painter.fillRect(NewLocalRect,LocalColor);
                painter.drawText(NewLocalRect,Qt::AlignCenter,"+");

                NewLocalRect.setRect(LocalRect.right()-2*LocalRect.height(),
                                     LocalRect.top()+1,
                                     LocalRect.height()-2,
                                     LocalRect.height()-2);
                painter.fillRect(NewLocalRect,LocalColor);
                painter.drawText(NewLocalRect,Qt::AlignCenter,"-");
            };
        };// Элемент видимый
      }; // Рисуем все элементы
      if(MaximumBottom>(LocalWindow->height)){ // если элементы вылезли за нижнюю границу окна
        float VisibleAreaPercent = (float)LocalWindow->height / (float)MaximumBottom;
        float ScrollPositionPercent = (float)LocalWindow->ScrollPosition / (float)MaximumBottom;
        LocalRect.setRect(LocalWindow->x+LocalWindow->width - 10,
                          LocalWindow->y+5,
                          5,
                          LocalWindow->height-10);
        painter.fillRect(LocalRect,Qt::darkGray);
        int Lheight = VisibleAreaPercent*(LocalWindow->height-10);
        int BottomCandidate = LocalWindow->y+5+ScrollPositionPercent*(LocalWindow->height-10) + Lheight;
        int BottomLimit = LocalWindow->y+5 + LocalWindow->height-10;
        if(BottomCandidate>BottomLimit)
            Lheight -= (BottomCandidate-BottomLimit);
        LocalRect.setRect(LocalWindow->x+LocalWindow->width-10,
                          LocalWindow->y+5+ScrollPositionPercent*(LocalWindow->height-10),
                          5,
                          Lheight);
        painter.fillRect(LocalRect,Qt::white);
      };// если элементы вылезли за нижнюю границу окна
    };
}

void SureWidget::keyPressEvent(QKeyEvent *event){

    SureControllerAction Action;
    SureControllerInput Input;
    Input.type = SUREINPUT_KEYDOWN;
    Input.key = event->key();
    Input.x = 1.0;

    EngineData->HandleInput(&Input);

    if(event->key()==Qt::Key_Escape){
        if(EngineData->MenuWindowsCounter>0){ // есть окна меню
            delete EngineData->MenuWindows[--EngineData->MenuWindowsCounter];
            if(EngineData->MenuWindowsCounter==0){
                EngineData->paused = false;
                EngineData->mousemove = true;
            };
        }else{ // нет окон меню
            EngineData->paused = true;
            EngineData->mousemove = false;
            EngineData->MenuWindows[EngineData->MenuWindowsCounter] = new SureMainMenuWindow(EngineData);
            SureMainMenuWindow* NewWindow = (SureMainMenuWindow*)EngineData->MenuWindows[EngineData->MenuWindowsCounter++];
            NewWindow->x = rect().width()*0.05;
            NewWindow->y = rect().height()*0.05;
            NewWindow->width = rect().width()*0.9;
            NewWindow->height = rect().height()*0.9;
            NewWindow->Rebuild();
        };// если нет открытых окон
    };// Escape
}

void SureWidget::keyReleaseEvent(QKeyEvent *event){
    SureControllerAction Action;
    SureControllerInput Input;
    Input.type = SUREINPUT_KEYUP;
    Input.key = event->key();
    EngineData->HandleInput(&Input);
}

void SureWidget::mouseMoveEvent(QMouseEvent *event){
    if(EngineData->MenuWindowsCounter>0){
        SureMenuWindow* CurrentWindow = EngineData->MenuWindows[EngineData->MenuWindowsCounter-1];
        for(int i = 0;i<CurrentWindow->ElementsCounter;++i){
            SureMenuElement* CurrentElement = CurrentWindow->Elements[i];
            CurrentElement->selected = false;
            int Lleft = CurrentWindow->x + CurrentElement->x;
            int Ltop = CurrentWindow->y + CurrentElement->y - CurrentWindow->ScrollPosition;
            int Lright = Lleft + CurrentElement->width;
            int Lbottom = Ltop + CurrentElement->height;
            if(event->x()>Lleft&&event->x()<Lright&&event->y()>Ltop&&event->y()<Lbottom)
                CurrentElement->selected = true;
        };
    };
    if(!EngineData->mousemove)return;
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
    if(!EngineData->mousemove)return;
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

void SureWidget::wheelEvent(QWheelEvent *event)
{
    SureControllerAction a;
    SureControllerInput i;
    i.key = 0;

    if(event->delta()<0){
        i.type = SUREINPUT_SCROLLDOWN;
        a.type = SUREACTION_MENUSCROLLDOWN;
        EngineData->AssignControl(&i,&a);
        i.x = -event->delta();
        EngineData->ExecuteAction(&a,&i);
    }else if(event->delta()>0){
        i.type = SUREINPUT_SCROLLUP;
        a.type = SUREACTION_MENUSCROLLUP;
        EngineData->AssignControl(&i,&a);
        i.x = event->delta();
        EngineData->ExecuteAction(&a,&i);
    };
}

void SureWidget::mousePressEvent(QMouseEvent *event)
{
    if(EngineData->MenuWindowsCounter>0){ // Открыто меню
        SureMenuWindow* CurrentWindow = EngineData->MenuWindows[EngineData->MenuWindowsCounter - 1];
        CurrentWindow->OnClick(event->button());
        for(int i = 0;i< CurrentWindow->ElementsCounter;++i)
        if(CurrentWindow->Elements[i]->selected){
            if(CurrentWindow->Elements[i]->type==SUREWINDOW_NUMBEREDIT){
                SureMenuNumberEdit* LocalNumberEdit = (SureMenuNumberEdit*)CurrentWindow->Elements[i];
                int x = QWidget::mapFromGlobal(QCursor::pos()).x();
                x-=CurrentWindow->x;
                //int y = QWidget::mapFromGlobal(QCursor::pos()).y();
                QRect LocalRect;
                LocalRect.setRect(LocalNumberEdit->x+LocalNumberEdit->width-LocalNumberEdit->height+1,
                                  LocalNumberEdit->y+1,
                                  LocalNumberEdit->height-2,
                                  LocalNumberEdit->height-2);
                if(x<LocalRect.right()&&x>LocalRect.left())
                    LocalNumberEdit->OnClick(1); // плюс
                LocalRect.setRect(LocalNumberEdit->x+LocalNumberEdit->width-2*LocalNumberEdit->height+1,
                                  LocalNumberEdit->y+1,
                                  LocalNumberEdit->height-2,
                                  LocalNumberEdit->height-2);
                if(x<LocalRect.right()&&x>LocalRect.left())
                    LocalNumberEdit->OnClick(0); // минус
            }else{ // (CurrentWindow->Elements[i]->type!=SUREWINDOW_NUMBEREDIT)
                CurrentWindow->Elements[i]->OnClick();
            };
        };// (CurrentWindow->Elements[i]->selected)
    }else{
        SureControllerAction Action;
        SureControllerInput Input;
        Input.type = SUREINPUT_MBUTDOWN;
        Input.key = event->button();
        Input.x = QWidget::mapFromGlobal(QCursor::pos()).x()/EngineData->ImageScale;
        Input.y = QWidget::mapFromGlobal(QCursor::pos()).y()/EngineData->ImageScale;
        EngineData->HandleInput(&Input);
    }; // Меню не открыто
} // mousePressEvent
