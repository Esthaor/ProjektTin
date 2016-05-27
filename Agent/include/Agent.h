//
// Created by Michal Lipinski on 02.05.16.
//

#ifndef PROJECT_AGENT_H
#define PROJECT_AGENT_H

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <pcap.h>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;

class Agent {
public:
    enum EndCondition{
        PACKETS,
        TIME,
        NONE
    };

    Agent();
    Agent(int port, EndCondition end_condition, int end_condition_value, int alarm);
    ~Agent();

    void displayInformation();

private:
    int port;
    EndCondition end_condition;
    int end_condition_value;
    int alarm;

    int packet_counter;
    bool enable_alarms;

    int sniff();
    static void callback(u_char* useless, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    bool receiveJson();

};




#endif //PROJECT_AGENT_H
