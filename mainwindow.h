#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "traingenerator.h"
#include "Train.h"
#include "station.h"
#include "inmovement.h"
#include "outmovement.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Station* station_thread;
    TrainGenerator* train_thread;
    InMovement* in_movement_thread;
    OutMovement* out_movement_thread;
};
#endif // MAINWINDOW_H
