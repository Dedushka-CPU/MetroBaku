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
        while (!current_station->TryArriveTrain(train_id, should_turn,forward)) {
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
            current_station->p_g_t(-passengers_in); // забираем людей со станции
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
            current_station=current_station->getNextForLine(line);
        } 
        else {
            if(current_station->getId()==13){//Нариманов
                int i=rand()%10;
                if(i<=2){
                    current_station=current_station->getPrevById(15);
                    std::cout<<"Qatar bakmil stansiyasi qedir.*Свет в вагоне начал тускнеть*\n";
                }else{
                    current_station=current_station->getPrevById(14);  
                }
                
            }else{
             current_station = current_station->getPrevForLine(line);
            }
            
        }
        
    }
}

bool Train::get_forward(){
    return forward;
}
