#include <iostream>
#include "Metro.cpp"

void RunMetro(Train& train) {
    train.runTrain();
}

int main() {
    auto s1=std::make_shared<Station> (0,"Ичяри шахер",5,true);
    auto s2=std::make_shared<Station> (1,"Сахиль",1,false);
    auto s3=std::make_shared<Station> (2,"28 Мая",3,false);
    auto s4=std::make_shared<Station> (3,"Гянджлик",1,false);
    auto s5=std::make_shared<Station> (4,"Нариман Нариманов",1,false);
    auto s6=std::make_shared<Station> (5,"Бакмиль",5,true);
    s1->setNext(s2);
    s1->setPrev(s2);
    s2->setPrev(s1);
    s2->setNext(s3);
    s3->setPrev(s2);
    s3->setNext(s4);
    s4->setPrev(s3);
    s4->setNext(s5);
    s5->setPrev(s4);
    s5->setNext(s6);
    s6->setPrev(s5);
    s6->setNext(s5);
    Train train1(1,s1,false);
    Train train2(2,s6,true);
    Train train3(3,s4,false);
    Train train4(4,s2,true);
    std::thread t1(RunMetro,std::ref(train1));
    std::thread t2(RunMetro,std::ref(train2));
    t1.join();
    t2.join();
    return 0;
}