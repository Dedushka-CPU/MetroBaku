#include "Train.h"
#include <thread>
#include <chrono>

// Конструктор
Train::Train(int id, std::shared_ptr<Station> s_s, MetroLine l, bool f) 
    : train_id(id), station_start(s_s), forward(f), line(l) {}

void Train::runTrain() {
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
