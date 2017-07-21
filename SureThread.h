#ifndef SURETHREAD_H
#define SURETHREAD_H

#include <SureData.h>
#include <SureWidget.h>

class SureThread : public QThread
{
    public:
        virtual ~SureThread();
        SureWidget* widget; // окно для вывода на экран
        void run() Q_DECL_OVERRIDE; // основная процедура потока
        bool m_running = true; // запущен ли поток
        void StopRender(){m_running = false;}; // m_running = false

        // Инициализация и выключение OpenCL
        void oclInit();
        void oclDelete();
        cl_int ret = 0;
        cl_device_id device;
        cl_context context;
        cl_program program_t;
        cl_program program_d;
        cl_program program_f;
        cl_program program_n;

        // буффер GPU<->CPU
        SureGPUData* GPUData;
        SureOCLData* OCLData;
        SureData* EngineData;
        cl_float* Randomf;

        void raytrace();
        void fractal();
        void noise();

    protected:
    private:
        SureWidget* parent;
};

#endif // SURETHREAD_H
