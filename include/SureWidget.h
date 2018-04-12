#ifndef SUREWIDGET_H
#define SUREWIDGET_H

#include <SureData.h>

class SureWidget : public QWidget
{
    public:
        SureWidget();
        virtual ~SureWidget();
        // Данные об изображении
        cl_float *rgbmatrix;
        cl_float *Randomf;

        // Данные движка
         SureData* EngineData;
         SureOCLData* OCLData;
         SureGPUData* GPUData;

        //
        bool mousemove = false;
        int last_x = 0;
        int last_y = 0;

        // Данные отладки
        float rendertime = 0;
        float posttime = 0;
        struct timespec framestart; // время начала текущего фрейма
        struct timespec frametime; // время отсечения

    protected:
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
        void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
        void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
        void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    private:
        QImage *image;
        QImage CursorImage;
        QPainter painter;

        void PlaceMatrixToImage();
        void DrawLinesDrawable(SureDrawable *lv_dr);
        void DrawOABB(SureDrawable *lv_dr);
        void DrawMeshLines(SureDrawable *lv_dr);
        void DrawTraceLog(SureTraceLog *i_tl);
        my_double3 ConvertCoordinatesGlobalToCamera(my_double3 GlobalPoint,my_double3 *CameraBasis,bool *VisibleIndicator);
        void DrawLineInGlobalCoordinates(my_double3 i_GP1,my_double3 i_GP2,my_double3 *CameraBasis);
};

#endif // SUREWIDGET_H
