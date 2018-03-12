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
    int my = image->rect().bottom();
    int mx = image->rect().right();
    EngineData->m_amx = rect().right()*SURE_FAA/SURE_SCALE;
    EngineData->m_amy = rect().bottom()*SURE_FAA/SURE_SCALE;
    clock_gettime(CLOCK_MONOTONIC,&framestart);
    double lv_max = 0;
    double lv_med = 0;
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
    if(drawdebug>=40)
    {
    p.setX(5);
    p.setY(15);
    painter.setPen(Qt::blue);
    sprintf(s,"DR:%.2f ms PH:%.2f ms PS:%.2f ms",rendertime,EngineData->frametime,posttime);
    painter.drawText(p,s);
    p.setY(30);
    sprintf(s,"X=%.4f Y=%.4f FOV=%.2f",EngineData->cam_x.x,EngineData->cam_x.y,EngineData->xy_h);
    painter.drawText(p,s);
    p.setY(45);
    if(OCLData->OpenCL)
    {
        sprintf(s,"GPU(OpenCL)");
        if(OCLData->rtype==SURE_RT_D)
            sprintf(s,"GPU(OpenCL-1)");
        if(OCLData->rtype==SURE_RT_T)
            sprintf(s,"GPU(OpenCL-2)");
        if(OCLData->rtype==SURE_RT_F)
            sprintf(s,"GPU(OpenCL-3)");
        if(OCLData->rtype==SURE_RT_N)
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
    if(drawdebug>=70)
    {
    for(int i = 0;i<EngineData->m_objects;++i)
    {
        SureObject* lv_o = &EngineData->objects[i];
        my_double3 dZ = EngineData->cam_vec;
        my_double3 vtc = EngineData->cam_x-lv_o->X;
        double dz = dot(dZ,vtc);
        if(dz<0)
        {
            my_double3 dY = -EngineData->cam_upvec;
            my_double3 dX = cross(dZ,dY);
            vtc = EngineData->cam_x-lv_o->p1;
            dz = dot(dZ,vtc);               // Локальные координаты
            double lx = dot(dX,vtc)/dz;               // Локальные координаты
            double ly = dot(dY,vtc)/dz;               // Локальные координаты
            //kx = GPUData->xy_h*(x+rx-mx/2.0)/mx;
            lx /= EngineData->xy_h;
            ly /= EngineData->xy_h;
            lx*=mx;
            ly*=mx;
            lx+=mx/2.0;
            ly+=my/2.0;
            QPoint p1;
            p1.setX(lx);
            p1.setY(ly);

            vtc = EngineData->cam_x-lv_o->p2;
            dz = dot(dZ,vtc);               // Локальные координаты
            lx = dot(dX,vtc)/dz;               // Локальные координаты
            ly = dot(dY,vtc)/dz;               // Локальные координаты
            //kx = GPUData->xy_h*(x+rx-mx/2.0)/mx;
            lx /= EngineData->xy_h;
            ly /= EngineData->xy_h;
            lx*=mx;
            ly*=mx;
            lx+=mx/2.0;
            ly+=my/2.0;
            QPoint p2;
            p2.setX(lx);
            p2.setY(ly);
            if ((p1.x()>0&&p1.x()<mx)&&
                (p2.x()>0&&p2.x()<mx)&&
                (p1.y()>0&&p1.y()<my)&&
                (p2.y()>0&&p2.y()<my))
                painter.drawLine(p1,p2);

            vtc = EngineData->cam_x-lv_o->p3;
            dz = dot(dZ,vtc);               // Локальные координаты
            lx = dot(dX,vtc)/dz;               // Локальные координаты
            ly = dot(dY,vtc)/dz;               // Локальные координаты
            //kx = GPUData->xy_h*(x+rx-mx/2.0)/mx;
            lx /= EngineData->xy_h;
            ly /= EngineData->xy_h;
            lx*=mx;
            ly*=mx;
            lx+=mx/2.0;
            ly+=my/2.0;
            QPoint p3;
            p3.setX(lx);
            p3.setY(ly);
            if ((p1.x()>0&&p1.x()<mx)&&
                (p3.x()>0&&p3.x()<mx)&&
                (p1.y()>0&&p1.y()<my)&&
                (p3.y()>0&&p3.y()<my))
                painter.drawLine(p1,p3);

            vtc = EngineData->cam_x-lv_o->p4;
            dz = dot(dZ,vtc);               // Локальные координаты
            lx = dot(dX,vtc)/dz;               // Локальные координаты
            ly = dot(dY,vtc)/dz;               // Локальные координаты
            //kx = GPUData->xy_h*(x+rx-mx/2.0)/mx;
            lx /= EngineData->xy_h;
            ly /= EngineData->xy_h;
            lx*=mx;
            ly*=mx;
            lx+=mx/2.0;
            ly+=my/2.0;
            QPoint p4;
            p4.setX(lx);
            p4.setY(ly);
            if ((p1.x()>0&&p1.x()<mx)&&
                (p4.x()>0&&p4.x()<mx)&&
                (p1.y()>0&&p1.y()<my)&&
                (p4.y()>0&&p4.y()<my))
                painter.drawLine(p1,p4);

            if ((p2.x()>0&&p2.x()<mx)&&
                (p3.x()>0&&p3.x()<mx)&&
                (p2.y()>0&&p2.y()<my)&&
                (p3.y()>0&&p3.y()<my))
                painter.drawLine(p2,p3);
            if ((p2.x()>0&&p2.x()<mx)&&
                (p4.x()>0&&p4.x()<mx)&&
                (p2.y()>0&&p2.y()<my)&&
                (p4.y()>0&&p4.y()<my))
                painter.drawLine(p2,p4);
            if ((p3.x()>0&&p3.x()<mx)&&
                (p4.x()>0&&p4.x()<mx)&&
                (p3.y()>0&&p3.y()<my)&&
                (p4.y()>0&&p4.y()<my))
                painter.drawLine(p3,p4);
        };// dz>0
    };
    };
    painter.end();
};

void SureWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Plus){
        EngineData->xy_h /= 1.1;
        EngineData->reset = true;
    };
    if(event->key()==Qt::Key_Minus){
        EngineData->xy_h *= 1.1;
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
        EngineData->cam_dx.y = -0.5;
    };
    if(event->key()==Qt::Key_Down){
        EngineData->cam_dx.y = 0.5;
    };
    if(event->key()==Qt::Key_Left){
        EngineData->cam_dx.x = -0.5;
    };
    if(event->key()==Qt::Key_Right){
        EngineData->cam_dx.x = 0.5;
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
        drawdebug-=30;
        if(drawdebug<30)drawdebug=99;
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
    if(event->key()==Qt::Key_L){
        EngineData->reset = true;
        switch(OCLData->rtype)
        {
            case SURE_RT_NOCL:
            {
                OCLData->rtype = SURE_RT_D;
                OCLData->OpenCL = true;
                std::wcout << L"Переключились на GPU-рендер вариант 1\n";
                break;
            };
            case SURE_RT_D:
            {
                OCLData->rtype = SURE_RT_T;
                OCLData->OpenCL = true;
                std::wcout << L"Переключились на GPU-рендер вариант 2\n";
                break;
            };
            case SURE_RT_T:
            {
                OCLData->rtype = SURE_RT_F;
                OCLData->OpenCL = true;
                std::wcout << L"Переключились на GPU-рендер вариант 3\n";
                break;
            };
            case SURE_RT_F:
            {
                OCLData->rtype = SURE_RT_N;
                OCLData->OpenCL = true;
                std::wcout << L"Переключились на GPU-рендер вариант 4\n";
                break;
            };
            case SURE_RT_N:
            {
                OCLData->rtype = SURE_RT_NOCL;
                OCLData->OpenCL = false;
                std::wcout << L"Переключились на CPU-рендер\n";
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
        if(EngineData->subp_rnd) {
            EngineData->subp_rnd = false;
        } else {
            EngineData->subp_rnd = true;
        };
    };
};

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
	if(last_x<20||last_x>(lv_rect.width()-20)||last_y<20||last_y>(lv_rect.bottom()-20)){
        QCursor::setPos(QWidget::mapToGlobal(QPoint(lv_rect.width()/2,lv_rect.height()/2)));
        last_x = lv_rect.width()/2;
        last_y = lv_rect.height()/2;
    };

};

void SureWidget::mousePressEvent(QMouseEvent *event)
{
    EngineData->TemplateObject.movable = true;
    EngineData->TemplateObject.collidable = true;
    if (event->button() == Qt::LeftButton) {
        EngineData->TemplateObject.type = SURE_OBJ_SPHERE;
        EngineData->TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
        EngineData->TemplateObject.lx = 1.0+10.0*(float)rand()/(float)RAND_MAX; // длина
        EngineData->TemplateObject.lp = EngineData->TemplateObject.lx*1.7;
        EngineData->TemplateObject.drawable.dist_type = SURE_D_NORM;
        EngineData->TemplateObject.drawable.map_id = EngineData->GetTexture("earth");
        EngineData->TemplateObject.drawable.advmap_id = EngineData->GetTexture("earth_adv");
        uint o = EngineData->CreateObjectFromTemplate(&EngineData->cam_x);
        EngineData->ObjByID(o)->push(EngineData->ObjByID(o)->X,EngineData->cam_vec,0.8);

    };
    if (event->button() == Qt::RightButton) {
        EngineData->TemplateObject.type = SURE_OBJ_MESH;
        EngineData->TemplateObject.lx = 6.0; // длина
        EngineData->TemplateObject.ly = 8.0; // ширина
        EngineData->TemplateObject.lz = 5.0; // высота
        EngineData->TemplateObject.lp = 10.0;
        EngineData->TemplateObject.ModelID = EngineData->GetModel("cube");
        EngineData->TemplateObject.ox = EngineData->cam_vec;
        EngineData->TemplateObject.oz = EngineData->cam_upvec;
        EngineData->TemplateObject.oy = cross(EngineData->cam_vec,EngineData->cam_upvec);
        EngineData->TemplateObject.drawable.mesh_start = EngineData->ModelsInfo[EngineData->TemplateObject.ModelID].mesh_start;
        EngineData->TemplateObject.drawable.mesh_count = EngineData->ModelsInfo[EngineData->TemplateObject.ModelID].mesh_count;
        EngineData->TemplateObject.drawable.map_id = EngineData->GetTexture("colstones");
        EngineData->TemplateObject.drawable.advmap_id = -1;
        EngineData->TemplateObject.drawable.dist_type = SURE_D_EQUAL;
        EngineData->TemplateObject.drawable.type = SURE_DR_MESH;
        uint o = EngineData->CreateObjectFromTemplate(&EngineData->cam_x);
        EngineData->ObjByID(o)->push(EngineData->ObjByID(o)->X,EngineData->cam_vec,0.8);
    };
};
