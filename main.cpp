#include <iostream>
#include "Metro.cpp"
#include <windows.h>

#ifdef _WIN32
#include <windows.h> // Для Windows: настройка консоли
#endif

void RunMetro(Train& train) {
    train.runTrain();
}

int main() {
    auto hodjasan = std::make_shared<Station>(1, "Ходжасан", 2);
    auto avtovokzal = std::make_shared<Station>(2, "Автовокзал", 2);
    auto memar_adjemi = std::make_shared<Station>(3, "Мемар Аджеми", 2);
    auto yanvarya = std::make_shared<Station>(4, "20 Ноября", 2);
    auto noyabra = std::make_shared<Station>(5, "8 Ноября", 2);
    auto inshaatchylar = std::make_shared<Station>(6, "Иншаатчылар", 2);
    auto elmlar_akademiyasy = std::make_shared<Station>(7, "Элмлар Академиясы", 2);
    auto niizami = std::make_shared<Station>(8, "Низами", 2);
    auto sahil = std::make_shared<Station>(9, "Сахил", 2);
    auto icheri_sheher = std::make_shared<Station>(10, "Ичери Шехер", 2);
    auto _may = std::make_shared<Station>(11, "28 Мая", 2);
    auto gandzhlik = std::make_shared<Station>(12, "Гянджлик", 2);
    auto nariman_narimanov = std::make_shared<Station>(13, "Нариман Нариманов", 2);
    auto ulduz = std::make_shared<Station>(14, "Улдуз", 2);
    auto bakmil = std::make_shared<Station>(15, "Бакмил", 2, true); // Depot
    auto koroğlu = std::make_shared<Station>(16, "Кёроглу", 2);
    auto qara_qarayev = std::make_shared<Station>(17, "Кара Караев", 2);
    auto neftchilar = std::make_shared<Station>(18, "Нефтчиляр", 2);
    auto xalqlar_dostluqu = std::make_shared<Station>(19, "Халглар Достлугу", 2);
    auto ahmedly = std::make_shared<Station>(20, "Ахмедлы", 2);
    auto azi_aslanov = std::make_shared<Station>(21, "Ази Асланов", 2, true); // Depot
    auto djabar_djabbarly = std::make_shared<Station>(22, "Джафар Джаббарлы", 2);
    auto shah_islam_khatayi = std::make_shared<Station>(23, "Шах Исмаил Хатаи", 2, true); // Depot
    auto dargahli = std::make_shared<Station>(24, "Дарнагюль", 2);
    auto azatlyg = std::make_shared<Station>(25, "Азадлыг", 2);
    auto nasimi = std::make_shared<Station>(26, "Насими", 2);
    // Фиолетовая линия: Ходжасан → Автовокзал → Мемар Аджеми → 8 Ноября
    hodjasan->addNext(avtovokzal, MetroLine::PURPLE);
    avtovokzal->setPrev(hodjasan);
    avtovokzal->addNext(memar_adjemi, MetroLine::PURPLE);
    memar_adjemi->setPrev(avtovokzal);
    memar_adjemi->addNext(noyabra, MetroLine::PURPLE);
    noyabra->setPrev(memar_adjemi);

    //Зеленая линия: Дарнагюль → Азатлыг → Насими → Мемар Аджеми → 20 Ноября → Иншаатчылар → Элмляр Академиясы → Низами → 28 Мая → Гянджлик → Нариман Нариманов → (Бакмил or Кёроглу) → Кара Караев → Нефтчиляр → Халглар Достлугу → Ахмедлы → Ази Асланов
    dargahli->addNext(azatlyg, MetroLine::GREEN);
    azatlyg->setPrev(dargahli);
    azatlyg->addNext(nasimi, MetroLine::GREEN);
    nasimi->setPrev(azatlyg);
    nasimi->addNext(memar_adjemi, MetroLine::GREEN);
    memar_adjemi->addNext(noyabra, MetroLine::GREEN);
    noyabra->setPrev(memar_adjemi);
    noyabra->addNext(inshaatchylar, MetroLine::GREEN);
    inshaatchylar->setPrev(noyabra);
    inshaatchylar->addNext(elmlar_akademiyasy, MetroLine::GREEN);
    elmlar_akademiyasy->setPrev(inshaatchylar);
    elmlar_akademiyasy->addNext(niizami, MetroLine::GREEN);
    niizami->setPrev(elmlar_akademiyasy);
    niizami->addNext(_may, MetroLine::GREEN);
    _may->setPrev(niizami);

    // Крассная линия: Ичери Шехер → Сахил → 28 Мая → Гянджлик → Нариман Нариманов → (Бакмил or Кёроглу) → Кара Караев → Нефтчиляр → Халглар Достлугу → Ахмедлы → Ази Асланов
    icheri_sheher->addNext(sahil, MetroLine::RED);
    sahil->setPrev(icheri_sheher);
    sahil->addNext(_may, MetroLine::RED);
    _may->setPrev(sahil);

    // Момент с нариманово: 28 Мая → Гянджлик → Нариман Нариманов → (branches to Бакмил or Кёроглу)
    _may->addNext(gandzhlik, MetroLine::RED);
    _may->addNext(gandzhlik, MetroLine::GREEN);
    _may->addNext(djabar_djabbarly, MetroLine::YELLOW); // Branch to Yellow Line
    gandzhlik->setPrev(_may);
    gandzhlik->addNext(nariman_narimanov, MetroLine::RED);
    gandzhlik->addNext(nariman_narimanov, MetroLine::GREEN);
    nariman_narimanov->setPrev(gandzhlik);
    nariman_narimanov->addNext(bakmil, MetroLine::RED); // Branch 1
    nariman_narimanov->addNext(bakmil, MetroLine::GREEN);
    nariman_narimanov->addNext(koroğlu, MetroLine::RED); // Branch 2
    nariman_narimanov->addNext(koroğlu, MetroLine::GREEN);
    bakmil->setPrev(nariman_narimanov);
    koroğlu->setPrev(nariman_narimanov);

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
    shah_islam_khatayi->addNext(djabar_djabbarly, MetroLine::YELLOW);

    Train train_red(1, icheri_sheher, MetroLine::RED);
    Train train_green(2, dargahli, MetroLine::GREEN);
    Train train_purple(3, hodjasan, MetroLine::PURPLE);
    Train train_yellow(4, djabar_djabbarly, MetroLine::YELLOW);

    /*std::thread red_thread(&Train::runTrain, &train_red);
    std::thread green_thread(&Train::runTrain, &train_green);*/
    std::thread purple_thread(&Train::runTrain, &train_purple);
    std::thread yellow_thread(&Train::runTrain, &train_yellow);

   /* red_thread.join();
    green_thread.join();*/
    purple_thread.join();
    yellow_thread.join();
    return 0;
}