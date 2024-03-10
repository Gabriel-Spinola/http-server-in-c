#include "../server/server.h"
#include "../http/http.h"
#include "../utils/utils.h"
#include "extrato/extrato.h"
#include "transacao/transacao.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
#include "../database/models/model.h"
#include <libpq-fe.h>
#include "../database/database.h"

static void print_req_debug(const struct request_handler_t* request);

void router(const struct request_handler_t* request, char* response) {
    ROUTER_START()

    ROUTE_GET("/clientes/:id/extrato") {
        #ifndef DEV
            print_req_debug(request);
        #endif

        extrato_route(request, response);
    }

    ROUTE_POST("/clientes/:id/transacoes") {
        #ifndef DEV
            print_req_debug(request);
        #endif

        transacao_route(request, response);
    }

    NOT_FOUND() {
        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response,  header_buffer, STATUS_NOT_FOUND, "404 Not Found");

        free(header_buffer);
    }
}

void print_req_debug(const struct request_handler_t* request) {
    printf("\n\n===REQDATA====\n");
    printf("\nmethod: %s", request->method);
    printf("\nuri: %s", request->uri);
    printf("\npayload_size: %zu", request->payload_size);
    printf("\npayload: %s", request->payload);
    printf("\nbody: %s", request->body);
    printf("\nbody_size: %zu", request->body_size);
    printf("\n\n===REQDATA====\n");
}
