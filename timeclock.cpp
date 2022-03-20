#include "timeclock.h"
#include <QDebug>

TimeClock::TimeClock(QObject *parent)
    : QThread{parent}
    , turbo(1)
{
}

void TimeClock::run()
{
    int counter = 0;
    while (true) {
        m1.lock();
        usleep(500000 / turbo);
        emit HalfSecondUpdate(counter);
        usleep(500000 / turbo);
        qDebug() << counter << " - timer clock";
        counter++;
        emit HalfSecondUpdate(counter);
        emit OneSecondUpdate();
        m1.unlock();
    }
}

void TimeClock::setTurbo(unsigned int newTurbo)
{
    m1.lock();
    turbo = newTurbo;
    m1.unlock();
}
