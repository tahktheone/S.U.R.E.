
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
        NewElement->width = 100;
        Engine->GetControllerActionName(i,NewElement->text);
        int k = 0;

        for(int j=0;j<Engine->ControlsCounter;++j)
        if(Engine->Controls[j].Action.type==i)
        if(Engine->Controls[j].Input.type==SUREINPUT_KEYDOWN){
            Elements[ElementsCounter] = new SureControlButton(this,&Engine->Controls[j]);
            NewElement = Elements[ElementsCounter++];
            NewElement->x = 140+(k++)*90;
            NewElement->y = 10 + (i-1)*30;
            NewElement->height = 20;
            NewElement->width = 80;
            swprintf(NewElement->text,40,L"%lc (%i)",(wchar_t)Engine->Controls[j].Input.key,Engine->Controls[j].Input.key);
        }; // все Controls для Action
    };// все Actions
}
