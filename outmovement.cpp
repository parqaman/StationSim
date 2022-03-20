#include "outmovement.h"

OutMovement::OutMovement(QObject *parent)
    : QThread{parent}
    , second_counter(1)
    , cooldown(0)
    , exit_platform_duration(5)
    , occupy_exit_line_duration(20)
    , moving_train(nullptr)
{
}

void OutMovement::run()
{
    while (true) {
        m1.lock();
        if(second_counter == 1){
            second_counter = 0;
            //work
            if(moving_train != nullptr && cooldown < (occupy_exit_line_duration * 2 + exit_platform_duration * 2)){
                if(cooldown < exit_platform_duration * 2){
                    emit MoveLabel(moving_train->getPlatform_index(), cooldown);
                }
                cooldown++;
            }
            if(moving_train != nullptr && cooldown == (occupy_exit_line_duration * 2 + exit_platform_duration * 2)){
//                emit OutLineFree(moving_train);
            }
            if(moving_train != nullptr && cooldown == exit_platform_duration * 2){
//                qDebug() << moving_train->getId() << " - left at its platform ";
                emit PlatformFree(moving_train);
                moving_train = nullptr;
                cooldown = 0;
            }
        }
        m1.unlock();
    }

}

void OutMovement::setExit_platform_duration(int newExit_platform_duration)
{
    m1.lock();
    exit_platform_duration = newExit_platform_duration;
    m1.unlock();
}

void OutMovement::onLeavingTrain(Train* train)
{
    moving_train = train;
}

void OutMovement::onHalfSecondUpdate(int timer)
{
    second_counter = 1;
}
