#ifndef TRAIN_H
#define TRAIN_H

#include "Station.h"
#include <memory>
#include <fstream>

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
    Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f = false);
    void runTrain(std::ofstream& outFile);
    std::string getLineName(MetroLine line);
    std::string getLineColor(MetroLine line);
    static std::mutex file_mtx;//мьютекс для вывода в файл,используем статический что бы для всех был один
    //и таким образом потоки будут ждать что бы предыдущий поток записал свое и потом запищут свое.Что бы не было каши
    //так же он в станции будет использоваться
};

#endif // TRAIN_H
