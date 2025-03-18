#include "Station.h"
#include "Train.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include "json.hpp"
#include <map>
#include <fstream>

using json = nlohmann::json;

void RunMetro(Train& train,std::ofstream& file) {
    train.runTrain(file);
}
MetroLine stringToMetroLine(const std::string& lineStr) {
    static const std::map<std::string, MetroLine> lineMap = {
        {"RED", MetroLine::RED},
        {"GREEN", MetroLine::GREEN},
        {"PURPLE", MetroLine::PURPLE},
        {"YELLOW", MetroLine::YELLOW}
    };

    auto it = lineMap.find(lineStr);
    if (it != lineMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown metro line: " + lineStr);
}

std::vector<std::shared_ptr<Station>> loadStationsFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    json j;
    file >> j;

    // Создаем станции без связей
    std::vector<std::shared_ptr<Station>> stations;
    std::map<int, std::shared_ptr<Station>> stationMap; // Для быстрого доступа по ID

    for (const auto& stationJson : j) {
        int id = stationJson["id"];
        std::string name = stationJson["name"];
        int square = stationJson["square"];
        bool last = stationJson["last"];
        bool depo = stationJson["depo"];

        auto station = std::make_shared<Station>(id, name, square, last, depo);
        stations.push_back(station);
        stationMap[id] = station;
    }

    // Устанавливаем связи
    for (size_t i = 0; i < j.size(); ++i) {
        auto& stationJson = j[i];
        auto station = stations[i];

        // Устанавливаем prev
        if (!stationJson["prev"].is_null()) {
            for (const auto& prevJson : stationJson["prev"]) {
                int prevId = prevJson["id"];
                std::string lineStr = prevJson["line"];
                MetroLine line = stringToMetroLine(lineStr);
                station->setPrev(stationMap[prevId], line);
            }
        }

        // Устанавливаем next
        if (!stationJson["next"].is_null()) {
            for (const auto& nextJson : stationJson["next"]) {
                int nextId = nextJson["id"];
                std::string lineStr = nextJson["line"];
                MetroLine line = stringToMetroLine(lineStr);
                station->addNext(stationMap[nextId], line);
            }
        }
    }

    return stations;
}
int main() {
 
    //try {
        auto stations = loadStationsFromJson("stations.json");
        std::cout << "Loaded " << stations.size() << " stations from JSON.\n";

        // Пример вывода для проверки
        for (const auto& station : stations) {
            std::cout << "Station: " << station->getName() << " (ID: " << station->getId() << ") with square="<<station->getSquare()<<"\n";
        }
   /*} catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }*/
 
    //Train train_purple(3, stations[0], MetroLine::PURPLE);
   // Train train_yellow(4, stations[21], MetroLine::YELLOW);
/*
    std::thread red_thread(&Train::runTrain, &train_red);
    std::thread green_thread(&Train::runTrain, &train_green);*/
  // std::thread purple_thread(&Train::runTrain, &train_purple);
  //  std::thread yellow_thread(&Train::runTrain, &train_yellow);
   // green_thread.join();
   // purple_thread.join();
   // yellow_thread.join();
    /*std::vector<Train> trains;
    std::vector<std::thread> threads;
    for (int i = 0; i < 50; ++i) {
        MetroLine line = (i % 2 == 0) ? MetroLine::GREEN : MetroLine::RED;
        trains.push_back(Train(i, stations[15], line));
    }
    for (auto& train : trains) {
        threads.push_back(std::thread(&Train::runTrain, &train));
    }
    for (auto& thread : threads) {
        thread.join();
    }*/
    /*Train train_purple(3, stations[0], MetroLine::PURPLE,false);
    std::thread purple_thread(&Train::runTrain, &train_purple);
    Train train_purple1(31, stations[4], MetroLine::PURPLE,true);
    std::thread purple_thread1(&Train::runTrain, &train_purple1);
    purple_thread.join();
    purple_thread1.join();*/
    std::ofstream outFile("trips.md", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Не удалось открыть trips.md\n";
        return 1;
    }
    std::vector<Train> trains;
    int train_count;
    std::cout << "Введите количество поездов в метро: ";
    std::cin >> train_count;
    int id;
    int station_id;

    for (int i = 0; i < train_count; i++) {
        std::cout << "\nПоезд #" << (i + 1) << ":\n";
        
        std::cout << "Введите номер поезда: ";
        std::cin >> id;

        std::cout << "Выберите начальную станцию:\n";
        std::cout << "0 - Ходжасан (Purple)\n";
        std::cout << "14 - Бакмиль (Red/Green)\n";
        std::cout << "22 - Шах Исмаил Хатаи (Yellow)\n";
        std::cout << "Введите номер станции: ";
        std::cin >> station_id;

        // Проверка корректности станции
        if (station_id != 0 && station_id != 14 && station_id != 22) {
            std::cout << "Неверный номер станции! Доступны только 0, 14 или 22. Попробуйте снова.\n";
            i--;
            continue;
        }

        MetroLine line;
        int color_choice;
        bool valid_input = true;

        // Определение цвета в зависимости от станции
        switch (station_id) {
            case 0: // Ходжасан - только Purple
                line = MetroLine::PURPLE;
                std::cout << "Выбрана станция Ходжасан - линия Purple\n";
                break;
                
            case 22: // Шах Исмаил Хатаи - только Yellow
                line = MetroLine::YELLOW;
                std::cout << "Выбрана станция Шах Исмаил Хатаи - линия Yellow\n";
                break;
                
            case 14: // Бакмиль - выбор Red/Green
                std::cout << "Выберите цвет линии для Бакмиль:\n";
                std::cout << "0 - Green\n1 - Red\n";
                std::cout << "Введите номер цвета: ";
                std::cin >> color_choice;
                if (color_choice == 0) {
                    line = MetroLine::GREEN;
                } else if (color_choice == 1) {
                    line = MetroLine::RED;
                } else {
                    std::cout << "Неверный выбор цвета! Доступны только 0 или 1. Попробуйте снова.\n";
                    i--;
                    valid_input = false;
                }
                break;
        }

        if (valid_input) {
            std::cout<<"Train with id:"<<id<<" added to station"<<stations[station_id]->getName()<<"\n";
            Train new_train(id, stations[station_id], line, false);
            trains.push_back(new_train);
        }
    }
    std::cout<<"Симуляция запущена!Все передвижения можете посмотерть в файле trips.md\n";
    std::vector<std::thread> threads;
    for (auto& train : trains) {
        threads.push_back(std::thread(&Train::runTrain, &train,std::ref(outFile)));
    }
    for (auto& thread : threads) {
        thread.join();
    }

    
    return 0;
}