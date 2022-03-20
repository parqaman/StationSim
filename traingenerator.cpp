#include "traingenerator.h"
#include <QDebug>

TrainGenerator::TrainGenerator(QObject* parent)
    :
      QThread(parent)
    , cycle_counter(0)
    , train_sleep_time(0)
{
}

void TrainGenerator::run()
{
    int train_cycle = 20;
    while (true) {
        m1.lock();
        if(cycle_counter == train_cycle){
            Train* newtrain = new Train(train_sleep_time);
            emit TrainGenerated(newtrain);
//            qDebug() << "Train generated emit sent";
            cycle_counter = 0;
        }
        m1.unlock();
    }
}

void TrainGenerator::setTrain_sleep_time(int newTrain_sleep_time)
{
    m1.lock();
    train_sleep_time = newTrain_sleep_time;
    m1.unlock();
}

void TrainGenerator::onSecondUpdate()
{
    cycle_counter++;
}
