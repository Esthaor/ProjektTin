//
// Created by Aleksandra Ziegart on 10/05/16.
//

#ifndef PROJEKTTIN_SOCKET_H
#define PROJEKTTIN_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>


using namespace std;

class Socket
{
public:
    Socket();
    ~Socket();


    int opt;

    int master_socket, new_socket;
    int  addrlen;
    int client_socket[30];
    int nactive , rval , sd;
    int max_sd;
    struct sockaddr_in address;
    struct timeval to;

    char buffer[1025];

    //socket descriptors
    fd_set readfds;

    int i;

    bool configureSocket();
    bool startCommunication(string json);

};


#endif //PROJEKTTIN_SOCKET_H
