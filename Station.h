#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <vector>

// ну тут короче цвета
enum class MetroLine {
    RED,
    GREEN,
    PURPLE,
    YELLOW
};

class Station {
private:
    int id;          // ID станции
    std::string name; // Имя станции
    std::vector<std::pair<std::shared_ptr<Station>, MetroLine>> next; // след станция и ее цвет
    std::shared_ptr<Station> prev;        // предыдущая станция
    std::mutex mtx; // просто мюьтекс
    int wait_seconds=3; // сколько стоит на станции
    bool depo; // это депо?
    int square;//площадь станции(где могут стоять люди)
    int max_people;//сколько максимально могут быть на станции
    int current_people=0;//сколько сейчас на станции
   // int index;//коэф-т станции,должен влиять на кол-во людей которые приходят,сходят на стацнии
    bool last;//будет ли разворачиваться или нет
public:
    Station(int i, const std::string& n, int s, bool l,bool d);

    bool TryArriveTrain(int train_id, bool& s_t);

    std::string getName();
    int getId();
    std::shared_ptr<Station> getPrev();
    void setPrev(std::shared_ptr<Station> p);

    void addNext(std::shared_ptr<Station> n, MetroLine line);
    std::shared_ptr<Station> getNextForLine(MetroLine line);
    const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& getNextStations() const;
};



#endif // STATION_H
