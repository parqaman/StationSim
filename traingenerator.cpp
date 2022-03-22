#include "traingenerator.h"
#include <QDebug>

TrainGenerator::TrainGenerator(QObject* parent)
    :
      QThread(parent)
    , id_counter(0)
    , cycle_counter(0)
    , train_cycle(15)
    , train_sleep_time(0)
{
}

void TrainGenerator::run()
{
    while (true) {
        m1.lock();
        if(cycle_counter == train_cycle){
            Train* newtrain = new Train(++id_counter, train_sleep_time);
            emit TrainGenerated(newtrain);
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

void TrainGenerator::setTrain_cycle(int newTrain_cycle)
{
    m1.lock();
    train_cycle = newTrain_cycle;
    m1.unlock();
}

void TrainGenerator::onSecondUpdate()
{
    cycle_counter++;
}
