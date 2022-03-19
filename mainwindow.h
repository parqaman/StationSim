#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "traingenerator.h"
#include "Train.h"
#include "station.h"
#include "inmovement.h"
#include "outmovement.h"
#include "timeclock.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel* getPlatformLabel(int pos) const;

private:
    void createLabels(int num_of_labels);

private:
    int counter;
    Ui::MainWindow *ui;
    TimeClock* time_clock_thread;
    Station* station_thread;
    TrainGenerator* train_thread;
    InMovement* in_movement_thread;
    OutMovement* out_movement_thread;
    QLabel** train_labels;

public slots:
    void onSecondUpdate();
};
#endif // MAINWINDOW_H
