#include "station.h"

Station::Station(QLabel** _train_labels, QObject *parent)
    : QThread{parent}
    , gate_in_open{true}
    , gate_out_open{true}
    , exit_line_free{true}
    , train_labels(_train_labels)
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        platforms[i] = nullptr;
    }
}

void Station::run()
{
    while (true) {
        sleep(1);

        // reduce stop duration of trains in platform
        m1.lock();
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
        m1.unlock();

        // notify that a train wants to leave its platform
        m1.lock();
        if(exit_line_free && !out_queue.empty()){
            gate_out_open = false;
            exit_line_free = false;
            Train* out_train = out_queue.at(0);
            qDebug() << "Train " << out_train->getId() << " is leaving platform " << out_train->getPlatform_index() + 1;
            emit TrainLeaving(out_train);
            out_queue.erase(out_queue.begin());
        }
        m1.unlock();

        // checks if any train wants to enter a platform
        m1.lock();
        if(gate_in_open && !in_queue.empty()){
            int pos = findFreePlatform();
            if(pos != -1){
                gate_in_open = false;
                in_queue.at(0)->setPlatform_index(pos);
                train_labels[in_queue.at(0)->getPlatform_index()]->setText(QString::fromStdString("Train " + std::to_string(in_queue.at(0)->getId())));
                train_labels[in_queue.at(0)->getPlatform_index()]->show();
                emit TrainComing(in_queue.at(0), train_labels[in_queue.at(0)->getPlatform_index()]);
                in_queue.erase(in_queue.begin());
            }
            else {
                qDebug() << "A train is on hold";
            }
        }
        m1.unlock();
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
    m1.lock();
    qDebug() << "Signal in from Train " << train->getId();
    in_queue.push_back(train);
    m1.unlock();
}

void Station::onArrivedAtPlatform(Train* tr)
{
    m1.lock();
    qDebug() << tr->getId() << " has arrived at platform " << tr->getPlatform_index() + 1 ;
    int pos = tr->getPlatform_index();
    platforms[pos] = tr;
    gate_in_open = true;
    m1.unlock();
}

void Station::onFreeExitLine(Train* train)
{
    m1.lock();
    qDebug() << "Next train can now leave the platform";
    delete train;
    exit_line_free = true;
    m1.unlock();
}

void Station::onPlatformFree(Train* train)
{
    m1.lock();
    qDebug() << "Next train can now enter platform " << train->getPlatform_index() + 1;
    int pos = train->getPlatform_index();
    platforms[pos] = nullptr;
    gate_out_open = true;
    m1.unlock();
}
