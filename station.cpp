#include "station.h"

Station::Station(QObject *parent)
    : QThread{parent}
    , second_counter{0}
    , gate_in_open{true}
    , exit_line_free{true}
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        free_platforms_index_list.push_back(i);
        platforms[i] = nullptr;
    }
}

void Station::run()
{
    int clock = 0;
    while (true) {
        QMutex m;
        m.lock();
        if(second_counter == 1){
            second_counter = 0;
            //work
            for(int i = 0; i < NUM_OF_PLATFORMS; i++){
                if(platforms[i] != nullptr){
                    if(platforms[i]->getStop_duration() > 0){
                        platforms[i]->reduce_stop_duration();
                    }
                    else {
                        if(platforms[i]->getListed() == false){
                            out_queue.push_back(platforms[i]);
                            platforms[i]->setListed(true);
                        }
                    }
                }
            }

            // notify that a train wants to leave its platform
            if(exit_line_free && !out_queue.empty()){
                Train* out_train = out_queue.at(0);
                qDebug() << "Train " << out_train->getId() << " is leaving platform " << out_train->getPlatform_index() + 1;
                emit TrainLeaving(out_train);
                exit_line_free = false;
                out_queue.erase(out_queue.begin());
            }

            // checks if any train wants to enter a platform
            if(gate_in_open && !in_queue.empty()){
                if(!free_platforms_index_list.empty()){
                    gate_in_open = false;
                    int pos = free_platforms_index_list.at(0);
                    free_platforms_index_list.erase(free_platforms_index_list.begin());
                    in_queue.at(0)->setPlatform_index(pos);
                    emit AttachLabel(pos, in_queue.at(0)->getId());
                    emit TrainComing(in_queue.at(0));
                    qDebug() << "Train " << in_queue.at(0)->getId() << " is entering platform " << pos + 1;
                    in_queue.erase(in_queue.begin());
                }
                else {
                    qDebug() << "A train is on hold";
                }
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

void Station::onSignalIn(Train* train)
{
    qDebug() << "Train generated emit received - ID: " << train->getId();
    in_queue.push_back(train);
}

void Station::onArrivedAtPlatform(Train* tr)
{
//    qDebug() << tr->getId() << " arrived at platform " << tr->getPlatform_index() + 1 ;
    int pos = tr->getPlatform_index();
    platforms[pos] = tr;
    gate_in_open = true;
}

void Station::onFreeExitLine(Train* train)
{
//    qDebug() << "Exit line is now free";
    delete train;
    exit_line_free = true;
}

void Station::onPlatformFree(Train* train)
{
//    qDebug() << train->getId() << " left platform " << train->getPlatform_index() + 1;
    emit DetachLabel(train->getPlatform_index());
    platforms[train->getPlatform_index()] = nullptr;
    free_platforms_index_list.push_back(train->getPlatform_index());
}

void Station::onSecondUpdate()
{
    second_counter = 1;
}

