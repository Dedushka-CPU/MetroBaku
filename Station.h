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
    static std::mutex cout_mtx; // еще один мютекс для вывода
    int wait_seconds=8; // сколько стоит на станции
    bool depo; // это депо?
    int square;//площадь станции(где могут стоять люди)
    int max_people;//сколько максимально могут быть на станции
    int current_people=0;//сколько сейчас на станции
   // int index;//коэф-т станции,должен влиять на кол-во людей которые приходят,сходят на стацнии
    bool last;//будет ли разворачиваться или нет
public:
    Station(int i, const std::string& n, int s, bool l,bool d);
    int get_cur_people();
    int get_max_pep() const;
    void updatePassengers();
    void p_g_t(int p);//people go to train
    void add_people(int);
    bool TryArriveTrain(int train_id, bool& s_t);//попытка приехать на станцию
    std::string getName();//получаем имя станции
    int getId();//гет нейм только с ид
    int getSquare();//гет ид только с площадью
    std::shared_ptr<Station> getPrev();//вернуть предыдущую локацию
    void setPrev(std::shared_ptr<Station> p);//сеттер предыдущий
    void addNext(std::shared_ptr<Station> n, MetroLine line);//сеттер следующей,только почему адд а не сет,но ладно не хочеться много кода менять.пусть будет адд
    std::shared_ptr<Station> getNextForLine(MetroLine line);//получаем след станцию по цвету
    const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& getNextStations() const;//а тут станцию
};



#endif // STATION_H
