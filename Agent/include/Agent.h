//
// Created by Michal Lipinski on 02.05.16.
//

#ifndef PROJECT_AGENT_H
#define PROJECT_AGENT_H

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;

class Agent {
public:
    Agent();
    ~Agent();

    bool receiveJson();

};


#endif //PROJECT_AGENT_H
