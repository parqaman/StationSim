#include "station.h"

Station::Station(QObject *parent)
    : QThread{parent}
    , second_counter{1}
    , gate_in_open{true}
    , gate_out_open{true}
    , moving_out_train(nullptr)
    , cooldown_out(5)
    , exit_line_cooldown(-1)
    , holded_train_counter(0)
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        platforms[i] = nullptr;
    }
}

void Station::run()
{
    int tmp_cooldown = -1;
    while (true) {
        QMutex m;
        m.lock();
        if(second_counter == 1){
            second_counter = 0;
            //work
            tmp_cooldown = exit_line_cooldown;
            if(tmp_cooldown == 1){
                gate_out_open = true;
            }

            if(tmp_cooldown == 0)
                exit_line_cooldown = -1;

            for(int i = 0; i < NUM_OF_PLATFORMS; i++){
                if(platforms[i] != nullptr){
                    if(platforms[i]->getStop_duration() > 0){
                        platforms[i]->reduce_stop_duration();
                    }
                    if(platforms[i]->getStop_duration() == 0 && platforms[i]->getListed() == false){
                        out_queue.push_back(platforms[i]);
                        emit ChangeColorToRed(i);
                        platforms[i]->setListed(true);
                    }
                }
            }

            if(cooldown_out != 0 && moving_out_train != nullptr){
                cooldown_out--;
            }
            if(cooldown_out == 0 && moving_out_train != nullptr){
                exit_line_cooldown = exit_line_max;
                int pos = moving_out_train->getPlatform_index();
                platforms[pos] = nullptr;
                moving_out_train = nullptr;
                cooldown_out = 5;
            }

            // notify that a train wants to leave its platform
            if(gate_out_open && !out_queue.empty()){
                gate_out_open = false;
                moving_out_train = out_queue.at(0);
                emit TrainLeaving(moving_out_train);
                out_queue.erase(out_queue.begin());
            }

            // checks if any train wants to enter a platform
            if(gate_in_open && !in_queue.empty()){
                int pos = findFreePlatform();
                if(pos != -1){
                    gate_in_open = false;
                    in_queue.at(0)->setPlatform_index(pos);
                    emit AttachLabel(pos, in_queue.at(0)->getId());
                    emit TrainComing(in_queue.at(0));
                    in_queue.erase(in_queue.begin());
                    if(holded_train_counter > 0){
                        holded_train_counter--;
                    }
                }
                else {
                    holded_train_counter++;
                }
            }

            if(tmp_cooldown > 0){
                tmp_cooldown--;
                exit_line_cooldown--;
            }
        }
        m.unlock();
    }
}

int Station::findFreePlatform()
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        if(platforms[i] == nullptr){
            return i;
        }
    }

    return -1;
}

void Station::setExit_line_max(int newExit_line_max)
{
    exit_line_max = newExit_line_max;
}

void Station::onSignalIn(Train* train)
{
    in_queue.push_back(train);
}

void Station::onArrivedAtPlatform(Train* tr)
{
    int pos = tr->getPlatform_index();
    platforms[pos] = tr;
    gate_in_open = true;
}

void Station::onFreeExitLine(Train* train)
{
    delete train;
}

void Station::onPlatformFree(Train* train)
{
    platforms[train->getPlatform_index()] = nullptr;
    emit DetachLabel(train->getPlatform_index());
}

void Station::onSecondUpdate()
{
    second_counter = 1;
}

