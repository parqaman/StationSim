#include "traingenerator.h"
#include <QDebug>

TrainGenerator::TrainGenerator(QObject* parent)
    :
      QThread(parent)
    , cycle_counter(0)
{
}

void TrainGenerator::run()
{
    int sleep_duration = 70;
    int train_cycle = 20;
    while (true) {
        QMutex m1;
        m1.lock();
        if(cycle_counter == train_cycle){
            Train* newtrain = new Train(sleep_duration);
            emit TrainGenerated(newtrain);
//            qDebug() << "Train generated emit sent";
            cycle_counter = 0;
        }
        m1.unlock();
    }
}

void TrainGenerator::onSecondUpdate()
{
    cycle_counter++;
}
