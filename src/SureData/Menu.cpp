
SureMenuElement::SureMenuElement(SureMenuWindow* i_parent)
{
    const char* s = "SURE";
    for(int i = 0;i<5;++i)
        text[i] = s[i];
    ParentWindow = i_parent;
}

SureMenuElement::~SureMenuElement()
{

}

void SureMenuElement::OnClick()
{

}

SureMenuWindow::SureMenuWindow(SureData* i_engine)
{
    Engine = i_engine;
}

SureMenuWindow::~SureMenuWindow()
{
    int ElementsTotal = ElementsCounter;
    for(int i = 0;i<ElementsTotal;++i){
        delete Elements[--ElementsCounter];
    };
}

void SureMenuWindow::OnKeyPress(int key)
{

}

void SureMenuWindow::Rebuild()
{
    int ElementsTotal=ElementsCounter;
    for(int i = 0;i<ElementsTotal;++i)
        delete Elements[--ElementsCounter];
}

SureMenuNumberEdit::SureMenuNumberEdit(void* i_number,EditingType i_type,SureMenuWindow* i_parent):SureMenuElement(i_parent){
    NumberType = i_type;
    EditingNumber = i_number;
    type = SUREWINDOW_NUMBEREDIT;
}

SureMenuNumberEdit::~SureMenuNumberEdit(){

}

void SureMenuNumberEdit::OnClick(int i_type){
    if(i_type == 0){
        if(NumberType==EditingType::Int){
            int *IntNumber = (int*)EditingNumber;
            *IntNumber -= 1;
        };
    };
    if(i_type == 1){
         if(NumberType==EditingType::Int){
            int *IntNumber = (int*)EditingNumber;
            *IntNumber += 1;
        };
    };
    if(NumberType==EditingType::Int){
        swprintf(text,40,L"%i",(*(int*)EditingNumber));
    };
}


SureMenuCheckbox::SureMenuCheckbox(bool* i_value,SureMenuWindow* i_parent):SureMenuElement(i_parent){
    EditingValue = i_value;
    type = SUREWINDOW_CHECKBOX;
}

SureMenuCheckbox::~SureMenuCheckbox(){

}

void SureMenuCheckbox::OnClick(){
    if(*EditingValue){
        *EditingValue=false;
    }else{
        *EditingValue=true;
    };
}


SureActionButton::SureActionButton(SureMenuWindow* i_parent,int i_action):SureMenuElement(i_parent)
{
    type = SUREWINDOW_BUTTON;
    Action = i_action;
}

void SureActionButton::OnClick()
{
    ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter] = new SureKeyListener(ParentWindow->Engine,Action);
    SureMenuWindow* NewWindow = ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter++];
    NewWindow->width = 300;
    NewWindow->height = 200;
    NewWindow->x = (ParentWindow->width/2-150)+ParentWindow->x;
    NewWindow->y = (ParentWindow->height/2-100)+ParentWindow->y;
}

SureActionButton::~SureActionButton()
{

}


SureControlButton::SureControlButton(SureMenuWindow* i_parent,SureControl* i_control):SureMenuElement(i_parent)
{
    type = SUREWINDOW_BUTTON;
    Control = i_control;
}

SureControlButton::~SureControlButton()
{

}

void SureControlButton::OnClick()
{
    int ControlIndex = -1;
    for(int i = 0;i<ParentWindow->Engine->ControlsCounter;++i)
    if(&ParentWindow->Engine->Controls[i]==Control)
        ControlIndex=i;
    if(ControlIndex>=0){
        ParentWindow->Engine->Controls[ControlIndex] = ParentWindow->Engine->Controls[--ParentWindow->Engine->ControlsCounter];
    };
    ((SureControlsWindow*)ParentWindow)->Rebuild();
}

SureKeyListener::SureKeyListener(SureData* i_engine, int i_action):SureMenuWindow(i_engine)
{
    Action = i_action;
}

SureKeyListener::~SureKeyListener()
{

}

void SureKeyListener::OnKeyPress(int key)
{
    SureControllerAction a;
    a.type = Action;
    SureControllerInput i;
    i.type = SUREINPUT_KEYDOWN;
    i.key = key;
    Engine->AssignControl(&i,&a);
    ((SureControlsWindow*)Engine->MenuWindows[Engine->MenuWindowsCounter-2])->Rebuild();
    --Engine->MenuWindowsCounter;
    delete this;
}

SureControlsWindow::SureControlsWindow(SureData* i_engine):SureMenuWindow(i_engine)
{

}

SureControlsWindow::~SureControlsWindow()
{

}

void SureControlsWindow::Rebuild()
{
    int ElementsTotal=ElementsCounter;
    for(int i = 0;i<ElementsTotal;++i)
        delete Elements[--ElementsCounter];

    for(int i = 1;i<SUREACTIONS_TOTAL;++i){
        Elements[ElementsCounter] = new SureActionButton(this,i);
        SureMenuElement* NewElement = Elements[ElementsCounter++];
        NewElement->x = 20;
        NewElement->y = 10 + (i-1)*30;
        NewElement->height = 20;
        NewElement->width = 200;
        Engine->GetControllerActionName(i,NewElement->text);
        int k = 0;

        for(int j=0;j<Engine->ControlsCounter;++j)
        if(Engine->Controls[j].Action.type==i)
        if(Engine->Controls[j].Input.type==SUREINPUT_KEYDOWN){
            Elements[ElementsCounter] = new SureControlButton(this,&Engine->Controls[j]);
            NewElement = Elements[ElementsCounter++];
            NewElement->x = 240+(k++)*110;
            NewElement->y = 10 + (i-1)*30;
            NewElement->height = 20;
            NewElement->width = 100;
            swprintf(NewElement->text,40,L"%lc (%i)",(wchar_t)Engine->Controls[j].Input.key,Engine->Controls[j].Input.key);
        }; // все Controls для Action
    };// все Actions
}

SureOptionsWindow::SureOptionsWindow(SureData* i_engine):SureMenuWindow(i_engine)
{

}

SureOptionsWindow::~SureOptionsWindow()
{

}

void SureOptionsWindow::Rebuild()
{
    int ElementsTotal=ElementsCounter;
    for(int i = 0;i<ElementsTotal;++i)
        delete Elements[--ElementsCounter];

    int h = 10;

    Elements[ElementsCounter] = new SureMenuElement(this);
    SureMenuElement* NewElement = Elements[ElementsCounter++];
    NewElement->type = SUREWINDOW_LABEL;
    NewElement->x = 20;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 200;
    swprintf(NewElement->text,40,L"Режим отрисовки:");

    Elements[ElementsCounter] = new SureMenuNumberEdit((void*)&Engine->r_type,EditingType::Int,this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = 240;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 150;
    swprintf(NewElement->text,40,L"%i",Engine->r_type);

    h = h + 30;

    Elements[ElementsCounter] = new SureMenuElement(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->type = SUREWINDOW_LABEL;
    NewElement->x = 20;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 200;
    swprintf(NewElement->text,40,L"Итерации:");

    Elements[ElementsCounter] = new SureMenuNumberEdit((void*)&Engine->GPUData.SAA,EditingType::Int,this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = 240;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 150;
    swprintf(NewElement->text,40,L"%i",Engine->GPUData.SAA);

    h = h + 30;

    Elements[ElementsCounter] = new SureMenuElement(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->type = SUREWINDOW_LABEL;
    NewElement->x = 20;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 200;
    swprintf(NewElement->text,40,L"Размытие:");

    Elements[ElementsCounter] = new SureMenuNumberEdit((void*)&Engine->ImageScale,EditingType::Int,this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = 240;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 150;
    swprintf(NewElement->text,40,L"%i",Engine->ImageScale);

    h = h + 30;

    Elements[ElementsCounter] = new SureMenuElement(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->type = SUREWINDOW_LABEL;
    NewElement->x = 20;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 200;
    swprintf(NewElement->text,40,L"Анти-алиасинг:");

    Elements[ElementsCounter] = new SureMenuCheckbox((bool*)&Engine->CameraInfo.subp_rnd,(SureMenuWindow*)this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = 240;
    NewElement->y = h;
    NewElement->height = 20;
    NewElement->width = 20;
    swprintf(NewElement->text,40,L" ");

}

SureMainMenuWindow::SureMainMenuWindow(SureData* i_engine):SureMenuWindow(i_engine){}
SureMainMenuWindow::~SureMainMenuWindow(){}
void SureMainMenuWindow::OnKeyPress(int key){}

void SureMainMenuWindow::Rebuild()
{
    SureMenuElement* NewElement;
    int ElementsTotal=ElementsCounter;
    for(int i = 0;i<ElementsTotal;++i)
        delete Elements[--ElementsCounter];

    Elements[ElementsCounter] = new SureContolOptionsButton(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = this->width/2-50;
    NewElement->y = 30;
    NewElement->height = 60;
    NewElement->width = 100;
    swprintf(NewElement->text,40,L"Управление");

    Elements[ElementsCounter] = new SureOptionsButton(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = this->width/2-50;
    NewElement->y = 120;
    NewElement->height = 60;
    NewElement->width = 100;
    swprintf(NewElement->text,40,L"Настройки");

    Elements[ElementsCounter] = new SureExitButton(this);
    NewElement = Elements[ElementsCounter++];
    NewElement->x = this->width/2-50;
    NewElement->y = 210;
    NewElement->height = 60;
    NewElement->width = 100;
    swprintf(NewElement->text,40,L"Выход");

}

SureExitButton::SureExitButton(SureMenuWindow* i_parent):SureMenuElement(i_parent){type = SUREWINDOW_BUTTON;}
SureExitButton::~SureExitButton(){}
void SureExitButton::OnClick()
{
    ParentWindow->Engine->Stop();
}

SureOptionsButton::SureOptionsButton(SureMenuWindow* i_parent):SureMenuElement(i_parent){type = SUREWINDOW_BUTTON;}
SureOptionsButton::~SureOptionsButton(){}
void SureOptionsButton::OnClick(){
    ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter] = new SureOptionsWindow(ParentWindow->Engine);
    SureOptionsWindow* NewWindow = (SureOptionsWindow*)ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter++];
    NewWindow->x = ParentWindow->x;
    NewWindow->y = ParentWindow->y;
    NewWindow->width = ParentWindow->width;
    NewWindow->height = ParentWindow->height;
    NewWindow->Rebuild();
}

SureContolOptionsButton::SureContolOptionsButton(SureMenuWindow* i_parent):SureMenuElement(i_parent){type = SUREWINDOW_BUTTON;}
SureContolOptionsButton::~SureContolOptionsButton(){}
void SureContolOptionsButton::OnClick(){
    ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter] = new SureControlsWindow(ParentWindow->Engine);
    SureControlsWindow* NewWindow = (SureControlsWindow*)ParentWindow->Engine->MenuWindows[ParentWindow->Engine->MenuWindowsCounter++];
    NewWindow->x = ParentWindow->x;
    NewWindow->y = ParentWindow->y;
    NewWindow->width = ParentWindow->width;
    NewWindow->height = ParentWindow->height;
    NewWindow->Rebuild();
}
