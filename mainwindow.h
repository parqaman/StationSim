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
    Ui::MainWindow *ui;
    TimeClock* time_clock_thread;
    Station* station_thread;
    TrainGenerator* train_thread;
    InMovement* in_movement_thread;
    OutMovement* out_movement_thread;
    QLabel** train_labels;
    int sec_total;
    int sec;
    int min;
    int hr;
    int day;

public slots:
    void onSecondUpdate();
    void attach_label(int, int);
    void detach_label(int);
    void move_label_in(int platform, int cooldown);
    void move_label_out(int platform, int cooldown);

private slots:
    void on_start_button_clicked();
    void on_confirm_turbo_spinbox_clicked();
    void onChangeColorToRed(int pos);
    void on_pause_continue_button_clicked();
    void on_exit_button_clicked();
};
#endif // MAINWINDOW_H
