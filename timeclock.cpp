#include "timeclock.h"
#include <QDebug>

TimeClock::TimeClock(QObject *parent)
    : QThread{parent}
{
}

void TimeClock::run()
{
    int counter = 0;
    const int turbo = 100;
    while (true) {
        usleep(500000 / turbo);
        emit HalfSecondUpdate(counter);
        usleep(500000 / turbo);
        qDebug() << counter << " - timer clock";
        counter++;
        emit HalfSecondUpdate(counter);
        emit OneSecondUpdate();
    }
}
