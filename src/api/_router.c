#include "../server/server.h"
#include "../http/http.h"
#include "../utils/utils.h"
#include "extrato/extrato.h"
#include "transacao/transacao.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

static void test_router(const struct request_handler_t* request, char* response);
static void print_req_debug(const struct request_handler_t* request);

void router(const struct request_handler_t* request, char* response) {
    ROUTER_START()

    ROUTE_GET("/clientes/:id/extrato") {
        #if DEBUG
            print_req_debug(request);
        #endif

        extrato_route(request, response);
    }

    ROUTE_POST("/clientes/:id/transacoes") {
        #if DEBUG
            print_req_debug(request);
        #endif

        transacao_route(request, response);
    }

    // ANCHOR - Test & Example routes
    ROUTE_GET("/ping/:id/:2d/oi") {
        #if DEBUG
            print_req_debug(request);
        #endif

        printf("\nGOT PARAMETER 1: %s\n", ext_uri_parameters[0]);
        printf("\nGOT PARAMETER 2: %s\n", ext_uri_parameters[1]);

        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response,  header_buffer, STATUS_OK, "pong");

        free(header_buffer);
    }

    ROUTE_POST("/") {
        test_router(request, response);
    }

    NOT_FOUND() {
        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response,  header_buffer, STATUS_NOT_FOUND, "404 Not Found");

        free(header_buffer);
    }
}

void test_router(const struct request_handler_t* request, char* response) {
    #if DEBUG
        print_req_debug(request);
    #endif

    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

    if (request->body == NULL) {
        const char* err_message = status_code_to_string(STATUS_BAD_REQUEST);
        build_http_response(response, header_buffer, STATUS_BAD_REQUEST, err_message);

        free(header_buffer);

        return;
    }

    json_object* body_json = json_object_new_object();
    body_json = json_tokener_parse(request->body);
    if (body_json == NULL) {
        const char* err_message = status_code_to_string(STATUS_INTERNAL_ERROR);
        build_http_response(response, header_buffer, STATUS_INTERNAL_ERROR, err_message);

        free(header_buffer);

        return;
    }

    // Extract and print data from given body
    json_object* field_value;
    if (json_object_object_get_ex(body_json, "field", &field_value)) {
        if (!json_object_is_type(field_value, json_type_int)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        printf("Field value: %d\n", json_object_get_int(field_value));
    }

    // json_object* string_data = json_object_new_string("hello");
    // json_object_object_add(body_json, "extra-data", string_data);

    const char* stringfied_response = json_object_to_json_string(body_json);
    build_http_response(response, header_buffer, STATUS_OK, stringfied_response);

    json_object_put(body_json);
    free(header_buffer);
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