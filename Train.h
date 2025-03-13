#ifndef TRAIN_H
#define TRAIN_H

#include "Station.h"
#include <memory>

class Train {
private:
    int train_id;
    std::shared_ptr<Station> station_start;
    bool forward; // куда движется
    MetroLine line; // к какой ветке пренадлежит

public:
    Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f = true);

    void runTrain();
};

#endif // TRAIN_H
