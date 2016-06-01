//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include <getopt.h>

#include "../include/Agent.h"
#include "../include/Socket.h"

#define PORT 5010


void help();

int main(int argc, char* argv[]) {
    int opt     = 0;
    string port = "port ";
    int end_condition_value = -1;
    int alarm   = -1;
    Agent::EndCondition end_condition = Agent::NONE;
    int long_index = 0;

    //std::list<boost::thread*> thread_list;

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
    static Socket* socket = new Socket();

    if(argc > 1) {
        Agent *agent = new Agent(socket->next_capture_id, port, end_condition, end_condition_value, alarm);
        socket->thread_list.insert(std::make_pair(socket->next_capture_id, new Socket::AgentThread(socket->next_capture_id, agent)));
        socket->mutex_list.insert(std::make_pair(socket->next_capture_id, new ThreadMutex(socket->next_capture_id)));
        socket->next_capture_id++;
        string to_send = agent->buildJson("started");
        cout << "Taki zaraz powinien wyslac " << to_send << endl;
        socket->sendToServer(to_send);
        //informacja do serwera o uruchomionym pomiarze z palca
        socket->configureSocket(PORT);
    }
    else {
        socket->configureSocket(PORT);
    }
}

void help() {
    std::cout << "sidagent <port> <endCondition> <alarm>" << std::endl;
    std::cout << "<port>\t\t-p or --port\tport to sniff" << std::endl;
    std::cout << "<endCondition>\t-c or --pacekts\tafter how many packets agent should end collecting statistics" << std::endl;
    std::cout << "<endCondition>\t-t or --time\tafter how many seconds agent should end collecting statistics" << std::endl;
    std::cout << "<alarm>\t\t-a or --alarm\tafter how many captured bytes agent should signal alarm" << std::endl;
}