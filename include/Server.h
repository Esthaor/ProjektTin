//
// Created by Michal Lipinski on 02.05.16.
//

#ifndef PROJEKTTIN_SERVER_H
#define PROJEKTTIN_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Server {

private:
    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char buffer[1024];
    int rval;

public:
    Server();
    ~Server();

    bool writeJson();
    bool sendJson();
    void error(char* msg);

};


#endif //PROJEKTTIN_SERVER_H
