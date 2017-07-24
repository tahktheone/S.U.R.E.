// Sure v 0.0001 by Vadim Malgin
// 24.07.2017 -- версия 0.0001:
// * unbiased rendering на GPU (рисуем сферы, плоские квадраты и произвольные mesh'ы)
// * 4 режима рендера на GPU (double, float, float без рассеивания, float без оражений) и 1 на CPU
// * физика - столкновение шаров, швров с плоскими квадратами
// * поддержка uv-карт
// * загрузка моделей из .obj файлов
//
#include <SureData.h>
#include <SureWidget.h>
#include <SureThread.h>
#include <SurePhysThread.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    SureWidget widget;
    SureGPUData GPUData;
    SureData EngineData;
    SureOCLData OCLData;
    SureThread Render;
    SurePhysThread Physics;
    SureDrawable Drawables[SURE_DR_MAX];
    cl_float Randomf[SURE_R_RNDSIZE];

    app.setApplicationName(SURE_TITLE);
    app.setApplicationVersion(SURE_VERSION);
    std::cout << SURE_TITLE << "/" << SURE_VERSION << "\n";

    widget.setMouseTracking(true);
    //widget.setCursor(QCursor(Qt::BlankCursor));

    widget.rgbmatrix = new float[SURE_MAXRES_X*SURE_MAXRES_Y*3*SURE_FAA*SURE_FAA];

    for(int i = 0;i<SURE_MAXRES_Y*SURE_FAA;++i)
        for(int j = 0;j<SURE_MAXRES_X*SURE_FAA;++j)
        {
            widget.rgbmatrix[SURE_MAXRES_X*i*3*SURE_FAA+j*3] = 0;
            widget.rgbmatrix[SURE_MAXRES_X*i*3*SURE_FAA+j*3+1] = 0;
            widget.rgbmatrix[SURE_MAXRES_X*i*3*SURE_FAA+j*3+2] = 0;
        };

    GPUData.Drawables = Drawables;
    widget.EngineData = &EngineData;
    widget.OCLData = &OCLData;
    Render.Randomf = Randomf;
    Render.EngineData = &EngineData;
    Render.GPUData = &GPUData;
    Render.OCLData = &OCLData;
    Render.widget = &widget;
    Physics.GPUData = &GPUData;
    Physics.EngineData = &EngineData;
    Physics.start();
    Render.start(QThread::IdlePriority);

    widget.show();
    app.exec();
    Render.StopRender();
    Render.wait();
    Physics.StopPhysics();
    Physics.wait();

    EngineData.SureClear(); // чистим память

    return 0;
}
