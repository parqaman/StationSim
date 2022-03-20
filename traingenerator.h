#ifndef TRAINGENERATOR_H
#define TRAINGENERATOR_H

#include <QThread>
#include "Train.h"
#include <QMutex>

class TrainGenerator : public QThread
{
    Q_OBJECT
public:
    explicit TrainGenerator(QObject* parent = 0);
    void run();

    void setTrain_sleep_time(int newTrain_sleep_time);

    void setTrain_cycle(int newTrain_cycle);

private:
    int cycle_counter;
    int train_cycle;
    int train_sleep_time;
    QMutex m1;


signals:
    void TrainGenerated(Train*);

public slots:
    void onSecondUpdate();
};

#endif // TRAINGENERATOR_H
