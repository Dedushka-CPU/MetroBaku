#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>

class Station {
private:
    int id;          // ID станции
    std::string name; // Название станции
    std::shared_ptr<Station> next;        // ID следующей станции
    std::shared_ptr<Station> prev;        // ID предыдущей станции
    std::mutex mtx; //мютекс для блокировки
    int wait_seconds;//Время ожидания на станции
    bool depo;
public:
    Station(int i, std::string n,int ws,bool d=false):id(i),name(n),wait_seconds(ws), next(nullptr),prev(nullptr),depo(d){};
    //~Station();
    bool TryArriveTrain(int train_id,bool& s_t) {
        if (mtx.try_lock()) {
            std::cout<<"Поезд с ид:"<<train_id<<" прибыл на станция  "<<name<<"("<<id<<")\n";
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
            std::cout<<"Поезд с ид:"<<train_id<<" покинул станция "<<name<<"("<<id<<")\n";
            if (depo==true) {
                std::cout<<"Поезд с ид:"<<train_id<<" в депо и разворачивается.\n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
                s_t=true;
            }
            mtx.unlock();
            return true;
        }else {
            std::cout<<"Поезд с ид "<<train_id<<" ждет пока станция  "<<name<<"("<<id<<") освободится.\n";
            return false;
        }
    }
    std::shared_ptr<Station> getNext(){ return next;}
    std::shared_ptr<Station> getPrev(){ return prev;}
    void setNext(std::shared_ptr<Station> n){next=n;};
    void setPrev(std::shared_ptr<Station> p){prev=p;};
};
class Train{
private:
    int train_id;
    std::shared_ptr<Station> station_start;
    bool forward;//true если вперед поедет(например выехал с депо),false если обратно(возвращается в депо в которое выехал)
public:
    Train(int id,std::shared_ptr<Station> s_s,bool f=true):train_id(id),station_start(s_s),forward(f){}
    //~Train();
    void runTrain() {
        std::shared_ptr<Station> current_station=station_start;
        bool should_turn=false;
        while (current_station) {

            while (!current_station->TryArriveTrain(train_id,should_turn)) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            if (should_turn == true) {
                forward = !forward;
               // std::cout << "Направление поменялось на " << forward << "\n";
                should_turn = false;
            }

            if (forward) {
                current_station = current_station->getNext();
            } else {
                current_station = current_station->getPrev();
            }

        }
    }

};
