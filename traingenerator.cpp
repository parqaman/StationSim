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
        if(cycle_counter == train_cycle){
            Train* newtrain = new Train(sleep_duration);
            emit TrainGenerated(newtrain);
            cycle_counter = 0;
        }
    }
}

void TrainGenerator::onSecondUpdate()
{
    cycle_counter++;
}
