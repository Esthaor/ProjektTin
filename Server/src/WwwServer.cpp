//
// Created by grzegorz on 07.06.16.
//

#include "WwwServer.h"
#include "Server.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
Database* WwwServer::database;

void WwwServer::handle_add_measurment(struct mg_connection *nc, struct http_message *hm) {
    char ip[100], port[100], type[100], conVal[100], alVal[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "ip", ip, sizeof(ip));
    mg_get_http_var(&hm->body, "port", port, sizeof(port));
    mg_get_http_var(&hm->body, "type", type, sizeof(type));
    mg_get_http_var(&hm->body, "conVal", conVal, sizeof(conVal));
    mg_get_http_var(&hm->body, "alVal", alVal, sizeof(alVal));

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    string sIp = ip;
    string sPort = port;
    string sType = type;
    string sConVal = conVal;
    string sAlVal = alVal;
    int iPort = stoi(sPort);
    int iConVal = stoi(sConVal);
    int iAlVal = stoi(sAlVal);

    Server::sendMeasurement(sIp, Server::writeJson("start", iPort, sType, iConVal, iAlVal));

    // TODO: Dodać tutaj dodawanie pomiaru z danych z jsona

    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::handle_add_machine(struct mg_connection *nc, struct http_message *hm) {
    char ip[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "ip", ip, sizeof(ip));

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    // TODO: Dodać tutaj dodawanie maszyny z danych z jsona

    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::handle_end_measurement(struct mg_connection *nc, struct http_message *hm) {
    char ip[100], id[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "ip", ip, sizeof(ip));
    mg_get_http_var(&hm->body, "id", id, sizeof(id));

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    string sIp = ip;
    string sId = id;
    int iId = stoi(sId);
    Server::sendMeasurement(sIp, Server::writeSmallJson("stop", iId));


    // TODO: Dodać tutaj kończenie pomiaru



    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::handle_edit_measurement(struct mg_connection *nc, struct http_message *hm) {
    char ip[100],id[100], port[100], type[100], conVal[100], alVal[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "ip", ip, sizeof(ip));
    mg_get_http_var(&hm->body, "id", id, sizeof(ip));
    mg_get_http_var(&hm->body, "port", port, sizeof(port));
    mg_get_http_var(&hm->body, "type", type, sizeof(type));
    mg_get_http_var(&hm->body, "conVal", conVal, sizeof(conVal));
    mg_get_http_var(&hm->body, "alVal", alVal, sizeof(alVal));

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    // TODO: Dodać tutaj edycje pomiaru z danych z jsona

    string sIp = ip;
    string sPort = port;
    string sType = type;
    string sConVal = conVal;
    string sAlVal = alVal;
    int iPort = stoi(sPort);
    int iConVal = stoi(sConVal);
    int iAlVal = stoi(sAlVal);

    Server::sendMeasurement(sIp, Server::writeJson("change", iPort, sType, iConVal, iAlVal));

    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::handle_print_list(struct mg_connection *nc, struct http_message *hm) {
    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    /* Compute the result and send it back as a JSON object */
    string response = "{\"data\":";

    string data = database->select_allWWW();

    response += "\"<table><tbody><tr><th>IP</th><th>Measurement ID</th><th>Status</th><th>Port</th><th>Alarm value</th><th>Current value</th><th>Date</th></tr>" + data + "</tbody></table>\"}";
    const char *cresponse = response.c_str();
    mg_printf_http_chunk(nc, cresponse);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::handle_print_ips(struct mg_connection *nc, struct http_message *hm) {
    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    /* Compute the result and send it back as a JSON object */
    string response = "{\"data\":";

    vector<string> ips = database->select_allAgentsWWW();
    response+="\"";
    int i = 0;
    int size = ips.size();
    for (i=0; i<size;i++){
        string temp = ips.at(i);
        response+="<option>" + temp + "</option>";
    }

    response += "\"}";
    const char *cresponse = response.c_str();
    mg_printf_http_chunk(nc, cresponse);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;

    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/addmeas") == 0) {
                handle_add_measurment(nc, hm); /* Handle RESTful call */
            } else if (mg_vcmp(&hm->uri, "/api/v1/lista") == 0) {
                handle_print_list(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/addmach") == 0) {
                handle_add_machine(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/endmeas") == 0) {
                handle_end_measurement(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/editmeas") == 0) {
                handle_edit_measurement(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/agents") == 0){
                handle_print_ips(nc, hm);
            }
            else {
                mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
            }

            break;
        default:
            break;
    }
}

int WwwServer::wwwServerStart()
{
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr, NULL);

    s_http_server_opts.document_root = ".";

    /* Set HTTP server options */
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s\n", s_http_port);
        exit(1);
    }
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.enable_directory_listing = "yes";
    printf("Starting RESTful server on port %s, serving %s\n", s_http_port, s_http_server_opts.document_root);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}