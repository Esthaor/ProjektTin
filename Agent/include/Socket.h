//
// Created by Aleksandra Ziegart on 10/05/16.
//

#ifndef PROJEKTTIN_SOCKET_H
#define PROJEKTTIN_SOCKET_H

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<iostream>
#include<netdb.h>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <ThreadMutex.h>
#include <Agent.h>
#include <unordered_map>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace std;


class Socket
{
public:
    class AgentThread{
    public:
        int id;
        boost::thread* thread_capture;

        AgentThread(int id, Agent* agent) {
            this->id = id;
            this->thread_capture = new boost::thread(boost::bind(&Agent::sniff, agent));
        }
    };

    Socket();
    ~Socket();

    int socket_descriptor, client_socket, port_number, c;
    struct sockaddr_in server, client;

    int next_capture_id;
    std::unordered_map<int, AgentThread*> thread_list;
    static std::unordered_map<int, ThreadMutex*> mutex_list;

    static bool sendToServer(string json);
    bool configureSocket(int port);
    void connection_handler (int socket_desc);

};

#endif //PROJEKTTIN_SOCKET_H
