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
    void HalfSecondUpdate();
    void OneSecondUpdate();
};

#endif // TIMECLOCK_H
