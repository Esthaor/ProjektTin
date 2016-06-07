//
// Created by Michał Lipiński on 30.05.16.
//

#ifndef PROJEKTTIN_THREADMUTEX_H
#define PROJEKTTIN_THREADMUTEX_H

#include <string>
#include <boost/thread/mutex.hpp>

class ThreadMutex {
public:
    //bool end_capture;
    int end_condition_value;
    int alarm;

    int id;
    bool to_read;
    bool stop;
    boost::mutex mutex;

    int all_total_length;

    ThreadMutex(int id);
    void fillData(std::string status, int end_condition_value, int alarm);
};


#endif //PROJEKTTIN_THREADMUTEX_H
