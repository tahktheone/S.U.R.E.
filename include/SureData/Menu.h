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

enum class EditingType {Int,Double,Bool,Float};

class SureMenuNumberEdit: public SureMenuElement{
    public:
        SureMenuNumberEdit(void* i_number,EditingType i_type,SureMenuWindow* i_parent);
        virtual ~SureMenuNumberEdit();
        void* EditingNumber;
        EditingType NumberType = EditingType::Int;

        void OnClick(int i_type = 2);
};

class SureMenuCheckbox: public SureMenuElement{
    public:
        SureMenuCheckbox(bool* i_value,SureMenuWindow* i_parent);
        virtual ~SureMenuCheckbox();
        bool* EditingValue;

        void OnClick();
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

class SureExitButton: public SureMenuElement{
    public:
        SureExitButton(SureMenuWindow* i_parent);
        virtual ~SureExitButton();
        void OnClick();
};

class SureOptionsButton: public SureMenuElement{
    public:
        SureOptionsButton(SureMenuWindow* i_parent);
        virtual ~SureOptionsButton();
        void OnClick();
};

class SureContolOptionsButton: public SureMenuElement{
    public:
        SureContolOptionsButton(SureMenuWindow* i_parent);
        virtual ~SureContolOptionsButton();
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
        virtual void OnClick(int key);
        virtual void Rebuild();
};

class SureMainMenuWindow: public SureMenuWindow{
    public:
        SureMainMenuWindow(SureData* i_engine);
        virtual ~SureMainMenuWindow();
        void OnKeyPress(int key);
        void Rebuild();
};

class SureKeyListener: public SureMenuWindow{
    public:
        SureKeyListener(SureData* i_engine, int i_action);
        virtual ~SureKeyListener();

        int Action = 0;
        void OnKeyPress(int key);
        void OnClick(int key);
};

class SureControlsWindow: public SureMenuWindow{
    public:
        SureControlsWindow(SureData* i_engine);
        virtual ~SureControlsWindow();

        void Rebuild();
};

class SureOptionsWindow: public SureMenuWindow{
    public:
        SureOptionsWindow(SureData* i_engine);
        virtual ~SureOptionsWindow();

        void Rebuild();
};
