#include "traingenerator.h"

TrainGenerator::TrainGenerator(QObject* parent)
    :
      QThread(parent)
{
}

void TrainGenerator::run()
{
    int sleep_duration = 70;
    int train_cycle = 20;
    while (true) {
        sleep(train_cycle);
        Train* newtrain = new Train(sleep_duration);
        emit TrainGenerated(newtrain);
    }
}
