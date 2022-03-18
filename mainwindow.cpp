#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->example_label->hide();
    train_labels = new QLabel*[NUM_OF_PLATFORMS];
    createLabels(NUM_OF_PLATFORMS);

    station_thread = new Station(train_labels, this);
    in_movement_thread = new InMovement(this);
    out_movement_thread = new OutMovement(this);
    train_thread = new TrainGenerator(this);

    connect(train_thread, SIGNAL(TrainGenerated(Train*)), station_thread, SLOT(onSignalIn(Train*)));
    connect(station_thread, SIGNAL(TrainComing(Train*, QLabel*)), in_movement_thread, SLOT(onTrainComing(Train*, QLabel*)));
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

QLabel *MainWindow::getPlatformLabel(int pos) const
{
    if(pos >= 0 && pos <= NUM_OF_PLATFORMS - 1)
        return train_labels[pos];
    return nullptr;
}

void MainWindow::createLabels(int num_of_labels)
{
    for(int i = 0; i < num_of_labels; i++){
        train_labels[i] = new QLabel(this);
        train_labels[i]->setGeometry(2180, 488, 80, 30);
        train_labels[i]->setStyleSheet("font: 10pt; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border: 2px solid black");
        train_labels[i]->setText(QString::fromStdString("Train " + std::to_string(499 + 1)));
        train_labels[i]->hide();
    }

}
