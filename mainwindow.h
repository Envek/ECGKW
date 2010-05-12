#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showHelp();
    void showAbout();
    void showTz();
    void switchFullScreen();

protected:
    void changeEvent(QEvent *e);
//    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
