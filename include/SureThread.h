#ifndef SURETHREAD_H
#define SURETHREAD_H

#include <SureData.h>
#include <SureWidget.h>

class SureThread : public QThread
{
    public:
        SureThread(SureData* i_engine);
        virtual ~SureThread();
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

        SureData* EngineData;

        void raytrace();

    protected:
    private:
        int SourceFromFile(const char* i_fname,char* e_source);
        SureWidget* parent;
        SureLog* Log;

        void SwitchKernel();
        void WriteModelToGPU(SureModelData *i_model);
        void WriteTextureToGPU(int TextureID);
};

#endif // SURETHREAD_H
