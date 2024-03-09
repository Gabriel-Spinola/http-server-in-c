#include "extrato.h"
#include "../../http/http.h"
#include <stdio.h>
#include "../../database/models/model.h"

void extrato_route(const struct request_handler_t* request, char* response) {
    client_model_t client;
    transaction_model_t transaction;
    struct pg_conn* m_conn;
    struct pg_result* res;

    
}