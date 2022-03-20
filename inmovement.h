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

    void setIn_movement_duration(int newIn_movement_duration);

private:
    void move_label(int platform, int cooldown);

private:
    int second_counter;
    int cooldown;
    int in_movement_duration;
    Train* moving_train;
    QMutex m1;

signals:
    void MoveLabel(int platform, int timer);
    void ArrivedAtPlatform(Train*);

public slots:
    void onTrainComing(Train*);
    void onHalfSecondUpdate(int);
};

#endif // INMOVEMENT_H
