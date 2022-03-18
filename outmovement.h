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
    int exit_platform_duration;
    int occupy_exit_line_duration;
    Train* moving_train;
    QMutex m;

signals:
    void OutLineFree(Train*);
    void PlatformFree(Train*);

public slots:
    void onLeavingTrain(Train* );
};

#endif // OUTMOVEMENT_H