#include "timeclock.h"
#include <QDebug>

TimeClock::TimeClock(QObject *parent)
    : QThread{parent}
    , turbo(1)
    , pause(false)
{
}

void TimeClock::run()
{
    while (true) {
        m1.lock();
        if(!pause){
            emit HalfSecondUpdate();
            usleep(500000 / turbo);
            emit HalfSecondUpdate();
            emit OneSecondUpdate();
            usleep(500000 / turbo);
        }
        m1.unlock();
    }
}

void TimeClock::setTurbo(unsigned int newTurbo)
{
    m1.lock();
    turbo = newTurbo;
    m1.unlock();
}

void TimeClock::setPause(bool newPause)
{
    pause = newPause;
}

bool TimeClock::getPause() const
{
    return pause;
}
