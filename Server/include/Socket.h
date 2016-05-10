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
#include <string>
using namespace std;

class Socket
{
public:
    Socket();
    ~Socket();

    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    int  n;

    bool configureSocket();
    bool startCommunication(string json);
};


#endif //PROJEKTTIN_SOCKET_H
