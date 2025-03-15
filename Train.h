#ifndef TRAIN_H
#define TRAIN_H

#include "Station.h"
#include <memory>

class Train {
private:
    int train_id;
    std::shared_ptr<Station> station_start;// место откуда поест начинает свое незабываемое путешествие
    bool forward; // куда движется
    MetroLine line; // к какой ветке пренадлежит
    int arround_trips=40;//наверное столько раз поезд проезжает полный путь за день
    int cur_pas=0;//текущее кол-во пассажиров 
    int max_pas=2600;//максимальное кол-во пассажиров
    
public:
    Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f = true);

    void runTrain();
};

#endif // TRAIN_H
