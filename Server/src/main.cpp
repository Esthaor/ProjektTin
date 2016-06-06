//
// Created by Michal Lipinski on 02.05.16.
//

#include <iostream>
#include "../include/Server.h"

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

    string json = server->writeJson("start", 22, "threshold", 666, 0);
    server->sendJson(json);

    free(server);

    return 0;
}
