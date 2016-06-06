//
// Created by grzegorz on 07.06.16.
//

#include "WwwServer.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

void WwwServer::handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
    char n1[100], n2[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
    mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    /* Compute the result and send it back as a JSON object */
    result = strtod(n1, NULL) + strtod(n2, NULL);
    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void WwwServer::ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;

    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/sum") == 0) {
                handle_sum_call(nc, hm); /* Handle RESTful call */
            } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
                char buf[100] = {0};
                memcpy(buf, hm->body.p,
                       sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
                printf("%s\n", buf);
            } else {
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
    char *cp;

    mg_mgr_init(&mgr, NULL);

    *cp = '\0';
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