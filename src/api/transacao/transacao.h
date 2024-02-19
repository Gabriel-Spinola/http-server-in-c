#ifndef TRANSACAO_H
#define TRANSACAO_H
    struct request_handler_t;
    struct response_t;

    void transacao_route(const struct request_handler_t* request, char* response);
#endif