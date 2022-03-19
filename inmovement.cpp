#include "inmovement.h"

InMovement::InMovement(QObject *parent)
    : QThread{parent}
    , second_counter(0)
    , cooldown(0)
    , in_movement_duration{5}
    , moving_train(nullptr)
{
}

void InMovement::run()
{
    while (true) {
        QMutex m1;
        m1.lock();
        if(second_counter == 1){
            second_counter = 0;
            //work
            if(moving_train != nullptr && cooldown < in_movement_duration*2){
                emit MoveLabel(moving_train->getPlatform_index(), cooldown);
                cooldown++;
            }
            else if(moving_train != nullptr && cooldown == in_movement_duration*2){
                emit ArrivedAtPlatform(moving_train);
                moving_train = nullptr;
                cooldown = 0;
            }
        }
        m1.unlock();
    }
}

void InMovement::onTrainComing(Train* train)
{
    moving_train = train;
}

void InMovement::onHalfSecondUpdate(int timer)
{
    second_counter = 1;
}
