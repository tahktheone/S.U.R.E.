#ifndef SUREPHYSTHREAD_H
#define SUREPHYSTHREAD_H

#include <SureData.h>

class SurePhysThread : public QThread
{
    public:
        SurePhysThread(SureData* i_engine);
        virtual ~SurePhysThread();
        SureData* EngineData;
        SureGPUData* GPUData;
        void run() Q_DECL_OVERRIDE; // основная процедура потока
        bool m_running = true; // запущен ли поток
        void StopPhysics(){m_running = false;};

        // Данные отладки:
        struct timespec framestart; // время начала текущего фрейма
        struct timespec frametime; // время отсечения

        // данные в GPU для отрисовки
        void drawscene();
    protected:
    private:
};

#endif // SUREPHYSTHREAD_H
