//
// Created by grzegorz on 07.06.16.
//

#ifndef PROJEKTTIN_WWWSERVER_H
#define PROJEKTTIN_WWWSERVER_H

#include <iostream>
#include "mongoose.h"

using namespace std;

class wwwServer {
public:
    wwwServer(){};
    ~wwwServer(){};
    int wwwServerStart();
    static void handle_sum_call(struct mg_connection *nc, struct http_message *hm);
    static void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
};


#endif //PROJEKTTIN_WWWSERVER_H
