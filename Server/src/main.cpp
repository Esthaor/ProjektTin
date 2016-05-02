//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include "../include/Server.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    Server* server = new Server();

    server->sendJson();

    free(server);

    return 0;
}