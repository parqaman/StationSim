#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    station_thread = new Station(this);
    train_thread = new TrainGenerator(this);
    in_movement_thread = new InMovement(this);
    out_movement_thread = new OutMovement(this);

    connect(train_thread, SIGNAL(TrainGenerated(Train*)), station_thread, SLOT(onSignalIn(Train*)));
    connect(station_thread, SIGNAL(TrainComing(Train*)), in_movement_thread, SLOT(onTrainComing(Train*)));
    connect(station_thread, SIGNAL(TrainLeaving(Train*)), out_movement_thread, SLOT(onLeavingTrain(Train*)));
    connect(in_movement_thread, SIGNAL(ArrivedAtPlatform(Train*)), station_thread, SLOT(onArrivedAtPlatform(Train*)));
    connect(out_movement_thread, SIGNAL(PlatformFree(Train*)), station_thread, SLOT(onPlatformFree(Train*)));
    connect(out_movement_thread, SIGNAL(OutLineFree(Train*)), station_thread, SLOT(onFreeExitLine(Train*)));

    train_thread->start();
    in_movement_thread->start();
    out_movement_thread->start();
    station_thread->start();

    QLabel* q = new QLabel(this);
    q->setGeometry(1410, 480, 261, 191);
    q->setProperty("text", "text label");
    q->setStyleSheet("font: 18pt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

