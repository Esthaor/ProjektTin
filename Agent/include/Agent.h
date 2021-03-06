//
// Created by Michal Lipinski on 02.05.16.
//

#ifndef PROJECT_AGENT_H
#define PROJECT_AGENT_H

#include <iostream>
#include <stdio.h>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <ctime>
#include <string>
#include <pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <ThreadMutex.h>

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

    int capture_id;

    Agent();
    ~Agent();

    Agent(int capture_id, string port, EndCondition end_condition, int end_condition_value, int alarm);

    void displayInformation();
    int showAllDevices();

    int sniff();
    string buildJson(string type);

private:
    ThreadMutex* threadMutex;

    pcap_t* handler;
    string port;
    EndCondition end_condition;
    int end_condition_value;
    time_t start_sniffing_time;
    boost::thread* thread_capture_check;
    boost::thread* thread_change_data;
    bool enable_alarms;
    int alarm;

    int packet_counter;
    int all_captured_length;
    int all_total_length;

    char* findDevice();
    static void callback(u_char* args, const struct pcap_pkthdr* packet_header, const u_char* packet_body);
    void packetInfo(const u_char *packet, struct pcap_pkthdr packet_header);
    void rewriteData();
    void checkCapture();
};

#endif //PROJECT_AGENT_H
