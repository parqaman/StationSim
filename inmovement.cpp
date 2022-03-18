#include "inmovement.h"

InMovement::InMovement(QObject *parent)
    : QThread{parent}
    , in_movement_duration{5}
    , moving_train(nullptr)
    , moving_label(nullptr)
{
}

void InMovement::run()
{
    int cooldown = 0;
    while (true) {
        usleep(500000);
        m.lock();
        if(moving_train != nullptr && cooldown < in_movement_duration*2){
            move_label(moving_train->getPlatform_index(), cooldown);
            cooldown++;
        }
        else if(moving_train != nullptr && cooldown == in_movement_duration*2){
            emit ArrivedAtPlatform(moving_train);
            moving_train = nullptr;
            cooldown = 0;
        }
        m.unlock();
    }
}

void InMovement::move_label(int platform, int cooldown)
{
    if(platform == 0){
        moving_label->setGeometry(moving_label->x() - 102, moving_label->y(), moving_label->width(), moving_label->height());
    }
    /*
    else if(platform == 1){
        if(cooldown != 1)
            moving_label->setGeometry(moving_label->x() - 92.4, moving_label->y(), moving_label->width(), moving_label->height());
        else
            moving_label->setGeometry(moving_label->x() - 185, moving_label->y() - 61, moving_label->width(), moving_label->height());
    }
    else if(platform == 2){
        if(cooldown < 1 || cooldown > 2)
            moving_label->setGeometry(moving_label->x() - 81.1, moving_label->y(), moving_label->width(), moving_label->height());
        else
            moving_label->setGeometry(moving_label->x() - 185, moving_label->y() - 61, moving_label->width(), moving_label->height());
    }
    */
}

void InMovement::onTrainComing(Train* train, QLabel* _label)
{
    m.lock();
    moving_label = _label;
    moving_train = train;
    m.unlock();
}
