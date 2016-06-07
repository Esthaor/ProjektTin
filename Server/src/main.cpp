//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include "../include/Server.h"
#include "WwwServer.h"

using namespace std;
/*
static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
    if (ev == MG_EV_HTTP_REQUEST) {
        mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
    }
}*/

int main()
{

    //TODO: fopen pliku konfiguracyjnego, zapis do struktury danych

    Server* server = new Server();

    remove( "database.db" );

    std::ifstream file("config");
    if(file) {
        std::string str;
        while (std::getline(file, str)) {
            string ip;
            string port;
            string endCondition;
            string endConditionValue;
            string alarmType;
            string alarmValue;
            stringstream line;
            line << str;

            while (line.good()) {
                string substr;
                getline(line, substr, ',');
                if (ip.empty()) {
                    ip = substr;
                    ip.erase (std::remove (ip.begin(), ip.end(), ' '), ip.end());
                } else if (port.empty()) {
                    port = substr;
                    port.erase (std::remove (port.begin(), port.end(), ' '), port.end());
                } else if (endCondition.empty()) {
                    endCondition = substr;
                    endCondition.erase (std::remove (endCondition.begin(), endCondition.end(), ' '), endCondition.end());
                } else if (endConditionValue.empty()) {
                    endConditionValue = substr;
                    endConditionValue.erase (std::remove (endConditionValue.begin(), endConditionValue.end(), ' '), endConditionValue.end());
                } else if (alarmType.empty()) {
                    alarmType = substr;
                    alarmType.erase (std::remove (alarmType.begin(), alarmType.end(), ' '), alarmType.end());
                } else if (alarmValue.empty()) {
                    alarmValue = substr;
                    alarmValue.erase (std::remove (alarmValue.begin(), alarmValue.end(), ' '), alarmValue.end());
                } else break;

            }

            server->addToMeasurements(ip, port, endCondition, endConditionValue, alarmType, alarmValue);

        }

        file.close();
    } else {
        cout<<"No \"config\" file in program directory!"<<endl;
    }
    server->init();


    free(server);

    return 0;
}
