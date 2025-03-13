#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <map>

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
    Station(int i, std::string n, int ws, bool d = false) 
        : id(i), name(n), wait_seconds(ws), depo(d), prev(nullptr) {}

    bool TryArriveTrain(int train_id, bool& s_t) {
        if (mtx.try_lock()) {
            std::cout << "Train with id:" << train_id << " arrived at the station " << name << "(" << id << ")\n";
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
            std::cout << "Train with id:" << train_id << " дeft the station " << name << "(" << id << ")\n";
            if (depo) {
                std::cout << "Train with id::" << train_id << " in the depot and turns around. \n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
                s_t = true;
            }
            mtx.unlock();
            return true;
        } else {
            std::cout << "Train with id: " << train_id << " waiting for the station " << name << "(" << id << ") to be released.\n";
            return false;
        }
    }
    std::string getName(){
        return name;
    }
    std::shared_ptr<Station> getPrev() { 
        return prev; 
    }

    void setPrev(std::shared_ptr<Station> p) { 
        prev = p; 
    }

    void addNext(std::shared_ptr<Station> n, MetroLine line) { 
        next.push_back({n, line}); 
    }

    std::shared_ptr<Station> getNextForLine(MetroLine line) {
        for (const auto& [station, station_line] : next) {
            if (station_line == line) {
                return station;
            }
        }
        return nullptr; 
    }
    const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& getNextStations() const {
        return next;
    }
};

class Train {
private:
    int train_id;
    std::shared_ptr<Station> station_start;
    bool forward; // куда движется
    MetroLine line; // к какой ветке пренадлежит

public:
    Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f = true) 
        : train_id(id), station_start(s_s), forward(f), line(l) {}

    void runTrain() {
        std::shared_ptr<Station> current_station = station_start;
        bool should_turn = false;
        
        while (current_station) {
            while (!current_station->TryArriveTrain(train_id, should_turn)) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            
            if (should_turn) {
                forward = !forward;
                should_turn = false;
            }

            if (forward) {
                if (current_station->getName() == "28 Мая") {
                    if (line == MetroLine::RED || line == MetroLine::GREEN) {
                        current_station = current_station->getNextForLine(line);
                    } else if (line == MetroLine::YELLOW) {
                        current_station = current_station->getNextForLine(MetroLine::YELLOW);
                    }
                } else if (current_station->getName() == "Нариман Нариманов") {
                    if (line == MetroLine::RED || line == MetroLine::GREEN) {
                        if (train_id % 2 == 0) {
                            for (const auto& next_station : current_station->getNextStations()) {
                                if (next_station.first->getName() == "Кёроглу") {
                                    current_station = next_station.first;
                                    break;
                                }
                            }
                        } else {
                            for (const auto& next_station : current_station->getNextStations()) {
                                if (next_station.first->getName() == "Бакмил") {
                                    current_station = next_station.first;
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    current_station = current_station->getNextForLine(line);
                }
            } else {
                current_station = current_station->getPrev();
            }
        }
    }
};