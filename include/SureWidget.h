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
        QPainter painter;

        void PlaceMatrixToImage();
};

#endif // SUREWIDGET_H
