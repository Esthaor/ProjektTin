//
// Created by Aleksandra Ziegart on 30/05/16.
//

#include "../include/Database.h"


/* g++ Server/include/Database.h Server/include/Server.h Server/include/Socket.h Server/src/Database.cpp Server/src/Server.cpp Server/src/Socket.cpp Server/src/main.cpp -lboost_system -lboost_thread -lpcap -lsqlite3
*/

vector<string> Database::selectAllWWWresult;
int Database::agentExists=-1;
string Database::mid;

Database::Database()
{

}

Database::~Database()
{

}
bool Database::open()
{
    zErrMsg = 0;

    rc =  sqlite3_open("database.db", &db);


    if( rc )
    {
        cout << "Can't open database" << endl;
        return false;
    }
    else{
        cout << "Opened database successfully" << endl;
    }

    return true;
}
void Database::close()
{
    sqlite3_close(db);
}
bool Database::create_table()
{
        zErrMsg = 0;
        data = "Callback function called";


        /* Create SQL statement */
        sql = "CREATE TABLE STATISTICS("  \
         "ID_MACHINE          INT    NOT NULL," \
         "ID_MEASUREMENT            INT     NOT NULL," \
         "STATUS        TEXT," \
         "PORT        INT," \
         "ALARM_VALUE        INT," \
         "CURRENT_VALUE        INT," \
         "DATETIME        TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL," \
         "PRIMARY KEY (ID_MACHINE, ID_MEASUREMENT));";


        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
        else
        {
            fprintf(stdout, "Table STATISTICS created successfully\n");
        }

    return true;
}

bool Database::create_table_agents()
{
    zErrMsg = 0;
    data = "Callback function called";


    /* Create SQL statement */
    sql = "CREATE TABLE AGENTS("  \
         "ID_MACHINE          INT    NOT NULL," \
         "IP            TEXT     NOT NULL," \
         "FOREIGN KEY(ID_MACHINE) REFERENCES STATISTICS(ID_MACHINE));";


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "Table AGENTS created successfully\n");
    }

    return true;
}

int Database::insert(int id_machine, int id_measurement, string status, int port, int alarm_value, int current_value)
{
    oss << "" << id_machine;
    string s_id_machine= oss.str();
    oss.str("");

    oss << "" << id_measurement;
    string s_id_measurement = oss.str();
    oss.str("");

    oss << "" << port;
    string s_port = oss.str();
    oss.str("");

    oss << "" << alarm_value;
    string s_alarm_value = oss.str();
    oss.str("");

    oss << "" << current_value;
    string s_current_value= oss.str();
    oss.str("");

   /* Create SQL statement */
    sql_temp =  "INSERT INTO STATISTICS (ID_MACHINE,ID_MEASUREMENT,STATUS,PORT,ALARM_VALUE,CURRENT_VALUE) "  \
         "VALUES ( " + s_id_machine + ", " + s_id_measurement + ", '" + status + "', " + s_port + ", " + s_alarm_value + ", " + s_current_value + " );";


    sql = sql_temp.c_str();


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        fprintf(stdout, "Record in table STATISTICS created successfully\n");
    }


    return 1;
}

int Database::insert_agents(int id_machine, string ip)
{
    oss << "" << id_machine;
    string s_id_machine= oss.str();
    oss.str("");

    oss << "" << ip;
    string s_ip = oss.str();
    oss.str("");

    /* Create SQL statement */
    sql_temp =  "INSERT INTO AGENTS "  \
         "VALUES ( '" + s_id_machine + "', '" + s_ip + "' );";


    sql = sql_temp.c_str();


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else
    {
        fprintf(stdout, "Record in table AGENTS created successfully\n");
    }


    return 1;
}

int Database::select_ip(string ip)
{
    fprintf(stdout, "\n ### DATABASE: ###\n");

    /* Create SQL statement */
    sql_temp = "SELECT ID_MACHINE from AGENTS where IP = '" + ip + "';";
    sql = sql_temp.c_str();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, dallback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return 0;
    }
    else
    {
        fprintf(stdout, "Select done successfully\n");
    }

}

bool Database::select_all()
{

    fprintf(stdout, "\n ### DATABASE: ###\n");

    /* Create SQL statement */
    sql = "SELECT * from STATISTICS";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "Select done successfully\n");
    }


return true;
}

int Database::check_if_exists_agents(string ip)
{

    fprintf(stdout, "\n ### DATABASE: ###\n");

    /* Create SQL statement */
    sql_temp = "SELECT EXISTS(SELECT 1 FROM AGENTS WHERE IP='" + ip + "');";

    sql = sql_temp.c_str();
    char* exists = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, &exists, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    else {

        return 1;
    }
}


bool Database::delete_row(int id_machine, int id_measurement)
{
    zErrMsg = 0;
    data = "Callback function called";

    oss << "" << id_machine;
    string s_id_machine = oss.str();
    oss.str("");

    oss << "" << id_measurement;
    string s_id_measurement = oss.str();
    oss.str("");


    /* Create merged SQL statement */
    sql_temp = "DELETE from STATISTICS where ID_MACHINE = " + s_id_machine + " AND ID_MEASUREMENT = " + s_id_measurement + "; ";

    sql = sql_temp.c_str();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }

    return true;
}

bool Database::update(string status, int id_machine, int id_measurement, int alarmValue, int currentValue)
{
    zErrMsg = 0;
    data = "Callback function called";

    oss << "" << id_machine;
    string s_id_machine = oss.str();
    oss.str("");

    oss << "" << id_measurement;
    string s_id_measurement = oss.str();
    oss.str("");

    oss << "" << alarmValue;
    string s_alarmValue = oss.str();
    oss.str("");

    oss << "" << id_measurement;
    string s_currentValue = oss.str();
    oss.str("");



    /* Create merged SQL statement */
    sql_temp = "UPDATE STATISTICS set STATUS = '" + status + "', ALARM_VALUE = " + s_alarmValue + ", CURRENT_VALUE = " + s_currentValue + "  where ID_MACHINE = " + s_id_machine + " AND ID_MEASUREMENT = " + s_id_measurement + " ; ";
    sql = sql_temp.c_str();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
}


int Database::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    //ipaddr = argv[i];

    agentExists = stoi(argv[0]);
    return 0;

}

int Database::dallback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    mid = argv[0];

    //agentExists = stoi(argv[0]);
    return 0;

}
int Database::callbackWWW(void *NotUsed, int argc, char **argv, char **azColName)
{
    string toReturn="<tr>";
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        if (argv[i] != NULL)
            toReturn += "<td>" + (string)argv[i] + "</td>";
        else
            toReturn += "<td> NULL </td>";
    }
    toReturn += "</tr>";
    selectAllWWWresult.push_back(toReturn);
    return 0;
}

string Database::select_allWWW()
{
    selectAllWWWresult.clear();
    /* Create SQL statement */
    sql = "SELECT IP, ID_MEASUREMENT, STATUS, PORT, ALARM_VALUE, CURRENT_VALUE, DATETIME from STATISTICS, AGENTS where AGENTS.ID_MACHINE = STATISTICS.ID_MACHINE";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callbackWWW, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return NULL;
    }
    else
    {
        fprintf(stdout, "Select * done successfully\n");
    }

    string s;
    s = accumulate(begin(selectAllWWWresult), end(selectAllWWWresult), s);

    return s;
}

int Database::check_exists_value()
{
    int result = agentExists;
    agentExists = -1;
    return result;
}

string Database::check_machine_id()
{
    string result = mid;
    mid.clear();
    return result;
}