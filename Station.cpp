#include "Station.h"
#include <thread>
#include <chrono>

std::mutex Station::cout_mtx;//компилятор сильна ругался,пришлось сюда добавить

// Конструктор
Station::Station(int i, const std::string& n, int s, bool l,bool d){
    id=i;
    name=n;
    prev=nullptr;
    depo=d;
    square=s;
    last=l;
    max_people = square / 0.2; // Рассчитываем макс. число людей на платформе
}


bool Station::TryArriveTrain(int train_id, bool& s_t) {
    if (mtx.try_lock()) {
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id:" << train_id << " arrived at the station " << name << "(" << id << ")\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id:" << train_id << " left the station " << name << "(" << id << ")\n";
        }
        if (depo) {
            {
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "Train with id:" << train_id << " in the depot and turns around. \n";
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
            s_t = true;
        }
        mtx.unlock();
        return true;
    } else {
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id: " << train_id << " waiting for the station " << name << "(" << id << ") to be released.\n";
        }
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

int Station::getId(){
    return id;
}
int Station::getSquare(){
    return square;
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
