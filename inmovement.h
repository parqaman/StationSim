#ifndef INMOVEMENT_H
#define INMOVEMENT_H

#include <QThread>
#include <QObject>
#include "Train.h"
#include <QDebug>
#include <QMutex>
#include <QLabel>

class InMovement : public QThread
{
    Q_OBJECT
public:
    explicit InMovement(QObject *parent = nullptr);
    void run();

private:
    void move_label(int platform, int cooldown);

private:
    int in_movement_duration;
    Train* moving_train;
    QLabel* moving_label;
    QMutex m;

signals:
    void ArrivedAtPlatform(Train*);

public slots:
    void onTrainComing(Train*, QLabel*);
};

#endif // INMOVEMENT_H
