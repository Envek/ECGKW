#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDialog>


//! [0]
class GLWidget : public QGLWidget {
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
//! [0]

//! [1]
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setHeight(int newHeight);
    void setZoom (double newZoom);
    void setZoom (int newZoomPercentage);
    void turnCameraLeft();
    void turnCameraRight();
    void turnCameraUp();
    void turnCameraDown();
    void setBallElasticity(double newElasticity);
    void setBallElasticity(int newElasticityPercentage);
    void setBgColor (QColor color);
    void setBallColor (QColor color);
    void setFloorColor (QColor color);
    void chooseBallColor();
    void chooseBgColor();
    void chooseFloorColor();
    void startAnimation();
    void stopAnimation();
    void recalculateAnimation();
    void setBallRadius(double newRadius);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zoomChanged (double newZoom);
    void zoomChanged (int newZoom);
    void ballColorChanged(QColor color);
    void bgColorChanged(QColor color);
    void floorColorChanged(QColor color);
    void animationStarted();
    void animationStopped();
//! [1]

//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
//! [2]

//! [3]
private:
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor ballColor;
    QColor bgColor;
    QColor floorColor;
    double xoffs;
    double yoffs;
    double zoom;
    double startBallHeight;
    double currentBallHeight;
    double ballElasticity;
    double gravity;
    double zSpeed;
    bool animated;
    double radius;
    double quality;
    QTimer *timer;
};
//! [3]

#endif // GLWIDGET_H
