#include "Station.h"
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include "Train.h" //для мьютекса от треина


// Конструктор
Station::Station(int i, const std::string& n, int s, bool l,bool d){
    id=i;
    name=n;
    depo=d;
    square=s;
    last=l;
    max_people = square / 0.2; // Рассчитываем макс. число людей на платформе
}
void Station::updatePassengers(std::ofstream& outFile, std::string line){
    int change = (rand() % 41) ; 
    int new_people = current_people + change;
    std::stringstream ss;
    if (new_people < 0) new_people = 0;
    if (new_people > max_people){ 
        new_people = max_people;
        int i=rand()%10;
        if (i < 10) { // Исправлено 1<10
            ss << "> 🚨 **Инцидент**: О НЕТ! Человек упал под поезд на станции <span style=\"color:" 
               << line << "\">" << name << "</span> 😢\n";
        }
    }
            
    current_people = new_people;
    ss << "- 📈 **Пассажиры:** на станции <span style=\"color:" << line 
       << "\">" << name << " (" << id << ")</span> теперь " << current_people << " человек 👥\n";

    std::lock_guard<std::mutex> lock(Train::file_mtx);
    outFile << ss.str();
}
void Station::add_people(int p){current_people+=p;}
int Station::get_cur_people(){ return current_people;}
void Station::p_g_t(int p){current_people+=p;}
int Station::get_max_pep() const{ return max_people;}
bool Station::TryArriveTrain(int train_id, bool& s_t, bool& forward, std::string line_color, std::ofstream& outFile) {
    if (forward) {
        if (mtx_t.try_lock()) {
            std::stringstream ss;
            ss << "### 🚉 <span style=\"color:" << line_color << "\">" 
               << name << " (" << id << ")</span> - Прибытие (Left_side)\n";
            {
                std::lock_guard<std::mutex> lock(Train::file_mtx);
                outFile << ss.str();
            }
            updatePassengers(outFile, line_color);

            ss.str(""); // Очищаем поток
            ss << "- 🚂 **Поезд #" << train_id << " прибыл** 🎉\n";
            ss << "- 📢 **Громкий голос:** *неразборчиво* \"GATARA DUSHMAYA TALASIN\" 🔊\n";
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));

            if (depo) {
                ss << "- 📢 **Громкий голос:** *голос* \"Поезд #" << train_id << " следует в депо\" 🏭\n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
                s_t = true;
            } else if (last) {
                ss << "- 📢 **Громкий голос:** *голос* \"Это последняя станция. Пожалуйста, покиньте поезд и не забудьте свои вещи\" 🚪\n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds + 5));
                s_t = true;
            }
            ss << "- 🚂 **Поезд #" << train_id << " покинул станцию** 👋\n";
            ss << "- 😡 **Крики:** *агрессивно* \"KANARA CYAKIN\" \"AY ADAM XƏTTİN ARXASINDA DUR\" 🗣️\n\n";

            {
                std::lock_guard<std::mutex> lock(Train::file_mtx);
                outFile << ss.str();
            }
            mtx_t.unlock();
            return true;
        } else {
            std::stringstream ss;
            ss << "#### ⏳ Ожидание у станции <span style=\"color:" << line_color << "\">" 
               << name << " (" << id << ")</span>\n";
            ss << "- 🚂 **Поезд #" << train_id << " ждет освобождения** ⏰\n";
            ss << "- 🗣️ **Голоса:** \"Сколько нам еще ждать?? Он что, вышел покурить?? Ay şofer sür da\" 😤\n\n";
            std::lock_guard<std::mutex> lock(Train::file_mtx);
            outFile << ss.str();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return false;
        }
    } else {
        if (mtx_f.try_lock()) {
            std::stringstream ss;
            ss << "### 🚉 <span style=\"color:" << line_color << "\">" 
               << name << " (" << id << ")</span> - Прибытие (Right_side)\n";
            {
                std::lock_guard<std::mutex> lock(Train::file_mtx);
                outFile << ss.str();
            }
            updatePassengers(outFile, line_color);

            ss.str("");
            ss << "- 🚂 **Поезд #" << train_id << " прибыл** 🎉\n";
            ss << "- 📢 **Громкий голос:** *неразборчиво* \"GATARA DUSHMAYA TALASIN\" 🔊\n";
            std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));

            if (depo) {
                ss << "- 📢 **Громкий голос:** *голос* \"Поезд #" << train_id << " следует в депо\" 🏭\n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
                s_t = true;
            } else if (last) {
                ss << "- 📢 **Громкий голос:** *голос* \"Это последняя станция. Пожалуйста, покиньте поезд и не забудьте свои вещи\" 🚪\n";
                ss << "- 🚶 **Машинист неспешным шагом пошел в другую часть поезда** 👨‍✈️\n";
                std::this_thread::sleep_for(std::chrono::seconds(wait_seconds + 5));
                s_t = true;
            }
            ss << "- 🚂 **Поезд #" << train_id << " покинул станцию** 👋\n";
            ss << "- 😡 **Крики:** *агрессивно* \"KANARA CYAKIN\" \"AY ADAM XƏTTİN ARXASINDA DUR\" 🗣️\n\n";

            {
                std::lock_guard<std::mutex> lock(Train::file_mtx);
                outFile << ss.str();
            }
            mtx_f.unlock();
            return true;
        } else {
            std::stringstream ss;
            ss << "#### ⏳ Ожидание у станции <span style=\"color:" << line_color << "\">" 
               << name << " (" << id << ")</span>\n";
            ss << "- 🚂 **Поезд #" << train_id << " ждет освобождения** ⏰\n";
            ss << "- 🗣️ **Голоса:** \"Сколько нам еще ждать?? Он что, вышел покурить?? Ay şofer sür da\" 😤\n\n";
            std::lock_guard<std::mutex> lock(Train::file_mtx);
            outFile << ss.str();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return false;
        }
    }
}
std::string Station::getName() {
    return name;
}

std::shared_ptr<Station> Station::getPrevById(int targetId) {
    for (const auto& [station, line] : prev) { 
        if (station && station->id == targetId) {
            return station;
        }
    }
    return nullptr;
}

std::vector<std::pair<std::shared_ptr<Station>, MetroLine>> Station::getPrev() { 
    return prev; 
}

void Station::setPrev(std::shared_ptr<Station> p, MetroLine line) { 
    prev.push_back({p, line});
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

std::shared_ptr<Station> Station::getPrevForLine(MetroLine line) {
    for (const auto& [station, station_line] : prev) {
        if (station_line == line) {
            return station;
        }
    }
    return nullptr; 
}

const std::vector<std::pair<std::shared_ptr<Station>, MetroLine>>& Station::getNextStations() const {
    return next;
}
