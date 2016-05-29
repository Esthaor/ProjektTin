//
// Created by Aleksandra Ziegart on 10/05/16.
//

#include "../include/Socket.h"


Socket::Socket()
{

}

Socket::~Socket()
{

}

bool Socket::configureSocket(int port)
{
    portno = port;

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        cout << "ERROR opening socket" << endl;
        return false;
    }

    server = gethostbyname("localhost");

    if (server != NULL)
    {
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);

        /* Now connect to the server */
        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            cout << "ERROR connecting" << endl;
            return false;
        }
    }
    else
        return false;




    return true;
}
bool Socket::startCommunication()
{
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        cout << "ERROR reading from socket" << endl;
        return false;
    }

    cout << "Odebrano: " << buffer << endl;  //TODO: konwersja buffer -> .. -> ptree


while(1)
{
    cout << "Message (press q to terminate): " << endl;

    if(buffer[0] == 'q')
        break;

    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }


}
    return true;
}
