// Sure v 0.0010 by Vadim Malgin

// В работе -- версия 0.0011


// 21.04.2018 -- Версия 0.0010
// Графика
// * Исправлены баги с обзором изнутри тумана и пересечением стекла с туманом
//   (добавлена логика определения того, внутри какого объекта находится точка трассровки)
// Физика
// * Распараллеливание и оптимизация расчетов столкновений
// * Составные объекты
// * Теперь у объектов есть масса
// * Системы частиц
// * Удалять отвалившиеся объекты
// Другое
// * Наведен порядок в коде
// * Загрузка/сохранение объектов в файл
// * Настраиваемое управление
// * Экран загрузки

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
    app.setApplicationName(SURE_TITLE);
    app.setApplicationVersion(SURE_VERSION);

    // Объект со всеми данными движка:
    SureData EngineData;
    // Главное окно:
    SureWidget widget(&EngineData);
    // Поток рендера
    SureThread Render(&EngineData);
    // Поток физики
    SurePhysThread Physics(&EngineData);

    // запускаемся
    Physics.start(); // <- Тут запускается основная загрузка EngideData->LoadEngine()
    Render.start(QThread::IdlePriority);
    widget.show();
    // Открываем окно
    app.exec();
    // Окно приложения закрыто - закругляемся:
    Render.StopRender(); Render.wait();
    Physics.StopPhysics(); Physics.wait();
    return 0;
}
