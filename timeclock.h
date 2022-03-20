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

    void setPause(bool newPause);

    bool getPause() const;

private:
    unsigned int turbo;
    bool pause;
    QMutex m1;

signals:
    void HalfSecondUpdate();
    void OneSecondUpdate();
};

#endif // TIMECLOCK_H
