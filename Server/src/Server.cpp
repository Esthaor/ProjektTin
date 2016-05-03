//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Server.h"

Server::Server() {

}

Server::~Server() {

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

    std::cout << "Wysłano JSON: " << json << std::endl;
    return true;
}