//
// Created by Aleksandra Ziegart on 10/05/16.
//

#include "../include/Socket.h"

#define PORT 5010
#define MAX_CLIENTS 30

Socket::Socket()
{

}

Socket::~Socket()
{
    close(master_socket);
    close(new_socket);
}


bool Socket::configureSocket()
{

    opt = 1;

    //set client_socket[] to 0
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout << "Listener on port " << PORT << endl;

    return true;
}


bool Socket::startCommunication(string json)
{
    int licznik = 20; //temp

    //temp message
    char *message = (char *)"Start sniffing";


    if (listen(master_socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    addrlen = sizeof(address);
    cout <<  "Waiting for connections ..." << endl;

    while((licznik--) > 0)
    {


        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        to.tv_sec = 5;
        to.tv_usec = 0;


        //add child sockets to set
        for ( i = 0 ; i < MAX_CLIENTS ; i++)
        {
            //socket descriptor
            sd = client_socket[i];


            if(sd > 0)
                FD_SET( sd , &readfds);

            //for select() function
            if(sd > max_sd)
                max_sd = sd;
        }


        nactive = select( max_sd + 1 , &readfds , NULL , NULL , &to);

        if ((nactive < 0) && (errno!=EINTR))
        {
            cout << "select error" << endl;
        }



        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            cout << "New connection, socket fd:" << new_socket << ",  ip: " << inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << endl;

            //send message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            cout << "Message sent successfully" << endl;

            //add new socket to array of sockets
            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    cout << "Adding to list of sockets as " <<  i << endl;

                    break;
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                memset(buffer, 0, sizeof(buffer));


                if ((rval = read( sd , buffer, 1024)) == 0)
                {
                    //Some Agent - disconnected
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    cout << "Agent sd:" << sd << "  disconnected , ip " << inet_ntoa(address.sin_addr)  << ", port " <<  ntohs(address.sin_port) << endl;

                    //cout << "-  " << sd << " ->   " << buffer << endl;

                    close( sd );
                    client_socket[i] = 0;
                }
                else if(rval == -1)
                    perror("reading stream message");
                else
                {
                    cout << "-  " << sd << " ->   " << buffer << endl;
                    //buffer[valread] = '\0';
                }
            }
        }
    }
    return true;
}



/*
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

 */

    /* Initialize socket structure */
  /*  bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5010;


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
*/
    /* Now bind the host address */
 /*   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "ERROR on binding" << endl;
        return false;
    }
*/
    /* Now start listening for the clients */
/*
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
*/
    /* Accept actual connection from the client */
/*   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);

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
*/