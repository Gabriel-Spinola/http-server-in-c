#ifndef TRANSACAO_H
#define TRANSACAO_H
    struct request_handler_t;
    struct response_t;

    void transacao_route(const struct request_handler_t* request, char* response);

    #define MAXCHAR 50
    #define MAXKEYS 3

    void jsonify_data(char (*headers)[], char (*bodies)[]);
#endif