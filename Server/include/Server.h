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
    struct measurement {
        string ip;
        int port;
        string endCondition;
        int endConditionValue;
        string alarmType;
        int alarmValue;
    };

    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char buffer[1024];
    int rval;
    boost::thread* communicationThread;
    vector<struct measurement> measurements;



public:
    Server();
    ~Server();

    int init();
    static std::string writeJson(std::string status, int port, std::string endCondition, int endConditionValue, int alarmValue);
    bool sendJSON(string ip, string json);
    void error(char* msg);
    void addToMeasurements(string ip, string port, string endCondition, string endConditionValue, string alarmValue);
    void sendMeasurements();
    static void sendMeasurement(string ip, string json);
    static int nextAgentID;
    static string writeSmallJson(string status, int id);
    static void addMachine(string ip);

    };


#endif //PROJEKTTIN_SERVER_H
