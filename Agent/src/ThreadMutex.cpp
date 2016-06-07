//
// Created by Michał Lipiński on 30.05.16.
//

#include "ThreadMutex.h"


ThreadMutex::ThreadMutex(int id){
    std::cout << "buduje mutex" << std::endl;
    this->id = id;
    this->to_read = false;
    this->end_condition_value = -1;
    this->alarm = -1;
    this->all_total_length = 0;
    this->stop = false;
}

void ThreadMutex::fillData(std::string status, int end_condition_value, int alarm){
    std::cout << "wypeniam threadmutex danymi" << std::endl;

    if(status != "change")
        return;
    this->to_read = true;
    this->end_condition_value = end_condition_value;
    this->alarm = alarm;
}