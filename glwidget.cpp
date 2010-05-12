#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    animated = false;
    gravity = -0.1;

    // Параметры отображения
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xoffs = 0;
    yoffs = 0;
    zoom = 1;

    // Параметры мяча
    quality = 32;
    radius = 1;
    startBallHeight = 5;
    currentBallHeight = startBallHeight;
    ballElasticity = 0.75;
    zSpeed = 0;

    // Цвета
    bgColor = Qt::black;
    ballColor = Qt::green;
    floorColor = Qt::lightGray;
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(400, 400);
}
//! [4]

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::startAnimation () {
    animated = true;
    setHeight(startBallHeight);
    currentBallHeight = startBallHeight;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recalculateAnimation()));
    timer->start(40);
    zSpeed = gravity;
    emit animationStarted();
}

void GLWidget::stopAnimation () {
    timer->stop();
    delete timer;
    animated = false;
    emit animationStopped();
}

void GLWidget::setXRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != xRot) {
         xRot = angle;
         emit xRotationChanged(angle);
         updateGL();
     }
 }

void GLWidget::setYRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != yRot) {
         yRot = angle;
         emit yRotationChanged(angle);
         updateGL();
     }
 }

void GLWidget::setZRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != zRot) {
         zRot = angle;
         emit zRotationChanged(angle);
         updateGL();
     }
 }

void GLWidget::setHeight(int newHeight) {
    startBallHeight = currentBallHeight = newHeight;
    if (!animated) {
        zoom = 1.0/pow(startBallHeight, 0.4);
        emit zoomChanged(int(zoom*100));
        emit zoomChanged(double(zoom));
        resizeGL(width(), height());
        updateGL();
    }
}
void GLWidget::setZoom (double newZoom) {
    zoom = newZoom;
    resizeGL(width(), height());
    updateGL();
}

void GLWidget::setZoom (int newZoomPercentage) {
    double nzoom = double(newZoomPercentage)/100;
    setZoom(double(nzoom));
}

void GLWidget::turnCameraLeft() {
    setZRotation(zRot - 20);
}

void GLWidget::turnCameraRight() {
    setZRotation(zRot + 20);
}

void GLWidget::turnCameraUp() {
    setYRotation(yRot + 20);
}

void GLWidget::turnCameraDown() {
    setYRotation(yRot - 20);
}

void GLWidget::setBallElasticity(double newElasticity) {
    if (ballElasticity >= 0 && ballElasticity <= 1)
        ballElasticity = newElasticity;
}

void GLWidget::setBallElasticity(int newElasticityPercentage) {
    setBallElasticity(double(newElasticityPercentage)/100);
}

void GLWidget::setBgColor (QColor color) {
    if (color.isValid()) {
        bgColor = color;
        qglClearColor(bgColor);
        updateGL();
        emit this->bgColorChanged(color);
    }
}

void GLWidget::setBallColor (QColor color) {
    if (color.isValid()) {
        ballColor = color;
        updateGL();
        emit this->ballColorChanged(color);
    }
}

void GLWidget::setFloorColor (QColor color) {
    if (color.isValid()) {
        floorColor = color;
        updateGL();
        emit this->floorColorChanged(color);
    }
}

void GLWidget::chooseBallColor () {
    QColor color = QColorDialog::getColor(ballColor, this, tr("Выберите цвет мяча"));
    setBallColor(color);
}

void GLWidget::chooseFloorColor () {
    QColor color = QColorDialog::getColor(floorColor, this, tr("Выберите цвет плоскости"));
    setFloorColor(color);
}

void GLWidget::chooseBgColor () {
    QColor color = QColorDialog::getColor(bgColor, this, tr("Выберите цвет фона"));
    setBgColor(color);
}

void GLWidget::setBallRadius(double newRadius) {
    if (radius != newRadius) {
        radius = newRadius;
        updateGL();
    }
}

//! [6]
void GLWidget::initializeGL()
{
    qglClearColor(bgColor);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    static GLfloat lightPosition[4] = { 55.5, 5.0, 55.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
//! [6]

void GLWidget::recalculateAnimation () {
    double nextSpeed = zSpeed + gravity;
    if (currentBallHeight > 0) {
        // Если мяч в полёте и лететь ему ещё далеко
        if (currentBallHeight+nextSpeed >= 0) {
            currentBallHeight += nextSpeed;
            zSpeed = nextSpeed;
        // А вот тут он уже подлетает к "земле"
        } else {
            currentBallHeight = 0;
            zSpeed = -zSpeed + (1.0 - ballElasticity) * zSpeed - gravity;
        }
    } else {
        if (nextSpeed < 0.01)
            stopAnimation(); // Хватит я сказал!
        if (zSpeed < 0)
            zSpeed = nextSpeed = -zSpeed + (1.0 - ballElasticity) * zSpeed - gravity;
        currentBallHeight = nextSpeed;
    }
    updateGL();
}

//! [7]
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0-xoffs, 0.0+yoffs);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    // Рисование сферы
    glShadeModel(GL_SMOOTH);
    glPushMatrix(); // Засейвились
        glTranslated(0.0, 0.0, currentBallHeight+radius);
        GLUquadric * quad = gluNewQuadric();
        qglColor(ballColor);
        gluSphere(quad, radius, quality, quality);
    glPopMatrix();

    // Рисование "пола"
    glShadeModel(GL_FLAT);
    qglColor(floorColor);
    // Верхняя грань
    glBegin(GL_POLYGON);
         glNormal3d( 0.0,  0.0, 1.0);
         glVertex3d(-5.0, -5.0, 0.0);
         glVertex3d( 5.0, -5.0, 0.0);
         glVertex3d( 5.0,  5.0, 0.0);
         glVertex3d(-5.0,  5.0, 0.0);
    glEnd();
    // Нижняя грань
    glBegin(GL_POLYGON);
         glNormal3d( 0.0,  0.0, -1.0);
         glVertex3d(-5.0, -5.0, -0.1);
         glVertex3d(-5.0,  5.0, -0.1);
         glVertex3d( 5.0,  5.0, -0.1);
         glVertex3d( 5.0, -5.0, -0.1);
    glEnd();
    // Боковая грань, ближняя к наблюдателю
    glBegin(GL_POLYGON);
         glNormal3d( 1.0,  0.0,  0.0);
         glVertex3d( 5.0, -5.0, -0.1);
         glVertex3d( 5.0,  5.0, -0.1);
         glVertex3d( 5.0,  5.0,  0.0);
         glVertex3d( 5.0, -5.0,  0.0);
    glEnd();
    // Боковая грань, дальше от наблюдателя
    glBegin(GL_POLYGON);
         glNormal3d(-0.1,  0.0,  0.0);
         glVertex3d(-5.0, -5.0, -0.1);
         glVertex3d(-5.0, -5.0,  0.0);
         glVertex3d(-5.0,  5.0,  0.0);
         glVertex3d(-5.0,  5.0, -0.1);
    glEnd();
    glBegin(GL_POLYGON);
         glNormal3d( 0.0, 1.0,  0.0);
         glVertex3d(-5.0, 5.0, -0.1);
         glVertex3d(-5.0, 5.0,  0.0);
         glVertex3d( 5.0, 5.0,  0.0);
         glVertex3d( 5.0, 5.0, -0.1);
    glEnd();
    glBegin(GL_POLYGON);
         glNormal3d( 0.0, -1.0, 0.0);
         glVertex3d(-5.0, -5.0, -0.1);
         glVertex3d( 5.0, -5.0, -0.1);
         glVertex3d( 5.0, -5.0,  0.0);
         glVertex3d(-5.0, -5.0,  0.0);
    glEnd();
}
//! [7]

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, double(width)/double(height), 0.1, 800.0);
    gluLookAt(
            15.0/zoom, 0.0, double(startBallHeight)/2.0, // Точка с к-рой смотрим
            0.0,       0.0, double(startBallHeight)/2.0, // Точка в к-рую смотрим
            0.0,       0.0, 1.0                          // Вектор наклона взгляда
            );
    glMatrixMode(GL_MODELVIEW);
}

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setYRotation(yRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        float scaler=0.04;
        xoffs -= (dx*scaler/zoom);
        yoffs -= (dy*scaler/zoom);
        resizeGL(width(),height());
        updateGL();
    } else if(event->buttons() & Qt::MidButton) {
        double newZoom = zoom + 0.01 * dy * zoom;
        if (newZoom >= 0.01 && newZoom <= 10) {
            setZoom(newZoom);
            emit zoomChanged(int(zoom*100));
            emit zoomChanged(double(zoom));
        }
    }
    lastPos = event->pos();
}
//! [10]

void GLWidget::wheelEvent(QWheelEvent *event) {
    int numDegrees = event->delta() / 32;
    double newZoom = zoom+numDegrees*zoom*0.04;
    if (newZoom>=0.01 && newZoom<=10) { // Между 1% и 500%
        setZoom(newZoom);
        emit zoomChanged(int(zoom*100));
        emit zoomChanged(double(zoom));
    }
}
