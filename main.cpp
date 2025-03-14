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

MetroLine stringToMetroLine(const std::string& line) {
    if (line == "RED") return MetroLine::RED;
    if (line == "GREEN") return MetroLine::GREEN;
    if (line == "PURPLE") return MetroLine::PURPLE;
    if (line == "YELLOW") return MetroLine::YELLOW;
    throw std::runtime_error("Unknown metro line: " + line);
}

// Функция для загрузки станций из JSON
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
            int prevId = stationJson["prev"];
            station->setPrev(stationMap[prevId]);
        }

        // Устанавливаем next
        for (const auto& nextJson : stationJson["next"]) {
            int nextId = nextJson["id"];
            std::string lineStr = nextJson["line"];
            MetroLine line = stringToMetroLine(lineStr);
            station->addNext(stationMap[nextId], line);
        }
    }

    return stations;
}
int main() {
   /* auto hodjasan = std::make_shared<Station>(1, "Ходжасан", 1440, false, false);
    auto avtovokzal = std::make_shared<Station>(2, "Автовокзал", 1440, false, false);
    auto memar_adjemi = std::make_shared<Station>(3, "Мемар Аджеми", 1050, false, false);
    auto yanvarya = std::make_shared<Station>(4, "20 Января", 1050, false, false);
    auto noyabra = std::make_shared<Station>(5, "8 Ноября", 1440, true, false); // Конечная фиолетовой линии
    auto inshaatchylar = std::make_shared<Station>(6, "Иншаатчылар", 1050, false, false);
    auto elmlar_akademiyasy = std::make_shared<Station>(7, "Элмлар Академиясы",1050, false, false);
    auto nizami = std::make_shared<Station>(8, "Низами",1050, false, false);
    auto sahil = std::make_shared<Station>(9, "Сахил",1050, false, false);
    auto icheri_sheher = std::make_shared<Station>(10, "Ичери Шехер",1050, false, false);
    auto may = std::make_shared<Station>(11, "28 Мая",1670, false, false);
    auto gandzhlik = std::make_shared<Station>(12, "Гянджлик",1050, false, false);
    auto nariman_narimanov = std::make_shared<Station>(13, "Нариман Нариманов", 1050, false, false);
    auto ulduz = std::make_shared<Station>(14, "Улдуз", 1050, false, false);
    auto bakmil = std::make_shared<Station>(15, "Бакмил", 1050, true, true); // Депо и конечная
    auto koroğlu = std::make_shared<Station>(16, "Кёроглу", 2, false, false);
    auto qara_qarayev = std::make_shared<Station>(17, "Кара Караев", 1050, false, false);
    auto neftchilar = std::make_shared<Station>(18, "Нефтчиляр", 2, false, false);
    auto xalqlar_dostluqu = std::make_shared<Station>(19, "Халглар Достлугу", 1050, false, false);
    auto ahmedly = std::make_shared<Station>(20, "Ахмедлы", 1050, false, false);
    auto azi_aslanov = std::make_shared<Station>(21, "Ази Асланов", 1050, true, true); // Депо и конечная
    auto djabar_djabbarly = std::make_shared<Station>(22, "Джафар Джаббарлы", 1050, false, false);
    auto shah_islam_khatayi = std::make_shared<Station>(23, "Шах Исмаил Хатаи", 1050, true, true); // Депо и конечная
    auto darnagul = std::make_shared<Station>(24, "Дарнагюль", 1050, false, false);
    auto azatlyg = std::make_shared<Station>(25, "Азадлыг", 1050, false, false);
    auto nasimi = std::make_shared<Station>(26, "Насими", 1050, false, false);
    // Фиолетовая линия: Ходжасан → Автовокзал → Мемар Аджеми → 8 Ноября
    hodjasan->addNext(avtovokzal, MetroLine::PURPLE);
    avtovokzal->setPrev(hodjasan);
    avtovokzal->addNext(memar_adjemi, MetroLine::PURPLE);
    memar_adjemi->setPrev(avtovokzal);
    memar_adjemi->addNext(noyabra, MetroLine::PURPLE);
    noyabra->setPrev(memar_adjemi);

    //Зеленая линия: Дарнагюль → Азатлыг → Насими → Мемар Аджеми → 20 Ноября → Иншаатчылар → Элмляр Академиясы → Низами → 28 Мая → Гянджлик → Нариман Нариманов → (Бакмил or Кёроглу) → Кара Караев → Нефтчиляр → Халглар Достлугу → Ахмедлы → Ази Асланов
    darnagul->addNext(azatlyg, MetroLine::GREEN);
    azatlyg->setPrev(darnagul);
    azatlyg->addNext(nasimi, MetroLine::GREEN);
    nasimi->setPrev(azatlyg);
    nasimi->addNext(memar_adjemi, MetroLine::GREEN);
    memar_adjemi->addNext(yanvarya, MetroLine::GREEN);
    yanvarya->setPrev(memar_adjemi);
    yanvarya->addNext(inshaatchylar, MetroLine::GREEN);
    inshaatchylar->setPrev(yanvarya);
    inshaatchylar->addNext(elmlar_akademiyasy, MetroLine::GREEN);
    elmlar_akademiyasy->setPrev(inshaatchylar);
    elmlar_akademiyasy->addNext(nizami, MetroLine::GREEN);
    nizami->setPrev(elmlar_akademiyasy);
    nizami->addNext(may, MetroLine::GREEN);
    may->setPrev(nizami);

    // Крассная линия: Ичери Шехер → Сахил → 28 Мая → Гянджлик → Нариман Нариманов → (Бакмил or Кёроглу) → Кара Караев → Нефтчиляр → Халглар Достлугу → Ахмедлы → Ази Асланов
    icheri_sheher->addNext(sahil, MetroLine::RED);
    sahil->setPrev(icheri_sheher);
    sahil->addNext(may, MetroLine::RED);
    may->setPrev(sahil);

    // Момент с нариманово: 28 Мая → Гянджлик → Нариман Нариманов → (branches to Бакмил or Кёроглу)
    may->addNext(gandzhlik, MetroLine::RED);
    may->addNext(gandzhlik, MetroLine::GREEN);
    may->addNext(djabar_djabbarly, MetroLine::YELLOW); // Branch to Yellow Line
    gandzhlik->setPrev(may);
    gandzhlik->addNext(nariman_narimanov, MetroLine::RED);
    gandzhlik->addNext(nariman_narimanov, MetroLine::GREEN);
    nariman_narimanov->setPrev(gandzhlik);
    nariman_narimanov->addNext(bakmil, MetroLine::RED); // Branch 1
    nariman_narimanov->addNext(bakmil, MetroLine::GREEN);
    nariman_narimanov->addNext(ulduz, MetroLine::RED); // Branch 2
    nariman_narimanov->addNext(ulduz, MetroLine::GREEN);
    bakmil->setPrev(nariman_narimanov);
    ulduz->setPrev(nariman_narimanov);
    ulduz->addNext(koroğlu,MetroLine::RED);
    ulduz->addNext(koroğlu,MetroLine::GREEN);
    // После кероглу: Кёроглу → Кара Караев → Нефтчиляр → Халглар Достлугу → Ахмедлы → Ази Асланов
    koroğlu->addNext(qara_qarayev, MetroLine::RED);
    koroğlu->addNext(qara_qarayev, MetroLine::GREEN);
    qara_qarayev->setPrev(koroğlu);
    qara_qarayev->addNext(neftchilar, MetroLine::RED);
    qara_qarayev->addNext(neftchilar, MetroLine::GREEN);
    neftchilar->setPrev(qara_qarayev);
    neftchilar->addNext(xalqlar_dostluqu, MetroLine::RED);
    neftchilar->addNext(xalqlar_dostluqu, MetroLine::GREEN);
    xalqlar_dostluqu->setPrev(neftchilar);
    xalqlar_dostluqu->addNext(ahmedly, MetroLine::RED);
    xalqlar_dostluqu->addNext(ahmedly, MetroLine::GREEN);
    ahmedly->setPrev(xalqlar_dostluqu);
    ahmedly->addNext(azi_aslanov, MetroLine::RED);
    ahmedly->addNext(azi_aslanov, MetroLine::GREEN);
    azi_aslanov->setPrev(ahmedly);

    // Желтая линия: Джафар Джаббарлы → Шах Исмаил Хатаи
    djabar_djabbarly->setPrev(shah_islam_khatayi);
    djabar_djabbarly->addNext(shah_islam_khatayi, MetroLine::YELLOW);
    shah_islam_khatayi->setPrev(djabar_djabbarly);
    shah_islam_khatayi->addNext(djabar_djabbarly, MetroLine::YELLOW);*/
    //try {
        auto stations = loadStationsFromJson("stations.json");
        std::cout << "Loaded " << stations.size() << " stations from JSON.\n";

        // Пример вывода для проверки
        for (const auto& station : stations) {
            std::cout << "Station: " << station->getName() << " (ID: " << station->getId() << ")\n";
        }
   /*} catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }*/
    Train train_red(1, stations[9], MetroLine::RED);
   /* Train train_green(2, darnagul, MetroLine::GREEN);
    Train train_purple(3, hodjasan, MetroLine::PURPLE);
    Train train_yellow(4, djabar_djabbarly, MetroLine::YELLOW);*/

    std::thread red_thread(&Train::runTrain, &train_red);
    //std::thread green_thread(&Train::runTrain, &train_green);*/
   /*std::thread purple_thread(&Train::runTrain, &train_purple);
    std::thread yellow_thread(&Train::runTrain, &train_yellow);*/

    red_thread.join();
    //green_thread.join();*/
  /* purple_thread.join();
    yellow_thread.join();*/
    
    return 0;
}