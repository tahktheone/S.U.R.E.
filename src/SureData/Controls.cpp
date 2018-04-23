void SureData::SaveControls(const char* name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./%s.SureControls",name);
    FILE *f = NULL;
    f = fopen(fname,"wb");
    if(f==NULL){
        sprintf(LogLine,"Не удается создать/заменить файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int ControlsFileVersion = 1;
    fwrite(&ControlsFileVersion,sizeof(int),1,f);

    fwrite(&ControlsCounter,sizeof(int),1,f);
    for(int i = 0;i<ControlsCounter;++i){
         fwrite(&Controls[i].Input.type,sizeof(int),1,f);
         fwrite(&Controls[i].Input.key,sizeof(int),1,f);
         fwrite(&Controls[i].Input.x,sizeof(float),1,f);
         fwrite(&Controls[i].Input.y,sizeof(float),1,f);
         fwrite(&Controls[i].Action.type,sizeof(int),1,f);
         fwrite(&Controls[i].active,sizeof(bool),1,f);
    };

    fclose(f);
    sprintf(LogLine,"Сохранены настройки управления %s",name);
    Log->AddLine(LogLine);
}

void SureData::LoadControls(const char* name)
{
    char fname[100];
    char LogLine[200];
    sprintf(fname,"./%s.SureControls",name);
    FILE *f = NULL;
    f = fopen(fname,"rb");
    if(f==NULL){
        sprintf(LogLine,"Не удается открыть файл %s",fname);
        Log->AddLine(LogLine);
        return;
    };

    int ControlsFileVersion = 1;
    SureFread(&ControlsFileVersion,sizeof(int),1,f);

    if(ControlsFileVersion==1){
        SureFread(&ControlsCounter,sizeof(int),1,f);
        for(int i = 0;i<ControlsCounter;++i){
             SureFread(&Controls[i].Input.type,sizeof(int),1,f);
             SureFread(&Controls[i].Input.key,sizeof(int),1,f);
             SureFread(&Controls[i].Input.x,sizeof(float),1,f);
             SureFread(&Controls[i].Input.y,sizeof(float),1,f);
             SureFread(&Controls[i].Action.type,sizeof(int),1,f);
             SureFread(&Controls[i].active,sizeof(bool),1,f);
        };
    }else{ // if(ControlsFileVersion==2)...
        sprintf(LogLine,"Версия настроек управления %i не поддерживается",ControlsFileVersion);
        Log->AddLine(LogLine);
    };

    fclose(f);
    sprintf(LogLine,"Загружены настройки управления %s",name);
    Log->AddLine(LogLine);
}

void SureData::AssignControl(SureControllerInput* i_input,SureControllerAction* i_action)
{
    if(i_input->type==SUREINPUT_KEYDOWN){
        SureControllerInput i;
        i = *i_input;
        i.type = SUREINPUT_KEYUP;
        AssignControl(&i,i_action);
    };

    for(int i = 0;i<ControlsCounter;++i)
    if((Controls[i].Input.type==i_input->type)&&
       (Controls[i].Input.key==i_input->key))
    if(Controls[i].Action.type==i_action->type)
        return; // уже есть такое присвоение

    Controls[ControlsCounter].Action = *i_action;
    Controls[ControlsCounter].Input = *i_input;
    Controls[ControlsCounter].active = true;
    ControlsCounter++;
}

void SureData::HandleInput(SureControllerInput* i_input)
{
    if(MenuWindowsCounter>0){
        SureMenuWindow* CurrentWindow = MenuWindows[MenuWindowsCounter-1];
        CurrentWindow->OnKeyPress(i_input->key);
    }else{
        for(int i = 0;i<ControlsCounter;++i)
        if((Controls[i].Input.type==i_input->type)&&
           (Controls[i].Input.key==i_input->key))
           ExecuteAction(&Controls[i].Action,i_input);
    };
}

void SureData::ExecuteAction(SureControllerAction* i_action,SureControllerInput* i_input)
{
    switch(i_action->type){
        case SUREACTION_MOVEFORVARD:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.x = 1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.x = 0;
            break;
        };//SUREACTION_MOVEFORVARD_START
        case SUREACTION_MOVEBACKWARD:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.x = -1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.x = 0;
            break;
        };//SUREACTION_MOVEFORVARD_START
        case SUREACTION_MOVELEFT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.y = -1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.y = 0;
            break;
        };
        case SUREACTION_MOVERIGHT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.y = 1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.y = 0;
            break;
        };
        case SUREACTION_ZOOMIN:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            CameraInfo.xy_h /= 1.1;
            reset = true;
        break;
        };// Увеличение
        case SUREACTION_ZOOMOUT:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            CameraInfo.xy_h *= 1.1;
            reset = true;
        break;
        };// Уменьшение
        case SUREACTION_MOVEUP:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.z = 1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.z = 0;
        break;
        };// Вверх
        case SUREACTION_MOVEDOWN:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dx.z = -1.5;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dx.z = 0;
        break;
        };// Вниз
        case SUREACTION_SPINLEFT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.x = -0.05;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.x = 0;
        break;
        };// Вращение влево
        case SUREACTION_SPINRIGHT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.x = 0.05;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.x = 0;
        break;
        };// Вращение вправо
        case SUREACTION_ROTATEUP:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.y = 0.3;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.y = 0;
        break;
        };// Смотреть вверх
        case SUREACTION_ROTATEDOWN:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.y = -0.3;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.y = 0;
        break;
        };// Смотреть вниз
        case SUREACTION_ROTATELEFT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.z = -0.3;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.z = 0;
        break;
        };// Поворот налево
        case SUREACTION_ROTATRIGHT:{
            if(i_input->type==SUREINPUT_KEYDOWN)
                cam_dw.z = 0.3;
            if(i_input->type==SUREINPUT_KEYUP)
                cam_dw.z = 0;
        break;
        };// Поворот направо
        case SUREACTION_MINUSITERS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_iters -= 2;
            reset = true;
        break;
        };// (Графика) Итерации-
        case SUREACTION_PLUSITERS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_iters += 2;
            reset = true;
        break;
        };// (Графика) Итерации+
        case SUREACTION_MINUSRECHECKS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_rechecks -= 2;
            reset = true;
        break;
        };// (Графика) Пересчеты-
        case SUREACTION_PLUSRECHECKS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_rechecks += 2;
            reset = true;
        break;
        };// (Графика) Пересчеты+
        case SUREACTION_MINUSBACKLIGHT:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_backlight -= 0.5;
            reset = true;
        break;
        };// (Графика) Подсветка-
        case SUREACTION_PLUSBACKLIGHT:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_backlight += 0.5;
            reset = true;
        break;
        };// (Графика) Подсветка+
        case SUREACTION_DECRASEDEBUG:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            r_drawdebug-=30;
            if(r_drawdebug<30)r_drawdebug=99;
        break;
        };// Отладочная информация
        case SUREACTION_PLUSSCALE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            ImageScale += 1;
            reset = true;
        break;
        };
        case SUREACTION_MINUSSCALE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(ImageScale>1)
                ImageScale -= 1;
            reset = true;
        break;
        };
        case SUREACTION_PLUSRAYS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            GPUData.SAA += 1;
            reset = true;
        break;
        };
        case SUREACTION_MINUSRAYS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(GPUData.SAA>1)
                GPUData.SAA -= 1;
            reset = true;
        break;
        };
        case SUREACTION_SCREENSHOT:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            QString Fname = "./screenshots/";
            char FileName[100];
            time_t seconds = time(NULL);
            tm* timeinfo = localtime(&seconds);
            strftime (FileName,100,"shot%y%m%d_%H%M%S.png",timeinfo);
            Fname += FileName;
            QImage* image = new QImage(CurrentWidth,CurrentHeight,QImage::Format_ARGB32);
            SureMatrixToImage(rgbmatrix,image,CurrentWidth,CurrentHeight,ImageScale);
            image->save(Fname);
            delete image;
        break;
        };// Скриншот
        case SUREACTION_TOGGLEMOUSEMOVE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(mousemove){
                mousemove = false;
            }else{
                mousemove = true;
            };
        break;
        };// Управление мышью
        case SUREACTION_TOGGLEPAUSE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(paused){
                paused = false;
            }else{
                paused = true;
            };
        break;
        };// Пауза
        case SUREACTION_SAVESTATE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            SaveState("initial");
        break;
        };// Сохр всё
        case SUREACTION_LOADSTATE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            LoadState("initial");
        break;
        };// Загр всё
        case SUREACTION_DELETESELECTED:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            DeleteObject(SelectedObject);
            reset = true;
        break;
        };// Удалить объект
        case SUREACTION_TOGGLEMODE:{
            if(i_input->type==SUREINPUT_KEYUP)break;
                switch(r_type){
                    case SURE_RT_NOCL:
                    {
                        r_type = SURE_RT_D;
                        OCLData.OpenCL = true;
                        Log->AddLine("Переключились на GPU-рендер вариант 1");
                        break;
                    };
                    case SURE_RT_D:
                    {
                        r_type = SURE_RT_T;
                        OCLData.OpenCL = true;
                        Log->AddLine("Переключились на GPU-рендер вариант 2");
                        break;
                    };
                    case SURE_RT_T:
                    {
                        r_type = SURE_RT_F;
                        OCLData.OpenCL = true;
                        Log->AddLine("Переключились на GPU-рендер вариант 3");
                        break;
                    };
                    case SURE_RT_F:
                    {
                        r_type = SURE_RT_N;
                        OCLData.OpenCL = true;
                        Log->AddLine("Переключились на GPU-рендер вариант 4");
                        break;
                    };
                    case SURE_RT_N:
                    {
                        r_type = SURE_RT_NOCL;
                        OCLData.OpenCL = false;
                        Log->AddLine("Переключились на СPU-рендер");
                        break;
                    };
                    default:
                    break;
                }; //SWITCH
                reset = true;
        break;
        };// Режим отрисовки
        case SUREACTION_MENUSCROLLDOWN:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(MenuWindowsCounter>0){
                int MaximumBottom = MenuWindows[MenuWindowsCounter-1]->height;
                for(int j = 0;j<MenuWindows[MenuWindowsCounter-1]->ElementsCounter;++j)
                if((MenuWindows[MenuWindowsCounter-1]->y+MenuWindows[MenuWindowsCounter-1]->Elements[j]->y) > MaximumBottom)
                    MaximumBottom = MenuWindows[MenuWindowsCounter-1]->y+MenuWindows[MenuWindowsCounter-1]->Elements[j]->y;
                if(MenuWindows[MenuWindowsCounter-1]->ScrollPosition < (MaximumBottom-MenuWindows[MenuWindowsCounter-1]->height))
                    MenuWindows[MenuWindowsCounter - 1]->ScrollPosition += i_input->x;
            };
            break;
        }; // SUREACTION_MENUSCROLLDOWN
        case SUREACTION_MENUSCROLLUP:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if((MenuWindowsCounter>0)&&MenuWindows[MenuWindowsCounter-1]->ScrollPosition >= i_input->x)
                MenuWindows[MenuWindowsCounter - 1]->ScrollPosition -= i_input->x;
            break;
        }; // SUREACTION_MENUSCROLLUP
        case SUREACTION_SAVECONTROLS:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            SaveControls("default");
            break;
        }; //SUREACTION_SAVECONTROLS
        case SUREACTION_TOGGLESAA:{
            if(i_input->type==SUREINPUT_KEYUP)break;
            if(CameraInfo.subp_rnd) {
                CameraInfo.subp_rnd = false;
            } else {
                CameraInfo.subp_rnd = true;
            };
            reset = true;
            break;
        }; //SUREACTION_TOGGLESAA
        default:
            break;
    }; //switch(i_action->type)
}

void SureData::GetControllerActionName(int i_action,wchar_t *e_name){
    if(i_action==SUREACTION_NONE)swprintf(e_name,40,L"No action");
    if(i_action==SUREACTION_MOVEFORVARD)swprintf(e_name,40,L"Вперед");
    if(i_action==SUREACTION_SAVECONTROLS)swprintf(e_name,40,L"СохрУпр");
    if(i_action==SUREACTION_MOVEBACKWARD)swprintf(e_name,40,L"Назад");
    if(i_action==SUREACTION_MOVELEFT)swprintf(e_name,40,L"Влево");
    if(i_action==SUREACTION_MOVERIGHT)swprintf(e_name,40,L"Вправо");
    if(i_action==SUREACTION_ZOOMIN)swprintf(e_name,40,L"Увеличение");
    if(i_action==SUREACTION_ZOOMOUT)swprintf(e_name,40,L"Уменьшение");
    if(i_action==SUREACTION_MOVEUP)swprintf(e_name,40,L"Вверх");
    if(i_action==SUREACTION_MOVEDOWN)swprintf(e_name,40,L"Вниз");
    if(i_action==SUREACTION_SPINLEFT)swprintf(e_name,40,L"Вращение влево");
    if(i_action==SUREACTION_SPINRIGHT)swprintf(e_name,40,L"Вращение вправо");
    if(i_action==SUREACTION_ROTATEUP)swprintf(e_name,40,L"Смотреть вверх");
    if(i_action==SUREACTION_ROTATEDOWN)swprintf(e_name,40,L"Смотреть вниз");
    if(i_action==SUREACTION_ROTATELEFT)swprintf(e_name,40,L"Поворот налево");
    if(i_action==SUREACTION_ROTATRIGHT)swprintf(e_name,40,L"Поворот направо");
    if(i_action==SUREACTION_MINUSITERS)swprintf(e_name,40,L"(Графика) Итерации-");
    if(i_action==SUREACTION_PLUSITERS)swprintf(e_name,40,L"(Графика) Итерации+");
    if(i_action==SUREACTION_MINUSRECHECKS)swprintf(e_name,40,L"(Графика) Пересчеты-");
    if(i_action==SUREACTION_PLUSRECHECKS)swprintf(e_name,40,L"(Графика) Пересчеты+");
    if(i_action==SUREACTION_MINUSBACKLIGHT)swprintf(e_name,40,L"(Графика) Подсветка-");
    if(i_action==SUREACTION_PLUSBACKLIGHT)swprintf(e_name,40,L"(Графика) Подсветка+");
    if(i_action==SUREACTION_DECRASEDEBUG)swprintf(e_name,40,L"Отладочная информация");
    if(i_action==SUREACTION_SCREENSHOT)swprintf(e_name,40,L"Скриншот");
    if(i_action==SUREACTION_TOGGLEMOUSEMOVE)swprintf(e_name,40,L"Управление мышью");
    if(i_action==SUREACTION_TOGGLEPAUSE)swprintf(e_name,40,L"Пауза");
    if(i_action==SUREACTION_SAVESTATE)swprintf(e_name,40,L"Сохр всё");
    if(i_action==SUREACTION_LOADSTATE)swprintf(e_name,40,L"Загр всё");
    if(i_action==SUREACTION_DELETESELECTED)swprintf(e_name,40,L"Удалить объект");
    if(i_action==SUREACTION_TOGGLEMODE)swprintf(e_name,40,L"Режим отрисовки");
    if(i_action==SUREACTION_MENUSCROLLUP)swprintf(e_name,40,L"Меню: листать вверх");
    if(i_action==SUREACTION_MENUSCROLLDOWN)swprintf(e_name,40,L"Меню: листать ввниз");
    if(i_action==SUREACTION_TOGGLESAA)swprintf(e_name,40,L"(Графика) Вкл/выкл SAA");
    if(i_action==SUREACTION_PLUSSCALE)swprintf(e_name,40,L"(Графика) Разрешение-");
    if(i_action==SUREACTION_MINUSSCALE)swprintf(e_name,40,L"(Графика) Разрешение+");
    if(i_action==SUREACTION_PLUSRAYS)swprintf(e_name,40,L"(Графика) Лучи+");
    if(i_action==SUREACTION_MINUSRAYS)swprintf(e_name,40,L"(Графика) Лучи-");
}
