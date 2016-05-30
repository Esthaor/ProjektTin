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

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace std;


class Socket
{
public:
    Socket();
    ~Socket();

    int socket_descriptor, client_socket, port_number, c;
    struct sockaddr_in server, client;

    unsigned next_capture_id;
    std::vector<boost::thread*> thread_list;
    static std::vector<ThreadMutex*> mutex_list;

    ThreadMutex* findMutex(int id);

    static bool sendToServer(string json);
    bool configureSocket(int port);
    void connection_handler (int socket_desc);
};

#endif //PROJEKTTIN_SOCKET_H
