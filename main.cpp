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

void RunMetro(Train& train) {
    train.runTrain();
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
    Train train_purple(3, stations[0], MetroLine::PURPLE,false);
    std::thread purple_thread(&Train::runTrain, &train_purple);
    Train train_purple1(31, stations[4], MetroLine::PURPLE,true);
    std::thread purple_thread1(&Train::runTrain, &train_purple1);
    purple_thread.join();
    purple_thread1.join();
    return 0;
}