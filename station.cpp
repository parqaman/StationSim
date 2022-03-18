#include "station.h"

Station::Station(QObject *parent)
    : QThread{parent}
    , gate_in_open{true}
    , gate_out_open{true}
    , exit_line_free{true}
{
    for(int i = 0; i < NUM_OF_PLATFORMS; i++){
        platforms[i] = nullptr;
    }
    QLabel* q = new QLabel;
    q->setGeometry(1410, 480, 261, 191);
    q->setProperty("text", "text label");
    q->setStyleSheet("font: 18pt");

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
                emit TrainComing(in_queue.at(0));
                in_queue.erase(in_queue.begin());
            }
            else {
                qDebug() << "A train is on hold";
            }
        }
        m1.unlock();
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
