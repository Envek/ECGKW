#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStateMachine>
#include <QSignalTransition>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
// Загружаем интерфейс пользователя из формы и устанавливаем действия в меню
    ui->setupUi(this);
    connect(ui->action_start, SIGNAL(triggered()), ui->startButton, SLOT(click()));
    connect(ui->action_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_help, SIGNAL(triggered()), this, SLOT(showHelp()));
    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->action_tech, SIGNAL(triggered()), this, SLOT(showTz()));
// Заводим машину состояний
    QStateMachine *animation = new QStateMachine(this);
    QState *idle = new QState();
    QState *animating = new QState();
    animating->assignProperty(ui->startButton,"text", tr("&Стоп"));
    animating->assignProperty(ui->startButton,"icon", QIcon(":/icons/control-stop-square.png"));
    animating->assignProperty(ui->action_start,"text",tr("О&становить анимацию"));
    animating->assignProperty(ui->action_start,"icon", QIcon(":/icons/control-stop-square.png"));
    idle->assignProperty(ui->startButton,"text", tr("Пу&ск"));
    idle->assignProperty(ui->startButton,"icon", QIcon(":/icons/control.png"));
    idle->assignProperty(ui->action_start,"text",tr("Запу&стить анимацию"));
    idle->assignProperty(ui->action_start,"icon", QIcon(":/icons/control.png"));
    QSignalTransition *startTransition = new QSignalTransition(ui->startButton, SIGNAL(clicked()), idle);
    startTransition->setTargetState(animating);
    QSignalTransition *stopTransition = new QSignalTransition(ui->startButton, SIGNAL(clicked()), animating);
    stopTransition->setTargetState(idle);
    QSignalTransition *doneTransition = new QSignalTransition(ui->widget, SIGNAL(animationStopped()), animating);
    doneTransition->setTargetState(idle);
    connect(startTransition, SIGNAL(triggered()), ui->widget, SLOT(startAnimation()));
    connect(stopTransition, SIGNAL(triggered()), ui->widget, SLOT(stopAnimation()));
    idle->addTransition(startTransition);
    animating->addTransition(stopTransition);
    animating->addTransition(doneTransition);
    animation->addState(idle);
    animation->addState(animating);
    animation->setInitialState(idle);
    animation->start();
 // В Linux мячик иногда сразу не отображается...
    ui->widget->updateGL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::showHelp() {
    QMessageBox::about(this, tr("Справка по курсовой работе"),
                 tr("<h2>Интерфейс пользователя</h2>"
                    "<p>Окно программы состоит из области визуализации и области"
                    " управляющих элементов справа от неё. Среди управляющих "
                    "элементов можно выделить вертикальный ползунок задания "
                    "высоты мяча, кнопки поворота камеры, поля задания высоты "
                    "мяча и масштаба, ползунок задания упругости мяча, кнопки "
                    "задания цвета и кнопку запуска визуализации.</p>"
                    "<h2>Запуск</h2>"
                    "<p>С помощью вертикального ползунка или текстового поля "
                    "выставьте начальную высоту мяча и нажмите кнопку «Пуск».</p>"
                    "<h2>Управление &laquo;камерой&raquo;</h2>"
                    "<p>Угол обзора сцены можно изменять при помощи кнопок с "
                    "изображёнными на них стрелками, при помощи курсорных клавиш"
                    " на клавиатуре или движениями мыши с зажатой левой кнопкой.</p>"
                    "<p>Перемещение камеры осуществляется движениями мыши с "
                    "зажатой правой кнопкой</p>"
                    "Приближение/отдаление осуществляется поворотом колеса мыши"
                    ", движениями мыши вверх/вниз с зажатой средней кнопкой или "
                    "выставлением нужного значения в текстовое поле со значком лупы.</p>"
                    "<h2>Цвета</h2>"
                    "<p>Выбор цвета мяча, плоскости или фона осуществляется по "
                    "нажатию соответствующих кнопок в интерфейсе программы, "
                    "после чего появляется окно выбора цвета, в котором можно "
                    "выбрать цвет, а так же задать свой.</p>"
                    "<h2>Полноэкранный режим</h2>"
                    "<p>В полноэкранном режиме окно программы (вместе с управля"
                    "ющими элементами, но без строк меню и состояния) "
                    "занимает весь экран. Переход в этот режим и обратно осущес"
                    "твляется по нажатию кнопки, расположенной между кнопками "
                    "поворота камерой или клавиатурной комбинацией Alt+Enter</p>"
));
}

void MainWindow::showAbout() {
    QMessageBox::about(this, tr("О программе"),
                 tr("<center><h2>КУРСОВАЯ РАБОТА</h2></center>"
                    "<p>по дисциплине «Компьютерная и инженерная графика»<br />"
                    "по теме: Визуализация трёхмерной картины скачкообразного движения"
                    " мяча, брошенного на горизонтальную поверхность.</p>"
                    "<p>Исполнитель<br />"
                    "студент группы 753&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                    "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;А.А. Новиков</p>"
                    "<br />"
                    "<p><strong>Курсовая работа</strong> написана на языке "
                    "программирования <strong>C++</strong> с использованием "
                    "програмного каркаса <strong>Qt</strong> и библиотеки для "
                    "работы с трёхмерной графикой <strong>OpenGL</strong>.</p>"
));
}

void MainWindow::showTz() {
    QMessageBox::about(this, tr("Техническое задание, вариант № 13"),
                 tr("<p style=\"clear: both\">Реализовать программу "
                    "визуализации трёхмерной картины скачкообразного "
                    "движения мяча, брошенного на горизонтальную поверхность "
                    "с заданнной высоты. Предусмотреть возможность задания "
                    "первоначальной высоты.</p>"
));
}

void MainWindow::switchFullScreen() {
    if (isFullScreen()) {
        showNormal();
        ui->statusBar->show();
        ui->menuBar->show();
        ui->fullScreenButton->setIcon(QIcon(":/icons/application-resize-full.png"));
    } else {
        ui->statusBar->hide();
        ui->menuBar->hide();
        ui->fullScreenButton->setIcon(QIcon(":/icons/application-resize-actual.png"));
        showFullScreen();
    }
}
