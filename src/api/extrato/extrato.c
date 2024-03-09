#include <stdio.h>
#include <libpq-fe.h>
#include <json-c/json.h>
#include "extrato.h"
#include "../../http/http.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include "../../server/server.h"
#include "../../utils/utils.h"

void extrato_route(const struct request_handler_t* request, char* response) {
    client_model_t client;
    balance_model_t balance;
    
    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

    struct pg_result* res = NULL;
    int ok = get_client_balances(&balance, m_conn, res, string_to_int(ext_uri_parameters[0]));
    printf("REQ PARAMETER: %d\n", string_to_int(ext_uri_parameters[0]));
    if (!ok) {
        const char* err_message = status_code_to_string(STATUS_INTERNAL_ERROR);
        build_http_response(response, header_buffer, STATUS_INTERNAL_ERROR, err_message);

        fprintf(stderr, "%s\n", PQerrorMessage(m_conn));

        free(header_buffer);

        return;    
    }

    build_http_response(response, header_buffer, STATUS_ACCEPTED, "accepted");

    free(header_buffer);
}