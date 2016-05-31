//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include "../include/Server.h"
#include "../include/Database.h"

using namespace std;

int main()
{
/*
    Database* database = new Database();
    database->open();
    database->create_table();
    database->insert(73488, 111, "zakonczony", 12344, 1234, 764 );
    database->select_all();
    database->update( "w realizacji" ,73488, 111);
    database->select_all();
   database->delete_row(73488, 111);


    database->close();
    */

    Server* server = new Server();

    string json = server->writeJson("start", 22, "threshold", 666, 0);
    server->sendJson(json);

    free(server);

    return 0;
}
