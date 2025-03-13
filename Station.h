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
    int wait_seconds; // сколько стоит на станции
    bool depo; // это депо?

public:
    Station(int i, std::string n, int ws, bool d = false);

    bool TryArriveTrain(int train_id, bool& s_t);

    std::string getName();
    std::shared_ptr<Station> getPrev();
    void setPrev(std::shared_ptr<Station> p);

    void addNext(std::shared_ptr<Station> n, MetroLine line);
    std::shared_ptr<Station> getNextForLine(MetroLine line);
    const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& getNextStations() const;
};

#endif // STATION_H
