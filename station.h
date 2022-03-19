#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QObject>
#include <vector>
#include "Train.h"
#include <QMutex>
#include <QDebug>
#include <QLabel>

#define NUM_OF_PLATFORMS 5

class Station : public QThread
{
    Q_OBJECT
public:
    explicit Station(QLabel** _train_labels, QObject *parent = nullptr);
    void run();

private:
    int findFreePlatform();

private:
    unsigned int second_counter;
    bool gate_in_open;
//    bool gate_out_open;
    bool exit_line_free;
    std::vector<Train*> in_queue;
    std::vector<Train*> out_queue;
    Train* platforms[NUM_OF_PLATFORMS];
    QMutex m1;
    QLabel** train_labels;

signals:
    void TrainComing(Train*, QLabel*);
    void TrainLeaving(Train*, QLabel*);

public slots:
    void onSignalIn(Train*);
    void onArrivedAtPlatform(Train*);
    void onFreeExitLine(Train*);
    void onPlatformFree(Train*);
    void onSecondUpdate();
};

#endif // STATION_H
