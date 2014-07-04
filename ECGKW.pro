# -------------------------------------------------
# Project created by QtCreator 2009-12-04T19:33:59
# -------------------------------------------------
QT += opengl
TARGET = ECGKW
TEMPLATE = app

# Qt 5 compatibility
contains(QT_VERSION, ^5.*) {
  QT += widgets
  DEFINES += QT5
}

# CONFIG += console precompile_header
PRECOMPILED_HEADER = precompile.h
SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp
HEADERS += mainwindow.h \
    glwidget.h \
    precompile.h
FORMS += mainwindow.ui
RESOURCES += resources.qrc
