#ifndef SURETRUCK_H
#define SURETRUCK_H

#include "SureData.h"

class SureTruck
{
    public:
        SureTruck(SureData* i_Engine,int i_ID);
        virtual ~SureTruck();
        int MainObjID = -1;
        SureData* Engine;
        float phase = 0.5f;
        int Caterpillars[100];
        float lx=1.0;
        float ly=1.0;
        float lz=1.0;
        float StepLen;
        float CatSize;

        void SetCaterpillars(); // разместить гусеицы
        void PushCaterpillars(float i_delta); // толкнуть гусеницы вдоль движения
        void PushByCaterlillars(); // толкнуть объект под действием гусениц

    protected:
    private:

};

#endif // SURETRUCK_H
