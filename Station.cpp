#include "Station.h"
#include <thread>
#include <chrono>
#include <random>

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
void Station::updatePassengers(){
    int change = (rand() % 41) ; // Случайное число от -20 до +20
    int new_people = current_people + change;
            
    if (new_people < 0) new_people = 0;
    if (new_people > max_people) new_people = max_people;
            
    current_people = new_people;
    {
        std::lock_guard<std::mutex> cout_lock(cout_mtx);
        std::cout << "Station " << id << " now has " << current_people  << " people\n";
    }
}
void Station::add_people(int p){current_people+=p;}
int Station::get_cur_people(){ return current_people;}
void Station::p_g_t(int p){current_people+=p;}
int Station::get_max_pep() const{ return max_people;}
bool Station::TryArriveTrain(int train_id, bool& s_t) {
    if (mtx.try_lock()) {
        updatePassengers();
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id:" << train_id << " arrived at the station " << name << "(" << id << ")\n";
            std::cout << "*unreadable voice \"GATARA DUSHMAYA TALASIN.\"*\n";
        }   
        std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id:" << train_id << " left the station " << name << "(" << id << ")\n";
            std::cout<<"*shouting with aggression*\"KANARA CYAKIN\"\n\"AY ADAM XƏTTİN ARXASINDA DUR\"*\n";
        }
        if (depo) {
            {
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "*voice \"Train(" << train_id << ") go to depo. \n";
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
            s_t = true;
        }else if(last){
			{
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "*voice \"This is last station.PLease leave this train and don't your belonginks \n";
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds+5));
            s_t = true;
		}
        mtx.unlock();
        return true;
    } else {
        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Train with id: " << train_id << " waiting for the station " << name << "(" << id << ") to be released.\n voice 'Сколь нам еще ждать??Он че там вышел покурить??Ay shofer sur da'\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
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
