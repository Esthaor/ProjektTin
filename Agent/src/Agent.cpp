//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Agent.h"

Agent::Agent() {

}

Agent::~Agent() {

}

bool Agent::receiveJson() {

    // Przykładowy JSON
    string json = "{\"measurement\":{\"status\":\"start\",\"port\":\"22\",\"endCondition\":\"threshold\",\"endConditionValue\":\"666\",\"alarmValue\":\"0\"}}";

    ptree root;
    istringstream is (json);
    read_json (is, root); //konwersja do ptree
    cout << root.get<string>("measurement.status") << endl; //odczyt przykładowego elementu

    return true;
}