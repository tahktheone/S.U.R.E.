#include "SureWidget.h"

SureWidget::SureWidget()
{
    this->update();
}

SureWidget::~SureWidget()
{
    delete rgbmatrix;
}

void SureWidget::paintEvent(QPaintEvent * event)
{
    int x = rect().right();
    int y = rect().bottom();
    uchar* lv_pixel;
    delete image;
    image = new QImage(x,y,QImage::Format_ARGB32);
    EngineData->CameraInfo.m_amx = rect().right()*SURE_FAA/SURE_SCALE;
    EngineData->CameraInfo.m_amy = rect().bottom()*SURE_FAA/SURE_SCALE;
    clock_gettime(CLOCK_MONOTONIC,&framestart);
    double lv_max = 0;
    double lv_med = 0;

                    QPoint P1;
                    QPoint P2;
                    bool VisibleIndicator1;
                    bool VisibleIndicator2;
                    #define DRAW_LINE(I_GP1,I_GP2) \
                        CONVERT_TO_CAMERA_XY(I_GP1,LocalPoint1,VisibleIndicator1); \
                        CONVERT_TO_CAMERA_XY(I_GP2,LocalPoint2,VisibleIndicator2); \
                        if(VisibleIndicator1&&VisibleIndicator2){ \
                            P1.setX(LocalPoint1.x); \
                            P1.setY(LocalPoint1.y); \
                            P2.setX(LocalPoint2.x); \
                            P2.setY(LocalPoint2.y); \
                            painter.drawLine(P1,P2);\
                        };

                //float* rgbmatrix = widget->rgbmatrix;
                SureDrawable* Drawables = GPUData->Drawables;
                cl_uchar* Textures = EngineData->TexturesData; // Текстуры
                cl_float* UVMap = EngineData->UVMap; // мэппинг мешей на текстуры
                cl_float* Normals = EngineData->Normals; // мэппинг мешей на текстуры
                cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
                cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей


            my_double3 CameraDX;
            my_double3 CameraDY;
            my_double3 CameraDZ;
            my_double3 VectorFromPoint;
            float DistanceToPoint;
            my_double3 LocalPoint;
            my_double3 LocalPoint1;
            my_double3 LocalPoint2;
            my_double3 LocalPoint3;
            my_double3 LocalPoint4;
            int my = image->rect().bottom();
            int mx = image->rect().right();

            #define CONVERT_TO_CAMERA_XY(I_GLOBAL_POINT,E_LOCAL_POINT,I_VISIBLE) \
            CameraDY = -EngineData->CameraInfo.cam_upvec; \
            CameraDZ = EngineData->CameraInfo.cam_vec; \
            CameraDX = cross(CameraDZ,CameraDY); \
            VectorFromPoint = EngineData->CameraInfo.cam_x-I_GLOBAL_POINT; \
            DistanceToPoint = dot(CameraDZ,VectorFromPoint); \
            if(DistanceToPoint<0.0f){ \
                I_VISIBLE = true; \
                E_LOCAL_POINT.x = mx/2 + mx*__DIVIDE(__DIVIDE(dot(CameraDX,VectorFromPoint),DistanceToPoint),EngineData->CameraInfo.xy_h); \
                E_LOCAL_POINT.y = my/2 + mx*__DIVIDE(__DIVIDE(dot(CameraDY,VectorFromPoint),DistanceToPoint),EngineData->CameraInfo.xy_h); \
            }else{ \
                I_VISIBLE = false; \
            };

    for(y=0;y<my;++y)for(x=0;x<mx;++x)
    {
        double l_m = lv_max;
        double lv_mm = lv_med;
        int k = y*3*SURE_MAXRES_X*SURE_FAA*SURE_FAA+x*3*SURE_FAA;
        lv_max = rgbmatrix[k] > lv_max ? rgbmatrix[k] : lv_max;
        lv_max = rgbmatrix[k+1] > lv_max ? rgbmatrix[k+1] : lv_max;
        lv_max = rgbmatrix[k+2] > lv_max ? rgbmatrix[k+2] : lv_max;
        lv_med += rgbmatrix[k] + rgbmatrix[k+1] + rgbmatrix[k+2];
        if(isnan(lv_max))
        {
            lv_max=l_m;
        };
        if(isnan(lv_med))
        {
            lv_med=lv_mm;
        };
    };
    lv_med /= mx*my*3;
    lv_max /= (lv_max/lv_med)*150.0;
    //lv_max /= 255.0;
    #pragma omp parallel for schedule(dynamic,1) private (lv_pixel,x,y)
    for(y=0;y<my;++y){
        uint32_t* scanLine = (uint32_t*)image->scanLine(y);
        for(x=0;x<mx;++x){
            float r=0;
            float g=0;
            float b=0;
            for(int i_aax=0;i_aax<SURE_FAA;++i_aax)
            for(int i_aay=0;i_aay<SURE_FAA;++i_aay)
            {
                r += rgbmatrix[y*3*SURE_MAXRES_X*SURE_FAA*SURE_FAA-i_aay*3*SURE_MAXRES_X*SURE_FAA+x*3*SURE_FAA-i_aax*3];
                g += rgbmatrix[y*3*SURE_MAXRES_X*SURE_FAA*SURE_FAA-i_aay*3*SURE_MAXRES_X*SURE_FAA+x*3*SURE_FAA-i_aax*3+1];
                b += rgbmatrix[y*3*SURE_MAXRES_X*SURE_FAA*SURE_FAA-i_aay*3*SURE_MAXRES_X*SURE_FAA+x*3*SURE_FAA-i_aax*3+2];
            };
            r /= SURE_FAA*SURE_FAA*lv_max;
            g /= SURE_FAA*SURE_FAA*lv_max;
            b /= SURE_FAA*SURE_FAA*lv_max;
            if(r>255)r=255;
            if(g>255)g=255;
            if(b>255)b=255;
            if(r<0)r=0;
            if(g<0)g=0;
            if(b<0)b=0;
            lv_pixel = (uchar*)scanLine;
            *lv_pixel = (uchar)b;
            ++lv_pixel;
            *lv_pixel = (uchar)g;
            ++lv_pixel;
            *lv_pixel = (uchar)r;
            ++lv_pixel;
            *lv_pixel = 255;
            ++scanLine;
        }; //X
    }; //Y
    uint32_t* l_img;
    l_img = (uint32_t*)image->bits();
    bool shift = true;
    bool shiftp = true;
    for(int sm=0;sm<SURE_SMOOTH;++sm)
    {
        if(shiftp)
        {
            shift=true;
            shiftp=false;
        }
        else
        {
            shift = false;
            shiftp = true;
        };
        #pragma omp parallel for schedule(dynamic,1) private (shift,lv_pixel,x,y)
        for(y=1;y<(my-1);++y)
        {
            if(sm%2==1)
            {
                shift = false;
            }
            else
            {
                shift = true;
            };
            for(x=shift?2:1;x<(mx-1);++x)
            {
                int r=0;
                int g=0;
                int b=0;
                lv_pixel = (uchar*)(&l_img[y*(mx+1)+x]);
                b += *lv_pixel;++lv_pixel;
                g += *lv_pixel;++lv_pixel;
                r += *lv_pixel;++lv_pixel;
                lv_pixel = (uchar*)(&l_img[(y-1)*(mx+1)+x]);
                b += *lv_pixel;++lv_pixel;
                g += *lv_pixel;++lv_pixel;
                r += *lv_pixel;++lv_pixel;
                lv_pixel = (uchar*)(&l_img[(y+1)*(mx+1)+x]);
                b += *lv_pixel;++lv_pixel;
                g += *lv_pixel;++lv_pixel;
                r += *lv_pixel;++lv_pixel;
                lv_pixel = (uchar*)(&l_img[y*(mx+1)+(x-1)]);
                b += *lv_pixel;++lv_pixel;
                g += *lv_pixel;++lv_pixel;
                r += *lv_pixel;++lv_pixel;
                lv_pixel = (uchar*)(&l_img[y*(mx+1)+(x+1)]);
                b += *lv_pixel;++lv_pixel;
                g += *lv_pixel;++lv_pixel;
                r += *lv_pixel;++lv_pixel;
                lv_pixel = (uchar*)(&l_img[y*(mx+1)+x]);
                r /= 5; g /=5; b /=5;
                *lv_pixel = (uchar)b;++lv_pixel;
                *lv_pixel = (uchar)g;++lv_pixel;
                *lv_pixel = (uchar)r;++lv_pixel;
            };  //X
        }; //Y
    };//SMOOTH
    clock_gettime(CLOCK_MONOTONIC,&frametime);
    posttime = frametime.tv_sec * 1000.0 + (float) frametime.tv_nsec / 1000000.0 - (  framestart.tv_sec*1000.0 + (float) framestart.tv_nsec / 1000000.0 );
    painter.begin(this);
    painter.drawImage(0,0,*image);
    QPoint p;
    char s[100];
    if(EngineData->r_drawdebug>=40)
    {
    p.setX(5);
    p.setY(15);
    painter.setPen(Qt::blue);
    sprintf(s,"DR:%.2f ms PH:%.2f ms PS:%.2f ms",rendertime,EngineData->frametime,posttime);
    painter.drawText(p,s);
    p.setY(30);
    sprintf(s,"X=%.4f Y=%.4f FOV=%.2f",EngineData->CameraInfo.cam_x.s[0],EngineData->CameraInfo.cam_x.s[1],EngineData->CameraInfo.xy_h);
    painter.drawText(p,s);
    p.setY(45);
    if(OCLData->OpenCL)
    {
        sprintf(s,"GPU(OpenCL)");
        if(EngineData->r_type==SURE_RT_D)
            sprintf(s,"GPU(OpenCL-1)");
        if(EngineData->r_type==SURE_RT_T)
            sprintf(s,"GPU(OpenCL-2)");
        if(EngineData->r_type==SURE_RT_F)
            sprintf(s,"GPU(OpenCL-3)");
        if(EngineData->r_type==SURE_RT_N)
            sprintf(s,"GPU(OpenCL-4)");
    }else{
        sprintf(s,"CPU(OpenMP)");
    };
    painter.drawText(p,s);
    p.setY(15);
    p.setX(rect().right()-100);
    sprintf(s,"It=%d R=%d",EngineData->r_iters,EngineData->r_rechecks);
    painter.drawText(p,s);
    p.setY(30);
    sprintf(s,"O=%d L=%d",EngineData->m_objects,EngineData->m_links);
    painter.drawText(p,s);
    p.setY(45);
    sprintf(s,"Backlight=%.1f",EngineData->r_backlight);
    painter.drawText(p,s);
    };
    // TODO: рисовать линки
    //for(int i = 0;i<EngineData->m_links;++i)
    //{
        //EngineData->links[i].o1->X;
    //}
    // Рисуем курсор
    if(!mousemove)
    {
        painter.setPen(Qt::green);
        painter.drawEllipse(QWidget::mapFromGlobal(QCursor::pos()),1,1);
        painter.setPen(Qt::red);
        painter.drawEllipse(QWidget::mapFromGlobal(QCursor::pos()),3,3);
        painter.setPen(Qt::yellow);
        painter.drawEllipse(QWidget::mapFromGlobal(QCursor::pos()),5,5);
        painter.setPen(Qt::blue);
        if(EngineData->r_drawdebug>=50)
        {
            p.setY(rect().bottom()-25);
            p.setX(5);
            sprintf(s,"Mx=%i My=%i",QWidget::mapFromGlobal(QCursor::pos()).x(),QWidget::mapFromGlobal(QCursor::pos()).y());
            painter.drawText(p,s);
            x = QWidget::mapFromGlobal(QCursor::pos()).x()*SURE_FAA/SURE_SCALE;
            y = QWidget::mapFromGlobal(QCursor::pos()).y()*SURE_FAA/SURE_SCALE;
            __VTYPE3 tv = DetermineTraceVector(x,y,&EngineData->CameraInfo);
            //__VTYPE3 tp = EngineData->CameraInfo.cam_x;
            p.setY(rect().bottom()-15);
            p.setX(5);
            sprintf(s,"TV = (%.3f;%.3f;%.3f)",tv.x,tv.y,tv.z);
            painter.drawText(p,s);

            EngineData->SelectedObject = -1;

            #define __SELECT_OBJECT
            #define SURE_RLEVEL 10
            #include <trace_common.c>
            #undef __SELECT_OBJECT

            p.setY(rect().bottom()-5);
            p.setX(5);
            sprintf(s,"SelectedObject = %i",EngineData->SelectedObject);
            painter.drawText(p,s);

        };
    };
    if((EngineData->r_drawdebug>=50)&&EngineData->SelectedObject>=0)
    {
        QPoint DrawPoint1;
        QPoint DrawPoint2;
        QPoint DrawPoint3;
        painter.setPen(Qt::white);
        switch (EngineData->objects[EngineData->SelectedObject].drawable.type){
            case SURE_DR_MESH:
            {
                if(EngineData->objects[EngineData->SelectedObject].drawable.mesh_count>50){
                    // рисуем OABB

                    SureDrawable *lv_dr = &EngineData->objects[EngineData->SelectedObject].drawable;
                    __VTYPE3 OABB_1;
                    __VTYPE3 OABB_2;
                    __VTYPE3 OABB_3;
                    __VTYPE3 OABB_4;
                    __VTYPE3 OABB_5;
                    __VTYPE3 OABB_6;
                    __VTYPE3 OABB_7;
                    __VTYPE3 OABB_8;

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


                    DRAW_LINE(OABB_1,OABB_2);
                    DRAW_LINE(OABB_2,OABB_3);
                    DRAW_LINE(OABB_3,OABB_4);
                    DRAW_LINE(OABB_4,OABB_1);

                    DRAW_LINE(OABB_5,OABB_6);
                    DRAW_LINE(OABB_6,OABB_7);
                    DRAW_LINE(OABB_7,OABB_8);
                    DRAW_LINE(OABB_8,OABB_5);

                    DRAW_LINE(OABB_1,OABB_5);
                    DRAW_LINE(OABB_2,OABB_6);
                    DRAW_LINE(OABB_3,OABB_7);
                    DRAW_LINE(OABB_4,OABB_8);

                }else{
                    // рисуем все mesh'ы
                    for(uint cm = 0;cm<EngineData->objects[EngineData->SelectedObject].drawable.mesh_count;++cm){
                        uint MeshID = EngineData->objects[EngineData->SelectedObject].drawable.mesh_start + cm;
                        __SURE_VINT4 MeshData;
                        __VTYPE3 LocalVertex1;
                        __VTYPE3 LocalVertex2;
                        __VTYPE3 LocalVertex3;
                        bool DrawPoint1Visible = false;
                        bool DrawPoint2Visible = false;
                        bool DrawPoint3Visible = false;
                        __GET_MESH(MeshData,MeshID);
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
                        CONVERT_TO_CAMERA_XY(GlobalVertex1,LocalPoint,DrawPoint1Visible);
                        if(DrawPoint1Visible){
                            DrawPoint1.setX(LocalPoint.x);
                            DrawPoint1.setY(LocalPoint.y);
                        };
                        CONVERT_TO_CAMERA_XY(GlobalVertex2,LocalPoint,DrawPoint2Visible);
                        if(DrawPoint2Visible){
                            DrawPoint2.setX(LocalPoint.x);
                            DrawPoint2.setY(LocalPoint.y);
                        };
                        CONVERT_TO_CAMERA_XY(GlobalVertex3,LocalPoint,DrawPoint3Visible);
                        if(DrawPoint3Visible){
                            DrawPoint3.setX(LocalPoint.x);
                            DrawPoint3.setY(LocalPoint.y);
                        };
                        if(DrawPoint1Visible&&DrawPoint2Visible)
                            painter.drawLine(DrawPoint1,DrawPoint2);
                        if(DrawPoint2Visible&&DrawPoint3Visible)
                            painter.drawLine(DrawPoint2,DrawPoint3);
                        if(DrawPoint1Visible&&DrawPoint3Visible)
                            painter.drawLine(DrawPoint3,DrawPoint1);
                    };
                };
            };
            break;
            case SURE_DR_SQUARE:
            {
                    SureDrawable *lv_dr = &EngineData->objects[EngineData->SelectedObject].drawable;
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

                    DRAW_LINE(OABB_1,OABB_2);
                    DRAW_LINE(OABB_2,OABB_3);
                    DRAW_LINE(OABB_3,OABB_4);
                    DRAW_LINE(OABB_4,OABB_1);
            };
            break;
            case SURE_DR_SPHERE:
            {
                QPoint DrawPoint;
                bool DrawPointVisible;
                SureDrawable *lv_dr = &EngineData->objects[EngineData->SelectedObject].drawable;

                __VTYPE3 GlobalPoint;
                for(float Angle=0.0;Angle<2*M_PI;Angle+=0.01){
                    GlobalPoint = lv_dr->X
                                 +lv_dr->ox*cos(Angle)*lv_dr->lx
                                 +lv_dr->oy*sin(Angle)*lv_dr->lx;
                    CONVERT_TO_CAMERA_XY(GlobalPoint,LocalPoint,DrawPointVisible);
                        if(DrawPointVisible){
                            DrawPoint.setX(LocalPoint.x);
                            DrawPoint.setY(LocalPoint.y);
                            painter.drawPoint(DrawPoint);
                        };
                    GlobalPoint = lv_dr->X
                                 +lv_dr->ox*cos(Angle)*lv_dr->lx
                                 +lv_dr->oz*sin(Angle)*lv_dr->lx;
                    CONVERT_TO_CAMERA_XY(GlobalPoint,LocalPoint,DrawPointVisible);
                        if(DrawPointVisible){
                            DrawPoint.setX(LocalPoint.x);
                            DrawPoint.setY(LocalPoint.y);
                            painter.drawPoint(DrawPoint);
                        };
                    GlobalPoint = lv_dr->X
                                 +lv_dr->oy*cos(Angle)*lv_dr->lx
                                 +lv_dr->oz*sin(Angle)*lv_dr->lx;
                    CONVERT_TO_CAMERA_XY(GlobalPoint,LocalPoint,DrawPointVisible);
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
        painter.setPen(Qt::blue);
    }; // if((EngineData->r_drawdebug>=50)&&EngineData->SelectedObject>=0)
    if((EngineData->r_drawdebug>=90)&&(EngineData->TraceLogsCount>0)){
        QPen l_Pen;
        QColor l_Color;
        l_Pen.setWidth(1);
        for(int TraceLogID = 0;TraceLogID<(int)EngineData->TraceLogsCount;TraceLogID++){
            double l_inicolor_x = 255;//EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.x;
            double l_inicolor_y = 255;//EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.y;
            double l_inicolor_z = 255;//EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.z;
            if((EngineData->r_drawdebug<70)&&(( EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.x
                                               +EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.y
                                               +EngineData->TraceLogs[TraceLogID].Items[EngineData->TraceLogs[TraceLogID].ItemsCount-1].Color.z)<20)){
                continue;
            };
            l_Color.setRed(l_inicolor_x);
            l_Color.setGreen(l_inicolor_y);
            l_Color.setBlue(l_inicolor_z);
            l_Pen.setColor(l_Color);
            painter.setPen(l_Pen);
            for(int TraceItemID = 0;TraceItemID<(int)EngineData->TraceLogs[TraceLogID].ItemsCount;TraceItemID++){
                SureTraceLogItem *l_item = &EngineData->TraceLogs[TraceLogID].Items[TraceItemID];
                __VTYPE3 CollisionPoint = l_item->TracePoint + l_item->TraceVector*l_item->IntersectDistance;
                DRAW_LINE(l_item->TracePoint,CollisionPoint);
                l_Pen.setColor(Qt::green);
                painter.setPen(l_Pen);
                __VTYPE3 NormPoint = CollisionPoint+10.0f*l_item->CollisionNomal;
                DRAW_LINE(NormPoint,CollisionPoint);
                l_Pen.setColor(Qt::red);
                painter.setPen(l_Pen);
                __VTYPE3 NormPointR = CollisionPoint+10.0f*l_item->NormalRandomized;
                DRAW_LINE(NormPointR,CollisionPoint);
                char txt[40];
                sprintf(txt,"%i(%i)",l_item->iter,l_item->rechecks);
                CONVERT_TO_CAMERA_XY(CollisionPoint,LocalPoint1,VisibleIndicator1);
                if(VisibleIndicator1){
                    P1.setX(LocalPoint1.x);
                    P1.setY(LocalPoint1.y);
                    painter.drawText(P1,txt);
                };
                l_Color.setRed(l_inicolor_x*l_item->Fade.x);
                l_Color.setGreen(l_inicolor_y*l_item->Fade.y);
                l_Color.setBlue(l_inicolor_z*l_item->Fade.z);
                l_Pen.setColor(l_Color);
                painter.setPen(l_Pen);
            };
        };
        painter.setPen(Qt::blue);
    };
    if(EngineData->r_drawdebug>=70)
    {
    for(int i = 0;i<EngineData->m_objects;++i)
    {
        SureObject* lv_o = &EngineData->objects[i];
        my_double3 dZ = EngineData->CameraInfo.cam_vec;
        my_double3 vtc = (__VTYPE3)EngineData->CameraInfo.cam_x-lv_o->X;
        double dz = dot(dZ,vtc);
        if(dz<0)
        {
            bool P1Visible;
            bool P2Visible;
            bool P3Visible;
            bool P4Visible;
            QPoint p1;
            CONVERT_TO_CAMERA_XY(lv_o->p1,LocalPoint,P1Visible);
            p1.setX(LocalPoint.x);
            p1.setY(LocalPoint.y);

            QPoint p2;
            CONVERT_TO_CAMERA_XY(lv_o->p2,LocalPoint,P2Visible);
            p2.setX(LocalPoint.x);
            p2.setY(LocalPoint.y);
            if (P1Visible&&P2Visible)
                painter.drawLine(p1,p2);

            QPoint p3;
            CONVERT_TO_CAMERA_XY(lv_o->p3,LocalPoint,P3Visible);
            p3.setX(LocalPoint.x);
            p3.setY(LocalPoint.y);
            if (P1Visible&&P3Visible)
                painter.drawLine(p1,p3);

            QPoint p4;
            CONVERT_TO_CAMERA_XY(lv_o->p4,LocalPoint,P4Visible);
            p4.setX(LocalPoint.x);
            p4.setY(LocalPoint.y);
            if (P1Visible&&P4Visible)
                painter.drawLine(p1,p4);
            if (P2Visible&&P4Visible)
                painter.drawLine(p2,p3);
            if (P2Visible&&P4Visible)
                painter.drawLine(p2,p4);
            if (P3Visible&&P4Visible)
                painter.drawLine(p3,p4);
        };// dz>0
    };
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
	//if(last_x<20||last_x>(lv_rect.width()-20)||last_y<20||last_y>(lv_rect.bottom()-20)){
        QCursor::setPos(QWidget::mapToGlobal(QPoint(lv_rect.width()/2,lv_rect.height()/2)));
        last_x = lv_rect.width()/2;
        last_y = lv_rect.height()/2;
    //};

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
    if(mousemove)
    {
        EngineData->TemplateObject.movable = true;
        EngineData->TemplateObject.collidable = true;
        if (event->button() == Qt::LeftButton) {
            EngineData->TemplateObject.type = SURE_OBJ_SPHERE;
            EngineData->TemplateObject.drawable.radiance = 0;
            EngineData->TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
            EngineData->TemplateObject.lx = 1.0+10.0*(float)rand()/(float)RAND_MAX; // длина
            EngineData->TemplateObject.lp = EngineData->TemplateObject.lx*1.7;
            EngineData->TemplateObject.drawable.transp = 0;
            EngineData->TemplateObject.drawable.dist_type = SURE_D_NORM;
            EngineData->TemplateObject.drawable.map_id = EngineData->GetTexture("earth");
            EngineData->TemplateObject.drawable.advmap_id = EngineData->GetTexture("earth_adv");
            __VTYPE3 X = EngineData->CameraInfo.cam_x;
            uint o = EngineData->CreateObjectFromTemplate(&X);
            EngineData->ObjByID(o)->push(EngineData->ObjByID(o)->X,EngineData->CameraInfo.cam_vec,0.8);

        };
        if (event->button() == Qt::RightButton) {
            EngineData->TemplateObject.drawable.rgb.s[0] = 120.0; // цвет
            EngineData->TemplateObject.drawable.rgb.s[1] = 240.0;// цвет
            EngineData->TemplateObject.drawable.rgb.s[2] = 120.0; // цвет
            EngineData->TemplateObject.type = SURE_OBJ_MESH;
            EngineData->TemplateObject.lx = 10.0; // длина
            EngineData->TemplateObject.ly = 10.0; // ширина
            EngineData->TemplateObject.lz = 1.0; // высота
            EngineData->TemplateObject.lp = 40.0;
            EngineData->TemplateObject.drawable.radiance = 0.0f;
            sprintf(EngineData->TemplateObject.ModelName_drawable,"cube");
            EngineData->TemplateObject.ModelID_drawable = EngineData->GetModel(EngineData->TemplateObject.ModelName_drawable);
            sprintf(EngineData->TemplateObject.ModelName_collider,"cube");
            EngineData->TemplateObject.ModelID_collider = EngineData->GetModel(EngineData->TemplateObject.ModelName_collider);
            EngineData->TemplateObject.ox = EngineData->CameraInfo.cam_vec;
            EngineData->TemplateObject.oz = EngineData->CameraInfo.cam_upvec;
            EngineData->TemplateObject.oy = cross(EngineData->CameraInfo.cam_vec,EngineData->CameraInfo.cam_upvec);
            EngineData->TemplateObject.drawable.map_id = -1;// EngineData->GetTexture("colstones");
            EngineData->TemplateObject.drawable.advmap_id = -1;
            EngineData->TemplateObject.drawable.transp = 0.99;
            EngineData->TemplateObject.drawable.refr = 1.4;
            EngineData->TemplateObject.drawable.dist_type = SURE_D_NORM;
            EngineData->TemplateObject.drawable.dist_sigma = 0.02f;
            EngineData->TemplateObject.drawable.type = SURE_DR_MESH;
            __VTYPE3 X = EngineData->CameraInfo.cam_x;
            uint o = EngineData->CreateObjectFromTemplate(&X);
            EngineData->ObjByID(o)->push(EngineData->ObjByID(o)->X,EngineData->CameraInfo.cam_vec,0.8);
        };
    }else{ // if mousemove
        if (event->button() == Qt::LeftButton) {

            int x = QWidget::mapFromGlobal(QCursor::pos()).x()*SURE_FAA/SURE_SCALE;
            int y = QWidget::mapFromGlobal(QCursor::pos()).y()*SURE_FAA/SURE_SCALE;
            for(int iter=0;iter<50000;++iter){

            #define __LOGGING
                //float* rgbmatrix = widget->rgbmatrix;
                SureDrawable* Drawables = GPUData->Drawables;
                cl_uchar* Textures = EngineData->TexturesData; // Текстуры
                cl_float* UVMap = EngineData->UVMap; // мэппинг мешей на текстуры
                cl_float* Normals = EngineData->Normals; // мэппинг мешей на текстуры
                cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
                cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей
            #define SURE_RLEVEL 100
            #include <trace_common.c>
            #undef __LOGGING
            if(EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount>0)
                if((EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.x+
                   EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.y+
                   EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.z) > 50){
                    iter = 50001;
                };
            };

            if((EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.x+
                   EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.y+
                   EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.z) > 50){
                if(++EngineData->TraceLogsCount>45)
                    EngineData->TraceLogsCount=0;
            };
        }; // (event->button() == Qt::LeftButton)
        if (event->button() == Qt::RightButton) {

            int x = QWidget::mapFromGlobal(QCursor::pos()).x()*SURE_FAA/SURE_SCALE;
            int y = QWidget::mapFromGlobal(QCursor::pos()).y()*SURE_FAA/SURE_SCALE;
            //for(int iter=0;iter<50000;++iter){

            #define __LOGGING
                //float* rgbmatrix = widget->rgbmatrix;
                SureDrawable* Drawables = GPUData->Drawables;
                cl_uchar* Textures = EngineData->TexturesData; // Текстуры
                cl_float* UVMap = EngineData->UVMap; // мэппинг мешей на текстуры
                cl_float* Normals = EngineData->Normals; // мэппинг мешей на текстуры
                cl_float* VrtxCLImg = EngineData->VrtxCLImg;// Набор vertexов
                cl_int* MeshCLImg = EngineData->MeshCLImg;// Набор mesh'ей
            #define SURE_RLEVEL 100
            #include <trace_common.c>
            #undef __LOGGING
            //if(EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount>0)
            //    if((EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.x+
            //       EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.y+
            //       EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.z) > 50){
            //        iter = 50001;
            //    };
            //};

            //if((EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.x+
            //       EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.y+
            //       EngineData->TraceLogs[EngineData->TraceLogsCount].Items[EngineData->TraceLogs[EngineData->TraceLogsCount].ItemsCount-1].Color.z) > 50){
                if(++EngineData->TraceLogsCount>45)
                    EngineData->TraceLogsCount=0;
            //};
        }; // (event->button() == Qt::LeftButton)
    }; // if !mousemove
} // mousePressEvent
