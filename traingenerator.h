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

private:
    int cycle_counter;


signals:
    void TrainGenerated(Train*);

public slots:
    void onSecondUpdate();
};

#endif // TRAINGENERATOR_H
