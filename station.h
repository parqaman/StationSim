#ifndef STATION_H
#define STATION_H

#include <QThread>
#include <QObject>
#include <vector>
#include "Train.h"
#include <QDebug>

#define NUM_OF_PLATFORMS 5

class Station : public QThread
{
    Q_OBJECT
public:
    explicit Station(QObject *parent = nullptr);
    void run();

private:
    int findFreePlatform();

private:
    bool gate_in_open;
    bool gate_out_open;
    std::vector<Train*> in_queue;
    std::vector<Train*> out_queue;
    Train* platforms[NUM_OF_PLATFORMS];

signals:
    void TrainComing(Train*);
    void TrainLeaving(Train*);

public slots:
    void onSignalIn(Train*);
    void onArrivedAtPlatform(Train*);
    void onFreeExitLine(Train*);
    void onPlatformFree(Train*);
};

#endif // STATION_H
