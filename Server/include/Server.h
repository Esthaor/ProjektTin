//
// Created by Michal Lipinski on 02.05.16.
//

#ifndef PROJEKTTIN_SERVER_H
#define PROJEKTTIN_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <string>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;

class Server {

private:
    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char buffer[1024];
    int rval;
    boost::thread* communicationThread;

public:
    Server();
    ~Server();

    std::string writeJson(std::string status, int port, std::string endCondition, int endConditionValue, int alarmValue);
    bool sendJson(std::string json);
    void error(char* msg);

};


#endif //PROJEKTTIN_SERVER_H
