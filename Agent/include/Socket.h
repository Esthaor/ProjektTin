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
#include <zconf.h>
#include <iostream>
using namespace std;

class Socket
{
public:
    Socket();
    ~Socket();

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    bool configureSocket(int port);
    bool startCommunication();

};

#endif //PROJEKTTIN_SOCKET_H
