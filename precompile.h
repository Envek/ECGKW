#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#include <math.h>

#if defined __cplusplus
#include <QMainWindow>
#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#ifdef QT5
    #include <QApplication>
#else
    #include <QtGui/QApplication>
#endif
#include <QDialog>
#include <QMatrix4x4>
#include <QVector3D>
#include <QStateMachine>
#include <QSignalTransition>
#endif

#endif // PRECOMPILE_H
