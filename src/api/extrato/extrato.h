#ifndef EXTRATO_H
#define EXTRATO_H
    struct request_handler_t;
    struct response_t;

    void extrato_route(const struct request_handler_t* request, char* response);
#endif