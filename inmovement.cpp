#include "inmovement.h"

InMovement::InMovement(QObject *parent)
    : QThread{parent}
    , in_movement_duration{5}
    , moving_train(nullptr)
{
}

void InMovement::run()
{
    int timer = 0;
    int cooldown = 0;
    while (true) {
//        qDebug() << timer << " - InMovement";
        timer++;
        sleep(1);
        if(moving_train != nullptr && cooldown < in_movement_duration){
            cooldown++;
        }
        else if(moving_train != nullptr && cooldown == in_movement_duration){
            emit ArrivedAtPlatform(moving_train);
            moving_train = nullptr;
            cooldown = 0;
        }
    }
}

void InMovement::onTrainComing(Train* train)
{
    moving_train = train;
}
