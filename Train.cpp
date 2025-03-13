/*#include "Metro.cpp"

class Train{
private:
    int train_id;
    std::shared_ptr<Station> station_start;
    bool forward;//true если вперед поедет(например выехал с депо),false если обратно(возвращается в депо в которое выехал)
public:
    Train(int id,std::shared_ptr<Station> s_s,bool f=true):train_id(id),station_start(s_s),forward(f){}
    //~Train();
    void runTrain() {
        std::shared_ptr<Station> current_station=station_start;
        while (current_station) {
            bool should_turn=false;
            while (!current_station->TryArriveTrain(train_id,should_turn)) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            if (should_turn) {
                forward=!forward;
            }
            if (forward=true) {
                current_station=current_station->getNext();
            }else {
                current_station=current_station->getPrev();
            }
        }
    }
    void RunMetro(Train& train) {
        train.runTrain();
    }
  };*/