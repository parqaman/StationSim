#include "station.h"

Station::Station(QObject *parent)
    : QThread{parent}
    , gate_in_open{true}
    , gate_out_open{true}
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        platforms[i] = nullptr;
    }

}

void Station::run()
{
    int timer = 0;
//    int in_cooldown = 0;
//    int out_cooldown = 0;
    while (true) {
//        qDebug() << timer << " - Station";
        timer++;
        sleep(1);
        // reduce stop duration of trains in platform
        for(int i = 0; i < NUM_OF_PLATFORMS; i++){
            if(platforms[i] != nullptr){
                if(platforms[i]->getStop_duration() > 0){
                    platforms[i]->reduce_stop_duration();
                }
                else {
                    if(gate_out_open){
                        out_queue.push_back(platforms[i]);
                    }
                }
            }
        }

        // notify that a train wants to leave its platform
        if(gate_out_open && !out_queue.empty()){
            gate_out_open = false;
            Train* out_train = out_queue.at(0);
            qDebug() << "Train " << out_train->getId() << " is leaving platform " << out_train->getPlatform_index() + 1;
            emit TrainLeaving(out_train);
            out_queue.erase(out_queue.begin());
        }

        // checks if any train wants to enter a platform
        if(gate_in_open && !in_queue.empty()){
            int pos = findFreePlatform();
            if(pos != -1){
                gate_in_open = false;
                in_queue.at(0)->setPlatform_index(pos);
                emit TrainComing(in_queue.at(0));
                in_queue.erase(in_queue.begin());
            }
            else {
                qDebug() << "A train is on hold";
            }
        }
//        timer++;
//        qDebug() << timer;
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
    qDebug() << "Signal in from Train " << train->getId();
    in_queue.push_back(train);
}

void Station::onArrivedAtPlatform(Train* tr)
{
    qDebug() << tr->getId() << " has arrived at platform " << tr->getPlatform_index() + 1 ;
    int pos = tr->getPlatform_index();
    platforms[pos] = tr;
    gate_in_open = true;
}

void Station::onFreeExitLine(Train* train)
{
    qDebug() << "Next train can now leave the platform";
    delete train;
    gate_out_open = true;
}

void Station::onPlatformFree(Train* train)
{
    qDebug() << "Next train can now enter platform " << train->getPlatform_index() + 1;
    int pos = train->getPlatform_index();
    platforms[pos] = nullptr;
}
