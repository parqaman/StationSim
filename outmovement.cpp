#include "outmovement.h"

OutMovement::OutMovement(QObject *parent)
    : QThread{parent}
    , exit_platform_duration(5)
    , occupy_exit_line_duration(20)
    , moving_train(nullptr)
{
}

void OutMovement::run()
{
    int cooldown = 0;
    while (true) {
        sleep(1);
        if(moving_train != nullptr && cooldown < occupy_exit_line_duration){
            cooldown++;
        }
        if(moving_train != nullptr && cooldown == occupy_exit_line_duration){
            emit OutLineFree(moving_train);
            moving_train = nullptr;
            cooldown = 0;
        }
        if(moving_train != nullptr && cooldown == exit_platform_duration){
            emit PlatformFree(moving_train);
        }
    }
}

void OutMovement::onLeavingTrain(Train* train)
{
    moving_train = train;
}
