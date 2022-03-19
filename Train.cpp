#include "Train.h"

Train::Train(int _duration)
    :
      id(++id_counter),
      stop_duration(_duration)
    , listed(false)
{
}

int Train::getStop_duration() const
{
    return stop_duration;
}

int Train::getId() const
{
    return id;
}

void Train::reduce_stop_duration()
{
    stop_duration -= 1;
}

int Train::getPlatform_index() const
{
    return platform_index;
}

void Train::setPlatform_index(int newPlatform_index)
{
    platform_index = newPlatform_index;
}

bool Train::getListed() const
{
    return listed;
}

void Train::setListed(bool newListed)
{
    listed = newListed;
}
