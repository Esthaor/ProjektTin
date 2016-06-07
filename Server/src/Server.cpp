//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Server.h"
#include "../include/Socket.h"
#include "../include/Database.h"
#include "../include/mongoose.h"
#include "WwwServer.h"

Server::Server() {

}

Server::~Server() {

}

int Server::nextAgentID = 0;

int Server::init() {
    //utworzenie bazy danych
    Database* database = new Database();
    database->open();
    database->create_table();
    database->create_table_agents();

    //database->insert(6, 1, "zakonczony", 12344, 1234, 764);
    //database->insert_agents(6, "192.168.1.10");
    database->select_all();
    database->select_ip("192.168.1.10");
    cout << database->ipaddr << endl;
    //database->check_if_exists_agents("192.168.1.1");
    //cout << "czy istnieje? " << database->agentExists << endl;

    /*database->update( "w realizacji" ,73488, 111);
    database->select_all();
    database->delete_row(73488, 111);*/

    //odpalenie gniazda - nasłuch
    Socket* socket = new Socket();
    socket->configureSocket(); //listener

    communicationThread = new boost::thread(boost::bind(&Socket::startCommunication, socket)); //nasłuch na połączenia
    //TODO: odczyt ze struktury danych z Agentami do odpalenia pomiarów
    //iteracja po liście Agentów i odpalenie wątku złożenia jsona i wysłania polecenia pomiaru dla każdego z nich

    WwwServer* www = new WwwServer();
    www->database = database;

    sendMeasurements();

    www->wwwServerStart();

    database->close();
}

string Server::writeJson(string status, int port, string endCondition, int endConditionValue, int alarmValue) {

    ptree contents;

    // Dodanie elementów JSONa
    contents.put("status", status);
    contents.put("port", port);
    contents.put("endCondition", endCondition);
    contents.put("endConditionValue", endConditionValue);
    contents.put("alarmValue", alarmValue);

    // Formowanie gotowego stringa
    std::ostringstream buf;
    write_json(buf, contents, false);
    string json = buf.str();

    return json;
}

bool Server::sendJSON(string ip, string json) {






    return true;
}



void Server::addToMeasurements(string ip, string port, string endCondition, string endConditionValue, string alarmValue) {
    struct measurement m;
    m.ip = ip;
    m.port = stoi(port);
    m.endCondition = endCondition;
    m.endConditionValue = stoi(endConditionValue);
    m.alarmValue = stoi(alarmValue);

    measurements.push_back(m);
}

void Server::sendMeasurements() {
    for(int i=0; i<measurements.size(); ++i) {
        string json = writeJson("start", measurements[i].port, measurements[i].endCondition,
                                measurements[i].endConditionValue, measurements[i].alarmValue);
        cout<<"wysylam"<<endl;
        boost::thread(Socket::sendToAgent, measurements[i].ip, json);

        WwwServer::database->check_if_exists_agents(measurements[i].ip);
        if((WwwServer::database->check_exists_value())==0) {
            cout << "nie istnieje" << endl;
            WwwServer::database->insert_agents(nextAgentID, measurements[i].ip);
        }

        //sleep(1);
    }
}
