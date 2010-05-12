# -------------------------------------------------
# Project created by QtCreator 2009-12-04T19:33:59
# -------------------------------------------------
QT += opengl
TARGET = ECGKW
TEMPLATE = app

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
