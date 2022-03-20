#ifndef TRAIN_H
#define TRAIN_H

#include <unistd.h>

static int id_counter = 0;

class Train
{
public:
    Train(int _duration);

    int getStop_duration() const;

    int getId() const;

    void reduce_stop_duration();

    int getPlatform_index() const;
    void setPlatform_index(int newPlatform_index);

    bool getListed() const;
    void setListed(bool newListed);

private:
    int id;
    int stop_duration;
    int platform_index;
    bool listed;
};

#endif // TRAIN_H
