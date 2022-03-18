#ifndef INMOVEMENT_H
#define INMOVEMENT_H

#include <QThread>
#include <QObject>
#include "Train.h"
#include <QDebug>
#include <QMutex>

class InMovement : public QThread
{
    Q_OBJECT
public:
    explicit InMovement(QObject *parent = nullptr);
    void run();

private:
    int in_movement_duration;
    Train* moving_train;
    QMutex m;

signals:
    void ArrivedAtPlatform(Train*);

public slots:
    void onTrainComing(Train*);
};

#endif // INMOVEMENT_H
