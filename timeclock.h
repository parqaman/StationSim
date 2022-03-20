#ifndef TIMECLOCK_H
#define TIMECLOCK_H

#include <QThread>
#include <QObject>
#include <QMutex>

class TimeClock : public QThread
{
    Q_OBJECT
public:
    explicit TimeClock(QObject *parent = nullptr);
    void run();

    void setTurbo(unsigned int newTurbo);

private:
    unsigned int turbo;
    QMutex m1;

signals:
    void HalfSecondUpdate(int);
    void OneSecondUpdate();
};

#endif // TIMECLOCK_H
