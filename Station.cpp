#include "Station.h"
#include <thread>
#include <chrono>

// Конструктор
Station::Station(int i, std::string n, int ws, bool d) 
    : id(i), name(n), wait_seconds(ws), depo(d), prev(nullptr) {}

bool Station::TryArriveTrain(int train_id, bool& s_t) {
    if (mtx.try_lock()) {
        std::cout << "Train with id:" << train_id << " arrived at the station " << name << "(" << id << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
        std::cout << "Train with id:" << train_id << " left the station " << name << "(" << id << ")\n";
        if (depo) {
            std::cout << "Train with id:" << train_id << " in the depot and turns around. \n";
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

std::string Station::getName() {
    return name;
}

std::shared_ptr<Station> Station::getPrev() { 
    return prev; 
}

void Station::setPrev(std::shared_ptr<Station> p) { 
    prev = p; 
}

void Station::addNext(std::shared_ptr<Station> n, MetroLine line) { 
    next.push_back({n, line}); 
}

std::shared_ptr<Station> Station::getNextForLine(MetroLine line) {
    for (const auto& [station, station_line] : next) {
        if (station_line == line) {
            return station;
        }
    }
    return nullptr; 
}

const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& Station::getNextStations() const {
    return next;
}
