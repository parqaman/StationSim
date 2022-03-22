#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QObject>
#include <vector>
#include "Train.h"
#include <QMutex>
#include <QDebug>

#define NUM_OF_PLATFORMS 5

class Station : public QThread
{
    Q_OBJECT
public:
    explicit Station(QObject *parent = nullptr);
    void run();
    ~Station();
    void setExit_line_max(int newExit_line_max);

private:
    int findFreePlatform();

private:
    unsigned int second_counter;
    bool gate_in_open;
    bool gate_out_open;
    std::vector<Train*> in_queue;
    std::vector<Train*> out_queue;
    Train* platforms[NUM_OF_PLATFORMS];
    QMutex m1;
    Train* moving_out_train;
    int cooldown_in;
    int cooldown_out;
    int exit_line_cooldown;
    int holded_train_counter;
    int exit_line_max;

signals:
    void TrainComing(Train*);
    void TrainLeaving(Train*);
    void AttachLabel(int index, int id);
    void DetachLabel(int index);
    void ChangeColorToRed(int);

public slots:
    void onSignalIn(Train*);
    void onArrivedAtPlatform(Train*);
    void onFreeExitLine(Train*);
    void onPlatformFree(Train*);
    void onSecondUpdate();
};

#endif // STATION_H
