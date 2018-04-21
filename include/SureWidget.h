#ifndef SUREWIDGET_H
#define SUREWIDGET_H

#include <SureData.h>

class SureWidget : public QWidget
{
    public:
        SureWidget(SureData *i_data);
        virtual ~SureWidget();
        // Данные движка
        SureData* EngineData;

        int last_x = 0;
        int last_y = 0;

    protected:
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
        void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
        void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
        void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
        void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
        void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    private:
        QImage *image;
        QImage CursorImage;
        QImage LoadingScreen;
        QPainter painter;

        void DrawMenu();
        void DrawDebugSelectedObject(SureObject *o);
        void DrawOABB(SureDrawable *lv_dr);
        void DrawMeshLines(SureDrawable *lv_dr);
        void DrawTraceLog(SureTraceLog *i_tl);
        my_double3 ConvertCoordinatesGlobalToCamera(my_double3 GlobalPoint,my_double3 *CameraBasis,bool *VisibleIndicator);
        void DrawLineInGlobalCoordinates(my_double3 i_GP1,my_double3 i_GP2,my_double3 *CameraBasis);
};

#endif // SUREWIDGET_H
