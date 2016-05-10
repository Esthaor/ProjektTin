//
// Created by Aleksandra Ziegart on 10/05/16.
//

#include "../include/Socket.h"


Socket::Socket()
{

}

Socket::~Socket()
{
    close(newsockfd);
    close(sockfd);
}

bool Socket::configureSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        cout << "ERROR opening socket" << endl;
        return false;
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5010;


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "ERROR on binding" << endl;
        return false;
    }

    /* Now start listening for the clients */

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);

    if (newsockfd < 0)
    {
        cout << "ERROR on accept" << endl;
        return false;
    }
    return true;

}
bool Socket::startCommunication(string json)
{
    bzero(buffer,256);
    strncpy(buffer, json.c_str(), sizeof(buffer));
    n = write(newsockfd, buffer, strlen(buffer));

    if (n < 0)
    {
        cout << "ERROR writing to socket" << endl;
        return false;
    }

    return true;
}