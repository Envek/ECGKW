#include "precompile.h"
#ifdef QT5
    #include <QApplication>
#else
    #include <QtGui/QApplication>
#endif
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#ifndef QT5
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    a.setOrganizationName("Новиков Андрей, группа 753, АмГУ");
    a.setApplicationName("Курсовая работа по предмету «Компьютерная графика» по теме: Визуализация трёхмерной картины скачкообразного движения мяча, брошенного на горизонтальную поверхность с заданнной высоты.");
    a.setApplicationVersion("1.0");
    MainWindow w;
    w.show();
    return a.exec();
}
