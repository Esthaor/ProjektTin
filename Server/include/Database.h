//
// Created by Aleksandra Ziegart on 30/05/16.
//

#ifndef PROJEKTTIN_DATABASE_H
#define PROJEKTTIN_DATABASE_H


#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;

class Database
{
public:
    Database();
    ~Database();

    sqlite3 *db;
    char *zErrMsg;
    int rc;
    const char *sql;
    const char* data;
    string sql_temp;
    ostringstream oss;
    static vector<string> selectAllWWWresult;

    bool open();
    void close();
    bool create_table();
    bool create_table_agents();
    int insert(int id_machine, int id_measurement, string status, int port, int alarm_value, int current_value);
    int insert_agents(int id_machine, string ip);
    int check_if_exists_agents(string ip);

    bool select_all();
    bool delete_row(int id_machine, int id_measurement);
    bool update(string status, int id_machine, int id_measurement);
    int select_ip(string ip);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    static int dallback(void *NotUsed, int argc, char **argv, char **azColName);
    static int callbackWWW(void *NotUsed, int argc, char **argv, char **azColName);

    string select_allWWW();
    static int agentExists;
    int check_exists_value();
    static string ipaddr;
    string check_ip_value();


};


#endif //PROJEKTTIN_DATABASE_H
