#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , counter(0)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->example_label->hide();
    ui->example_label_2->hide();
    ui->example_label_3->hide();

    ui->turbo_spinbox->setMinimum(1);
    ui->turbo_spinbox->setMaximum(1000);

    train_labels = new QLabel*[NUM_OF_PLATFORMS];
    createLabels(NUM_OF_PLATFORMS);

    time_clock_thread = new TimeClock(this);
    station_thread = new Station(this);
    in_movement_thread = new InMovement(this);
    out_movement_thread = new OutMovement(this);
    train_thread = new TrainGenerator(this);

    connect(time_clock_thread, SIGNAL(HalfSecondUpdate(int)), in_movement_thread, SLOT(onHalfSecondUpdate(int)));
    connect(time_clock_thread, SIGNAL(HalfSecondUpdate(int)), out_movement_thread, SLOT(onHalfSecondUpdate(int)));
    connect(time_clock_thread, SIGNAL(OneSecondUpdate()), station_thread, SLOT(onSecondUpdate()));
    connect(time_clock_thread, SIGNAL(OneSecondUpdate()), train_thread, SLOT(onSecondUpdate()));
    connect(time_clock_thread, SIGNAL(OneSecondUpdate()), this, SLOT(onSecondUpdate()));

    connect(train_thread, SIGNAL(TrainGenerated(Train*)), station_thread, SLOT(onSignalIn(Train*)));

    connect(station_thread, SIGNAL(AttachLabel(int,int)), this, SLOT(attach_label(int,int)));
    connect(station_thread, SIGNAL(DetachLabel(int)), this, SLOT(detach_label(int)));

    connect(station_thread, SIGNAL(TrainComing(Train*)), in_movement_thread, SLOT(onTrainComing(Train*)));
    connect(station_thread, SIGNAL(TrainLeaving(Train*)), out_movement_thread, SLOT(onLeavingTrain(Train*)));

    connect(in_movement_thread, SIGNAL(MoveLabel(int,int)), this, SLOT(move_label_in(int,int)));
    connect(in_movement_thread, SIGNAL(ArrivedAtPlatform(Train*)), station_thread, SLOT(onArrivedAtPlatform(Train*)));

    connect(out_movement_thread, SIGNAL(MoveLabel(int,int)), this, SLOT(move_label_out(int,int)));
    connect(out_movement_thread, SIGNAL(PlatformFree(Train*)), station_thread, SLOT(onPlatformFree(Train*)));
    connect(out_movement_thread, SIGNAL(OutLineFree(Train*)), station_thread, SLOT(onFreeExitLine(Train*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete time_clock_thread;
    delete station_thread;
    delete in_movement_thread;
    delete out_movement_thread;
    delete train_thread;
    delete [] train_labels;
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
        train_labels[i]->setGeometry(2209, 489, 81, 30);
        train_labels[i]->setStyleSheet("font: 10pt; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border: 2px solid black");
        train_labels[i]->setText(QString::fromStdString("Train " + std::to_string(499 + 1)));
        train_labels[i]->hide();
    }

}

void MainWindow::move_label_in(int platform, int cooldown)
{
    if(train_labels[platform] != nullptr){
        if(platform == 0){
            train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
        }

        else if(platform == 1){
            if(cooldown != 2)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() - 60);
        }
        else if(platform == 2){
            if(cooldown < 2 || cooldown > 3)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() - 60);
        }
        else if(platform == 3){
            if(cooldown < 2 || cooldown > 4)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() - 60);
        }
        else if(platform == 4){
            if(cooldown < 2 || cooldown > 5)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() - 60);
        }
    }
}

void MainWindow::move_label_out(int platform, int cooldown)
{
    if(train_labels[platform] != nullptr){
        if(platform == 0){
            train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
        }

        else if(platform == 1){
            if(cooldown != 7)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() + 60);
        }
        else if(platform == 2){
            if(cooldown < 6 || cooldown > 7)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() + 60);
        }
        else if(platform == 3){
            if(cooldown < 5 || cooldown > 7)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() + 60);
        }
        else if(platform == 4){
            if(cooldown < 4 || cooldown > 7)
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y());
            else
                train_labels[platform]->move(train_labels[platform]->x() - 105, train_labels[platform]->y() + 60);
        }
    }
}

void MainWindow::onSecondUpdate()
{
    this->ui->clock_display->display(counter);
    counter++;
}

void MainWindow::attach_label(int index, int id)
{
    train_labels[index]->setText(QString::fromStdString("Train " + std::to_string(id)));
    train_labels[index]->show();
}

void MainWindow::detach_label(int index)
{
    train_labels[index]->hide();
    train_labels[index]->setGeometry(2209, 489, 81, 30);
}

void MainWindow::on_start_button_clicked()
{

    int sleep_time = this->ui->platform_duration_spinbox->value();
    this->train_thread->setTrain_sleep_time(sleep_time);

    int in_out_time = this->ui->enter_leave_duration_spinbox->value();
    this->in_movement_thread->setIn_movement_duration(in_out_time);
    this->out_movement_thread->setExit_platform_duration(in_out_time);

    ui->enter_leave_duration_heading->hide();
    ui->enter_leave_duration_spinbox->hide();
    ui->platform_duration_spinbox->hide();
    ui->platform_stop_duration_heading->hide();
    ui->start_button->hide();
    ui->cover->hide();
    train_thread->start();
    station_thread->start();
    in_movement_thread->start();
    out_movement_thread->start();

    time_clock_thread->start();
}


void MainWindow::on_confirm_turbo_spinbox_clicked()
{
    int value = ui->turbo_spinbox->value();
    time_clock_thread->setTurbo(value);
}

