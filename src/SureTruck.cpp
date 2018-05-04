#include "SureTruck.h"

#define TRACKS 8

SureTruck::SureTruck(SureData* i_Engine,int i_ID)
{
    Engine = i_Engine;
    MainObjID = i_ID;
    SureObject* lv_o = Engine->ObjByID(MainObjID);
    StepLen = lv_o->lx * 2.0f / ((float)TRACKS+1.0f);
    CatSize = __SURE_MIN((lv_o->ly/8.0f),(StepLen/4.0f));

    Engine->SetTemplate_GlassSphere(CatSize);
    Engine->TemplateObject.CollideExcludeID = MainObjID;
    Engine->TemplateObject.oz = -lv_o->oz;
    Engine->TemplateObject.ox = lv_o->oy;
    Engine->TemplateObject.oy = lv_o->ox;
    my_double3 X;
    my_double3 LocalX;

    for(int i=0;i<TRACKS;++i){
        LocalX.x = lv_o->lx - CatSize - i*StepLen - phase * StepLen;
        LocalX.y = lv_o->ly-CatSize;
        LocalX.z = -lv_o->lz;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        Caterpillars[i*2] = Engine->CreateObjectFromTemplate(&X);
        LocalX.y = -lv_o->ly+CatSize;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        Caterpillars[i*2+1] = Engine->CreateObjectFromTemplate(&X);
    };

}

SureTruck::~SureTruck()
{
    //dtor
}

void SureTruck::SetCaterpillars(){
    // разместить гусеицы
    if(phase<0.0f){
        phase = phase + 1.0f;
        Caterpillars[TRACKS*2] = Caterpillars[0];
        Caterpillars[TRACKS*2+1] = Caterpillars[1];
        for(int i=0;i<TRACKS;++i){
            Caterpillars[i*2] = Caterpillars[(i+1)*2];
            Caterpillars[i*2+1] = Caterpillars[(i+1)*2+1];
        };
    };
    if(phase>1.0f){
        phase = phase - 1.0f;
        for(int i=TRACKS;i>0;--i){
            Caterpillars[i*2] = Caterpillars[(i-1)*2];
            Caterpillars[i*2+1] = Caterpillars[(i-1)*2+1];
        };
        Caterpillars[0]=Caterpillars[TRACKS*2];
        Caterpillars[1]=Caterpillars[TRACKS*2+1];
    };
    SureObject* lv_o = Engine->ObjByID(MainObjID);
    my_double3 LocalX;
    my_double3 X;
    for(int i=0;i<TRACKS;++i){
        LocalX.x = lv_o->lx - CatSize - i*StepLen - phase * StepLen;
        LocalX.y = lv_o->ly-CatSize;
        LocalX.z = -lv_o->lz;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        Engine->ObjByID(Caterpillars[i*2])->teleport(X);
        Engine->ObjByID(Caterpillars[i*2])->stop();
        LocalX.y = -lv_o->ly+CatSize;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        Engine->ObjByID(Caterpillars[i*2+1])->teleport(X);
        Engine->ObjByID(Caterpillars[i*2+1])->stop();
    };
}

void SureTruck::PushCaterpillars(float i_delta){
    // толкнуть гусеницы вдоль движения
    phase = phase+i_delta;
    SureObject* lv_o = Engine->ObjByID(MainObjID);

    for(int i=0;i<TRACKS;++i){
        SureObject* l_cp = Engine->ObjByID(Caterpillars[i*2]);
        l_cp->push(l_cp->X,-lv_o->ox,StepLen * i_delta);
        l_cp = Engine->ObjByID(Caterpillars[i*2+1]);
        l_cp->push(l_cp->X,-lv_o->ox,StepLen * i_delta);
    };
}

void SureTruck::PushByCaterlillars(){
    // толкнуть объект под действием гусениц
    SureObject* lv_o = Engine->ObjByID(MainObjID);
    my_double3 X;
    my_double3 LocalX;
    my_double3 MoveVec;

    for(int i=0;i<TRACKS;++i){
        LocalX.x = lv_o->lx-CatSize-i*StepLen-phase * StepLen;
        LocalX.y = lv_o->ly-CatSize;
        LocalX.z = -lv_o->lz;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        MoveVec = Engine->ObjByID(Caterpillars[i*2])->X-X;
        lv_o->push(X,MoveVec,0.12f);

        LocalX.y = -lv_o->ly+CatSize;
        X = lv_o->X + lv_o->ox*LocalX.x + lv_o->oy*LocalX.y + lv_o->oz*LocalX.z;
        MoveVec = Engine->ObjByID(Caterpillars[i*2+1])->X-X;
        lv_o->push(X,MoveVec,0.12f);
    };
}
