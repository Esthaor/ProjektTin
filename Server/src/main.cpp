//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include "../include/Server.h"

using namespace std;

int main() {
    Server* server = new Server();

    string json = server->writeJson("start", 22, "threshold", 666, 0);
    server->sendJson(json);

    free(server);

    return 0;
}