//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Server.h"
#include "../include/Socket.h"
#include "../include/Database.h"
//#include "../include/mongoose.h"

Server::Server() {

}

Server::~Server() {

}

int Server::init() {
    //utworzenie bazy danych
    Database* database = new Database();
    database->open();
    database->create_table();
    database->create_table_agents();

/*    database->insert(73488, 111, "zakonczony", 12344, 1234, 764 );
    database->select_all();
    database->update( "w realizacji" ,73488, 111);
    database->select_all();
    database->delete_row(73488, 111);*/

    //odpalenie gniazda - nasłuch
    Socket* socket = new Socket();
    socket->configureSocket(); //listener

    communicationThread = new boost::thread(boost::bind(&Socket::startCommunication, socket)); //nasłuch na połączenia
    //TODO: odczyt ze struktury danych z Agentami do odpalenia pomiarów
    //iteracja po liście Agentów i odpalenie wątku złożenia jsona i wysłania polecenia pomiaru dla każdego z nich


    //TODO: odpalenie serwera WWW z klasy Webserver (do utworzenia)

    database->close();
}

string Server::writeJson(string status, int port, string endCondition, int endConditionValue, int alarmValue) {

    ptree root;
    ptree contents;

    // Dodanie elementów JSONa
    contents.put("status", status);
    contents.put("port", port);
    contents.put("endCondition", endCondition);
    contents.put("endConditionValue", endConditionValue);
    contents.put("alarmValue", alarmValue);
    root.put_child("measurement", contents);

    // Formowanie gotowego stringa
    std::ostringstream buf;
    write_json(buf, root, false);
    string json = buf.str();

    return json;
}

bool Server::sendJson(string json) {






    return true;
}

void Server::addToMeasurements(string ip, string port, string endCondition, string endConditionValue, string alarmType, string alarmValue) {
    struct measurement m;
    m.ip = ip;
    m.port = stoi(port);
    m.endCondition = endCondition;
    m.endConditionValue = stoi(endConditionValue);
    m.alarmType = alarmType;
    m.alarmValue = stoi(alarmValue);

    measurements.push_back(m);
}

void Server::displayMeasurements() {
    for(int i=0; i<measurements.size(); ++i)
        std::cout << measurements[i].ip << "," << measurements[i].port << "," << measurements[i].endCondition << "," << measurements[i].endConditionValue << "," << measurements[i].alarmType << "," << measurements[i].alarmValue << endl;
}
