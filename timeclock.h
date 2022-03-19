#ifndef TIMECLOCK_H
#define TIMECLOCK_H

#include <QThread>
#include <QObject>

class TimeClock : public QThread
{
    Q_OBJECT
public:
    explicit TimeClock(QObject *parent = nullptr);
    void run();

signals:
    void HalfSecondUpdate(int);
    void OneSecondUpdate();
};

#endif // TIMECLOCK_H
