#include "outmovement.h"

OutMovement::OutMovement(QObject *parent)
    : QThread{parent}
    , cooldown(0)
    , exit_platform_duration(5)
    , occupy_exit_line_duration(20)
    , moving_train(nullptr)
    , moving_label(nullptr)
{
}

void OutMovement::run()
{
    while (true) {
        usleep(500000);
        m.lock();
        if(moving_train != nullptr && cooldown < occupy_exit_line_duration * 2){
            move_label(moving_train->getPlatform_index(), cooldown);
            cooldown++;
        }
        if(moving_train != nullptr && cooldown == occupy_exit_line_duration * 2){
            emit OutLineFree(moving_train);
            moving_train = nullptr;
            cooldown = 0;
        }
        if(moving_train != nullptr && cooldown == exit_platform_duration * 2){
            emit PlatformFree(moving_train);
            moving_label = nullptr;
        }
        m.unlock();
    }
}

void OutMovement::move_label(int platform, int cooldown)
{
    if(moving_label != nullptr){
        if(platform == 0){
            moving_label->move(moving_label->x() - 105, moving_label->y());
        }

        else if(platform == 1){
            if(cooldown != 2)
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y(), moving_label->width(), moving_label->height());
            else
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y() + 60, moving_label->width(), moving_label->height());
        }
        else if(platform == 2){
            if(cooldown < 2 || cooldown > 3)
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y(), moving_label->width(), moving_label->height());
            else
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y() + 60, moving_label->width(), moving_label->height());
        }
        else if(platform == 3){
            if(cooldown < 2 || cooldown > 4)
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y(), moving_label->width(), moving_label->height());
            else
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y() + 60, moving_label->width(), moving_label->height());
        }
        else if(platform == 4){
            if(cooldown < 2 || cooldown > 5)
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y(), moving_label->width(), moving_label->height());
            else
                moving_label->setGeometry(moving_label->x() - 105, moving_label->y() + 60, moving_label->width(), moving_label->height());
        }
    }
}

void OutMovement::onLeavingTrain(Train* train, QLabel* _label)
{
    m.lock();
    moving_label = _label;
    moving_train = train;
    m.unlock();
}

void OutMovement::onHalfSecondUpdate()
{
    if(moving_train != nullptr && cooldown < occupy_exit_line_duration * 2){
        move_label(moving_train->getPlatform_index(), cooldown);
        cooldown++;
    }
    if(moving_train != nullptr && cooldown == occupy_exit_line_duration * 2){
        emit OutLineFree(moving_train);
        moving_train = nullptr;
        cooldown = 0;
    }
    if(moving_train != nullptr && cooldown == exit_platform_duration * 2){
        emit PlatformFree(moving_train);
        moving_label = nullptr;
    }
}
