//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Agent.h"

int main() {
    std::cout << "Hello, World2!" << std::endl;
    Agent* agent = new Agent();

    agent->receiveJson();

    free(agent);

    return 0;
}
