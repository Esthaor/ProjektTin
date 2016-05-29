//
// Created by Aleksandra Ziegart on 10/05/16.
//

#include <thread>
#include "../include/Socket.h"
#include "../include/Agent.h"

Socket::Socket()
{

}

Socket::~Socket()
{

}

bool Socket::configureSocket(int port)
{
    port_number = port;

    /* Create a socket point */
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descriptor < 0)
    {
        cout << "ERROR creating socket!" << endl;
        return false;
    }
    cout << "Socket created" << endl;

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port_number);

    //Bind
    if( bind(socket_descriptor,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("ERROR binding!");
        return 1;
    }
    puts("Bind done!");

    //Listen
    listen(socket_descriptor , 10); //10 - max ilość połączeń w kolejce


    //Accept and incoming connection

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while( (client_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        thread_list.push_back(new boost::thread(boost::bind(&Socket::connection_handler, this, this->client_socket)));
        puts("Handler assigned");
    }
    if (client_socket < 0)
    {
        perror("accept failed");
        return 1;
    }
    return true;
}

/*
 * This will handle connection for each client
 * */
void Socket::connection_handler (int socket_desc)
{
    //Get the socket descriptor
    int sock =  socket_desc;
    char buffer[256];
    bzero(buffer, 256);
    int len = read(sock, buffer, 255);
    if ( len < 0 ) {
        cout << "blad odczytu" << endl;
    } else if ( len == 0 ) {
        cout << "eof" << endl;
    } else {
        string inputBuffer(buffer, len);
        istringstream is(inputBuffer);

        // Create a root
        ptree root;

        // Load the json file in this ptree
        cout << "reading JSON" << endl;
        read_json(is, root);

        string status = root.get<string>("status");
        if (status == "start") { //rozpoczęcie pomiaru
            cout << "status is start" << endl;
            string port = root.get<string>("port");
            string endConditionString = root.get<string>("endCondition");
            Agent::EndCondition endCondition;
            if(endConditionString == "time")
                endCondition = Agent::TIME;
            else if(endConditionString == "threshold")
                endCondition = Agent::PACKETS;
            int endConditionValue = root.get<int>("endConditionValue");
            int alarm = root.get<int>("alarmValue");
            port.insert(0, "port ");
            Agent *agent = new Agent(port, endCondition, endConditionValue, alarm);
            agent->sniff();
        }
    }

    cout << "koncze watek" << endl;
}