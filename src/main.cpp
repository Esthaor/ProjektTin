#include <iostream>
#include "../include/Server.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    Server* server = new Server();

    server->sendJson();

    return 0;
}