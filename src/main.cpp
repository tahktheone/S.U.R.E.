// Sure v 0.0004 by Vadim Malgin

// В работе - Версия 0.0004:
// Графика
// * Исправлены баги с обзором изнутри тумана и пересечением стекла с туманом
//   (добавлена логика определения того, внутри какого объекта находится точка трассровки)
// Другое
// * (в работе) Наведен порядок в коде widget'а

// 23.03.2018 - Версия 0.0003:
// Иитерфейс:
// * Добавлен курсор, исправлен баг с вываливанием курсора из окна
// * Логирование теперь в файлы, не в консоль
// * Сохранение/загрузка текущего состояния
// * логирование и отображение трассировки по ЛКМ
// * Доработан загрузчик obj в части универсализации
// Физика:
// * Реализовано столкновение mesh'ей (алгоритм GJK)
// * создание объекта одним методом из шаблона
// * удаление выбранного объекта
// Графика:
// * Исправлен целый ряд багов с отображением, связанных с ошибками округления на GPU
// * Подсвечивается выбранный объект

// 27.02.2018 -- версия 0.0002:
// * физика - столкновение шаров с mesh'ми, mesh'ей с плоскостями
// * генерация текстур (шахматные)
// * полноценная кросплатформенность, собраны бинарники под nix и win32
// * универсализация загрузчика obj

// 24.07.2017 -- версия 0.0001:
// * unbiased rendering на GPU (рисуем сферы, плоские квадраты и произвольные mesh'ы)
// * 4 режима рендера на GPU (double, float, float без рассеивания, float без оражений) и 1 на CPU
// * физика - столкновение шаров, шаров с плоскими квадратами
// * поддержка uv-карт
// * загрузка моделей из .obj файло

#include <SureData.h>
#include <SureWidget.h>
#include <SureThread.h>
#include <SurePhysThread.h>
#ifdef _WIN32
    #include <windows.h>
    #include <fcntl.h>
#endif // _WIN32

int main(int argc, char* argv[]) {

    #ifdef _WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
    #endif // _WIN32

    QApplication app(argc, argv);
    SureWidget widget;
    SureGPUData GPUData;
    SureData EngineData;
    SureOCLData OCLData;

    std::wcout << SURE_TITLE << "/" << SURE_VERSION << "\n";

    SureThread Render;
    SurePhysThread Physics;
    SureDrawable Drawables[SURE_DR_MAX];
    cl_float Randomf[SURE_R_RNDSIZE];

    app.setApplicationName(SURE_TITLE);
    app.setApplicationVersion(SURE_VERSION);

    widget.setMouseTracking(true);
    widget.setCursor(QCursor(Qt::BlankCursor));

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
    widget.GPUData = &GPUData;
    widget.Randomf = Randomf;
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
