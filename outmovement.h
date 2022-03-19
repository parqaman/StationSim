#ifndef OUTMOVEMENT_H
#define OUTMOVEMENT_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include "Train.h"

class OutMovement : public QThread
{
    Q_OBJECT
public:
    explicit OutMovement(QObject *parent = nullptr);
    void run();

private:
    void move_label(int platform, int cooldown);

private:
    int second_counter;
    int cooldown;
    int exit_platform_duration;
    int occupy_exit_line_duration;
    Train* moving_train;
    QMutex m;

signals:
    void MoveLabel(int, int);
    void OutLineFree(Train*);
    void PlatformFree(Train*);

public slots:
    void onLeavingTrain(Train*);
    void onHalfSecondUpdate(int timer);
};

#endif // OUTMOVEMENT_H
