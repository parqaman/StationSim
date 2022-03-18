#include "inmovement.h"

InMovement::InMovement(QObject *parent)
    : QThread{parent}
    , in_movement_duration{5}
    , moving_train(nullptr)
{
}

void InMovement::run()
{
    int cooldown = 0;
    while (true) {
        sleep(1);
        m.lock();
        if(moving_train != nullptr && cooldown < in_movement_duration){
            cooldown++;
        }
        else if(moving_train != nullptr && cooldown == in_movement_duration){
            emit ArrivedAtPlatform(moving_train);
            moving_train = nullptr;
            cooldown = 0;
        }
        m.unlock();
    }
}

void InMovement::onTrainComing(Train* train)
{
    m.lock();
    moving_train = train;
    m.unlock();
}
