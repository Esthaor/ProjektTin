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
//#include <boost/date_time/local_time/local_date_time.hpp>
#include <ctime>
#include <string>
#include <pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>

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
    Agent(string port, EndCondition end_condition, int end_condition_value, int alarm);
    ~Agent();

    void displayInformation();
    int showAllDevices();
    bool receiveJson();
    int sniff();

private:
    //static Agent* instance;

    pcap_t* handler;
    string port;
    EndCondition end_condition;
    int end_condition_value;
    time_t start_sniffing_time;
    boost::thread* thread_timeout;
    boost::thread* thread_alarm;
    bool enable_alarms;
    int alarm;

    int packet_counter;
    int all_captured_length;
    int all_total_length;

    //Deprecated
    int ip_packet_counter;
    int arp_packet_counter;
    int revarp_packet_counter;
    //End of deprecated

    static void callback(u_char* args, const struct pcap_pkthdr* packet_header, const u_char* packet_body);
    void packetInfo(const u_char *packet, struct pcap_pkthdr packet_header);
    void signalAlarm();
    void stopSniffing();
    string buildJson(string type);

};

#endif //PROJECT_AGENT_H
