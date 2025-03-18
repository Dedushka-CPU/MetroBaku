#include "Train.h"
#include <thread>
#include <chrono>
#include <random>
#include <sstream>

 std::mutex Train::file_mtx;//компилятор снова ругался просил сюда тоже добавить

// Конструктор
Train::Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f) 
    : train_id(id), station_start(s_s), forward(f), line(l) {}

std::string Train::getLineName(MetroLine line) {
    switch (line) {
        case MetroLine::GREEN: return "Green";
        case MetroLine::RED: return "Red";
        case MetroLine::PURPLE: return "Purple";
        case MetroLine::YELLOW: return "Yellow";
        default: return "Unknown";
    }
}

std::string Train::getLineColor(MetroLine line) {
    switch (line) {
        case MetroLine::GREEN: return "#00FF00"; // Зеленый
        case MetroLine::RED: return "#FF0000";   // Красный
        case MetroLine::PURPLE: return "#800080"; // Фиолетовый
        case MetroLine::YELLOW: return "#FFFF00"; // Желтый
        default: return "#000000"; // Черный по умолчанию
    }
}
void Train::runTrain(std::ofstream& outFile) {
    std::shared_ptr<Station> current_station = station_start;
    bool should_turn = false;
    
    std::string line_color = getLineColor(line);
    {
        std::stringstream ss;
        ss << "🚂 <h1>Поезд #" << train_id << " - Рейс начат</h1>\n";
        ss << "| Параметр          | Значение                                      |\n";
        ss << "|-------------------|-----------------------------------------------|\n";
        ss << "| 📌 Линия         | " << getLineName(line) << "                   |\n";
        ss << "| 🏁 Начало        | <span style=\"color:" << line_color << "\">" 
           << station_start->getName() << " (" << station_start->getId() << ")</span> |\n";
        ss << "| ⏳ Время старта  | " << std::chrono::system_clock::now().time_since_epoch().count() << " |\n\n";
        std::lock_guard<std::mutex> lock(file_mtx);
        outFile << ss.str();
    }

    while (arround_trips>0) {
        while (!current_station->TryArriveTrain(train_id, should_turn,forward,line_color,outFile)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        /*дальше про людей на станции*/
        {
            std::stringstream ss;
            ss << "---\n";
            ss << "### 🚉 Действия на <span style=\"color:" << line_color << "\">" 
               << current_station->getName() << " (" << current_station->getId() << ")</span>\n";
            ss << "| Действие          | Результат         |\n";
            ss << "|-------------------|-------------------|\n";

            int passengers_out = cur_pas > 0 ? rand() % (cur_pas + 1) : 0;
            cur_pas -= passengers_out;
            current_station->add_people(passengers_out);
            ss << "| 👋 Высадка       | " << passengers_out << " пассажиров 🧳 |\n";

            int available_space = max_pas - cur_pas;
            int waiting_passengers = current_station->get_cur_people();
            int passengers_in = std::min(available_space, waiting_passengers);
            if (passengers_in > 0) {
                passengers_in = rand() % (passengers_in + 1);
                cur_pas += passengers_in;
                current_station->p_g_t(-passengers_in);
                ss << "| 🚶 Посадка       | " << passengers_in << " пассажиров 🎫 |\n";
            } else {
                ss << "| 🚫 Посадка       | Нет пассажиров    |\n";
            }
            ss << "| 📊 Загрузка      | " << cur_pas << " / " << max_pas << " 👥 |\n";
        if (should_turn) {
            forward = !forward;
            should_turn = false;
            arround_trips--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));//ну типа в туннеле едет,надеюсь в правильном месте ставлю это
        if (forward) {
            current_station=current_station->getNextForLine(line);
        } 
        else {
            if(current_station->getId()==13){//Нариманов
                int i=rand()%10;
                if(i<=2){
                    current_station=current_station->getPrevById(15);
                    std::lock_guard<std::mutex> lock(file_mtx);
                    ss << "> ⚠️ *Qatar bakmil stansiyasi qedir.Свет в вагоне начал тускнеть...* 💡\n";
                }else{
                    current_station=current_station->getPrevById(14);  
                }
                
            }else{
             current_station = current_station->getPrevForLine(line);
            }
            
        }
        {
            ss << "| ➡️ Далее         | " << (current_station ? 
                "<span style=\"color:" + line_color + "\">" + current_station->getName() + " (" + std::to_string(current_station->getId()) + ")</span>" : 
                "нет 🚧") << " |\n\n";

        std::lock_guard<std::mutex> lock(file_mtx);
        outFile << ss.str();
        }
    }
}
}


