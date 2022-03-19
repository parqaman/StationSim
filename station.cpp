#include "station.h"

Station::Station(QLabel** _train_labels, QObject *parent)
    : QThread{parent}
    , second_counter{0}
    , gate_in_open{true}
//    , gate_out_open{true}
    , exit_line_free{true}
    , train_labels(_train_labels)
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        platforms[i] = nullptr;
    }
}

void Station::run()
{
    int timer = 0;
    while (true) {
        qDebug() << timer;
        sleep(1);
        timer++;

        // reduce stop duration of trains in platform
        m1.lock();
        for(int i = 0; i < NUM_OF_PLATFORMS; i++){
            if(platforms[i] != nullptr){
                if(platforms[i]->getStop_duration() > 0){
                    platforms[i]->reduce_stop_duration();
                }
                else {
                    if(exit_line_free){
                        out_queue.push_back(platforms[i]);
                    }
                }
            }
        }
        m1.unlock();

        // notify that a train wants to leave its platform
        m1.lock();
        if(exit_line_free && !out_queue.empty()){
//            gate_out_open = false;
            exit_line_free = false;
            Train* out_train = out_queue.at(0);
            qDebug() << "Train " << out_train->getId() << " is leaving platform " << out_train->getPlatform_index() + 1;
            emit TrainLeaving(out_train, train_labels[out_train->getPlatform_index()]);
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
                qDebug() << "Train " << in_queue.at(0)->getId() << " is entering platform " << pos + 1;
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
    qDebug() << tr->getId() << " arrived at platform " << tr->getPlatform_index() + 1 ;
    int pos = tr->getPlatform_index();
    platforms[pos] = tr;
    gate_in_open = true;
    m1.unlock();
}

void Station::onFreeExitLine(Train* train)
{
    m1.lock();
    qDebug() << "Exit line is now free";
    delete train;
    exit_line_free = true;
    m1.unlock();
}

void Station::onPlatformFree(Train* train)
{
//    m1.lock();
    platforms[train->getPlatform_index()] = nullptr;
    qDebug() << train->getId() << " left platform " << train->getPlatform_index() + 1;
    train_labels[train->getPlatform_index()]->hide();
    train_labels[train->getPlatform_index()]->setGeometry(2209, 489, 81, 30);
//    gate_out_open = true;
//    m1.unlock();
}

void Station::onSecondUpdate()
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        if(platforms[i] != nullptr){
            if(platforms[i]->getStop_duration() > 0){
                platforms[i]->reduce_stop_duration();
            }
            else {
                if(exit_line_free){
                    out_queue.push_back(platforms[i]);
                }
            }
        }
    }

    // notify that a train wants to leave its platform
    if(exit_line_free && !out_queue.empty()){
//            gate_out_open = false;
        exit_line_free = false;
        Train* out_train = out_queue.at(0);
        qDebug() << "Train " << out_train->getId() << " is leaving platform " << out_train->getPlatform_index() + 1;
        emit TrainLeaving(out_train, train_labels[out_train->getPlatform_index()]);
        out_queue.erase(out_queue.begin());
    }

    // checks if any train wants to enter a platform
    if(gate_in_open && !in_queue.empty()){
        int pos = findFreePlatform();
        if(pos != -1){
            gate_in_open = false;
            in_queue.at(0)->setPlatform_index(pos);
            qDebug() << "Train " << in_queue.at(0)->getId() << " is entering platform " << pos + 1;
            train_labels[in_queue.at(0)->getPlatform_index()]->setText(QString::fromStdString("Train " + std::to_string(in_queue.at(0)->getId())));
            train_labels[in_queue.at(0)->getPlatform_index()]->show();
            emit TrainComing(in_queue.at(0), train_labels[in_queue.at(0)->getPlatform_index()]);
            in_queue.erase(in_queue.begin());
        }
        else {
            qDebug() << "A train is on hold";
        }
    }
}
