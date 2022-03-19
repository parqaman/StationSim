#include "timeclock.h"
#include <QDebug>

TimeClock::TimeClock(QObject *parent)
    : QThread{parent}
{
}

void TimeClock::run()
{
    int counter = 0;
    while (true) {
        usleep(500000);
//        qDebug() << counter;
        emit HalfSecondUpdate();
        usleep(500000);
        emit HalfSecondUpdate();
        emit OneSecondUpdate();
        qDebug() << counter;
        counter++;
    }
}
