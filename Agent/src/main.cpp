//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include <getopt.h>

#include "../include/Agent.h"

void help();

int main(int argc, char* argv[]) {
    int opt     = 0;
    string port = "port ";
    int end_condition_value = -1;
    int alarm   = -1;
    Agent::EndCondition end_condition = Agent::NONE;
    int long_index = 0;

    std::list<boost::thread*> thread_list;

    std::cout << "Hello, World2!" << std::endl;

    static struct option long_options[] = {
            { "help",       no_argument,        0,  'h' },
            { "port",       required_argument,  0,  'p' },
            { "packets",    required_argument,  0,  'c' },
            { "time",       required_argument,  0,  't' },
            { "alarm",      required_argument,  0,  'a' },
            { 0,            0,                  0,  0   }
    };

    while ((opt = getopt_long(argc, argv, "hp:c:t:a:", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'h':
                help();
                return 0;
            case 'p':
                port += optarg;
                break;
            case 'c':
                end_condition = Agent::PACKETS;
                end_condition_value = atoi(optarg);
                break;
            case 't':
                end_condition = Agent::TIME;
                end_condition_value = atoi(optarg);
                break;
            case 'a':
                alarm = atoi(optarg);
                break;
            default:
                help();
                exit(1);
        }
    }

    Agent *agent = new Agent(port, end_condition, end_condition_value, alarm);

    /*while(true){
        if(agent->receiveJson()){
            thread_list.push_back(new boost::thread(boost::bind(&Agent::sniff, agent)));
        }
    }*/

    agent->displayInformation();
}

void help() {
    std::cout << "sidagent <port> <endCondition> <alarm>" << std::endl;
    std::cout << "<port>\t\t-p or -port\tport to sniff" << std::endl;
    std::cout << "<endCondition>\t-c or -pacekts\tafter how many packets agent should end collecting statistics" << std::endl;
    std::cout << "<endCondition>\t-t or -time\tafter how many seconds agent should end collecting statistics" << std::endl;
    std::cout << "<alarm>\t\t-a or -alarm\tafter how many captured packets agent should signal alarm" << std::endl;
}