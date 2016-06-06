//
// Created by Aleksandra Ziegart on 10/05/16.
//

#include <thread>
#include "../include/Socket.h"
#include "../include/Agent.h"

Socket::Socket()
{
    this->next_capture_id = 0;
}

Socket::~Socket()
{

}

std::unordered_map<int, ThreadMutex*> Socket::mutex_list;// = NULL;

bool Socket::sendToServer(string json)
{
    int socket_desc;
    struct sockaddr_in server;
    char message[256];
    int maxRetries = 10;
    int retryTime = 30;

    json.copy(message,json.length(),0);
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket!\n");
    }
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //adres serwera
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    int tries = 0;
    while (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        tries++;
        if(tries > maxRetries) {
            puts("Server unreachable, max retries count reached! JSON not sent!");
            return false;
        }
        sleep(retryTime);
        puts("Server unreachable, retrying in 30 seconds!\n"); //TODO: retry
    }

    if( send(socket_desc , message , strlen(message) , 0) < 0)
    {
        puts("Sending failed!\n");
        return false;
    }
    cout << "JSON sent: " << json << endl;
    return true;
}

bool Socket::configureSocket(int port)
{
    port_number = port;
    int option = 1;
    /* Create a socket point */
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descriptor < 0)
    {
        cout << "ERROR creating socket!" << endl;
        return false;
    }
    setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR,(char*)&option,sizeof(option));
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

    //Accept an incoming connection

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while( (client_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        new boost::thread(boost::bind(&Socket::connection_handler, this, this->client_socket));
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
            //pole id jest pomijane
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

            Agent *agent = new Agent(this->next_capture_id, port, endCondition, endConditionValue, alarm);
            //this->thread_list.push_back(new Socket::AgentThread(agent));
            this->thread_list.insert(std::make_pair(this->next_capture_id, new Socket::AgentThread(this->next_capture_id, agent)));
            this->mutex_list.insert(std::make_pair(this->next_capture_id, new ThreadMutex(this->next_capture_id)));
            this->next_capture_id++;
        }

        if (status == "change") { //rozpoczęcie pomiaru
            cout << "status is change" << endl;
            int id = root.get<int>("id");
            std::cout << "id from change json: " << id << std::endl;
            string endConditionString = root.get<string>("endCondition");

            int end_condition_value;
            if(endConditionString == "time")
                end_condition_value = root.get<int>("endConditionValue");
            else
                end_condition_value = -1;

            int alarm = root.get<int>("alarmValue");

            ThreadMutex* threadMutex = this->mutex_list[id];//this->findMutex(id);
            threadMutex->mutex.lock();
            std::cout << "po locku w socket" << std::endl;
            threadMutex->fillData(status, end_condition_value, alarm);
            threadMutex->mutex.unlock();
            std::cout << "po unlocku w socket" << std::endl;
        }

        if (status == "stop") { //rozpoczęcie pomiaru
            cout << "status is stop" << endl;
            int id = root.get<int>("id");
            std::cout << "id: " << id << std::endl;
            cout << "poszedl interrupt" << endl;
            std::cout << "znaleziono thread o id: " << this->thread_list[id]->id << std::endl;
            this->thread_list[id]->thread_capture->interrupt();
        }

    }
    cout << "koncze watek" << endl;
}
