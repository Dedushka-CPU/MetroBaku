#include "Train.h"
#include <thread>
#include <chrono>
#include <random>

// Конструктор
Train::Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f) 
    : train_id(id), station_start(s_s), forward(f), line(l) {}

void Train::runTrain() {
    std::shared_ptr<Station> current_station = station_start;
    bool should_turn = false;
    
    while (arround_trips>0) {
        while (!current_station->TryArriveTrain(train_id, should_turn)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        /*дальше про людей на станции*/
        int passengers_out = cur_pas > 0 ? rand() % (cur_pas + 1) : 0;
        cur_pas -= passengers_out;
        current_station->add_people(passengers_out) ;
        std::cout << passengers_out << " passengers exited at station " 
                  << current_station->getId() << "\n";
        
        int available_space = max_pas - cur_pas; 
        int waiting_passengers = current_station->get_cur_people(); 
        int passengers_in = std::min(available_space, waiting_passengers); 
        if (passengers_in > 0) {
            passengers_in = rand() % (passengers_in + 1);
            cur_pas += passengers_in;
            current_station->p_g_t(passengers_in); // забираем людей со станции
            std::cout << passengers_in << " passengers boarded at station " 
                      << current_station->getId() << "\n";
        } else {
            std::cout << "No passengers boarded at station " 
                      << current_station->getId() << "\n";
        }
        if (should_turn) {
            forward = !forward;
            should_turn = false;
            arround_trips--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));//ну типа в туннеле едет,надеюсь в правильном месте ставлю это
        if (forward) {
            if (current_station->getId() == 11) {//28 мая
                if (line == MetroLine::RED || line == MetroLine::GREEN) {
                    current_station = current_station->getNextForLine(line);
                } else if (line == MetroLine::YELLOW) {
                    current_station = current_station->getNextForLine(MetroLine::YELLOW);
                }
            } else if (current_station->getId() == 13) {//нариман нариманов
                if (line == MetroLine::RED || line == MetroLine::GREEN) {
                    if (train_id % 2 == 0) {
                        for (const auto& next_station : current_station->getNextStations()) {
                            if (next_station.first->getId() == 14) {//улдуз
                                current_station = next_station.first;
                                break;
                            }
                        }
                    } else {
                        for (const auto& next_station : current_station->getNextStations()) {
                            if (next_station.first->getId() == 15) {//бакмиль
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
