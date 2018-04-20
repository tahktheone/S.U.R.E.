class SureMenuElement{
    public:
        SureMenuElement(SureMenuWindow* i_parent);
        virtual ~SureMenuElement();
        SureMenuWindow* ParentWindow;
        int x = 0;
        int y = 0;
        int width = 1;
        int height = 1;
        int type = 0;
        bool selected = false;
        wchar_t text[60];

        virtual void OnClick();
};

class SureActionButton: public SureMenuElement{
    public:
        SureActionButton(SureMenuWindow* i_parent,int i_action);
        virtual ~SureActionButton();

        void OnClick();
        int Action = 0;
};

class SureControlButton: public SureMenuElement{
    public:
        SureControlButton(SureMenuWindow* i_parent,SureControl* i_control);
        virtual ~SureControlButton();
        SureControl* Control;

        void OnClick();
};

class SureMenuWindow{
    public:
        SureMenuWindow(SureData* i_engine);
        virtual ~SureMenuWindow();
        SureData* Engine;
        int x = 0;
        int y = 0;
        int width = 1;
        int height = 1;
        SureMenuElement* Elements[100];
        int ElementsCounter = 0;
        int ScrollPosition = 0;

        virtual void OnKeyPress(int key);
};

class SureKeyListener: public SureMenuWindow{
    public:
        SureKeyListener(SureData* i_engine, int i_action);
        virtual ~SureKeyListener();

        int Action = 0;
        void OnKeyPress(int key);
};

class SureControlsWindow: public SureMenuWindow{
    public:
        SureControlsWindow(SureData* i_engine);
        virtual ~SureControlsWindow();

        void Rebuild();
};
