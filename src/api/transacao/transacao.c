#include "transacao.h"
#include "../../http/http.h"
#include "../../utils/utils.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include <stdio.h>
#include <json-c/json.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <libpq-fe.h>

static status_e set_transaction_fields_values(const struct request_handler_t* request, transaction_model_t* transaction);
static int handle_transaction(
    char type,
    struct pg_conn *conn,
    struct pg_result *res,
    int client_id,
    int value,
    const char *description
);

static const char* build_response_json_body(int limit, int balance) {
    json_object* response_object = json_object_new_object();

    json_object_object_add(response_object, "limite", json_object_new_int(limit));
    json_object_object_add(response_object, "saldo", json_object_new_int(balance));

    const char* json_string = json_object_to_json_string(response_object);

    char* output = malloc(strlen(json_string) + 1);
    if (output != NULL) {
        strcpy(output, json_string);
    }

    json_object_put(response_object);

    return output;
}

void transacao_route(const struct request_handler_t* request, char* response) {
    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
    if (header_buffer == NULL) {
        perror("Failed lo allocate memory for header buffer");

        return;
    }

    if (strlen(ext_uri_parameters[0]) > 1) {
        build_http_response(response,  header_buffer, STATUS_NOT_FOUND, "404 Not Found");

        free(header_buffer);
        return;
    }

    struct pg_result* res = NULL;
    int client_id = string_to_int(ext_uri_parameters[0]);
    int res_code = user_exists(m_conn, res, client_id);

    PQclear(res);

    if (res_code == -1) {
        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    if (res_code == 0) {
        build_http_response(response,  header_buffer, STATUS_NOT_FOUND, "404 Not Found");

        free(header_buffer);
        return;
    }

    balance_model_t balance;
    client_model_t client;
    
    transaction_model_t* transaction = malloc(sizeof(transaction_model_t));
    if (transaction == NULL) {
        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    status_e status = set_transaction_fields_values(request, transaction);
    if (status != STATUS_OK) {
        return build_error_response(response, header_buffer, status);
    }

    // TODO - Add into validation function
    if (transaction->type != 'c' && transaction->type != 'd') {
        fprintf(stderr, "Invalid transaction type\n");

        return build_error_response(response, header_buffer, STATUS_UNPROCESSABLE_ENTITY);
    }

    int ok = handle_transaction(
        transaction->type,
        m_conn,
        res,
        client_id,
        transaction->value,
        transaction->description
    );

    PQclear(res);
    if (!ok) {
        if (strlen(PQerrorMessage(m_conn)) != 0) {
            fprintf(stderr, "%s\n", PQerrorMessage(m_conn));
        }

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    // NOTE - Post-transaction values
    ok = get_client_balances(&balance, m_conn, res, client_id);
    PQclear(res);
    if (!ok) {    
        if (strlen(PQerrorMessage(m_conn)) != 0) {
            fprintf(stderr, "%s\n", PQerrorMessage(m_conn));
        }

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    ok = get_client_data(&client, m_conn, res, client_id);
    PQclear(res);
    if (!ok) {
        if (strlen(PQerrorMessage(m_conn)) != 0) {
            fprintf(stderr, "%s\n", PQerrorMessage(m_conn));
        }

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    const char* stringfied_response_body = build_response_json_body(client.limit, balance.value);
    build_http_response(response, header_buffer, STATUS_OK, stringfied_response_body);

    free(transaction);
    free(header_buffer);
}

static int handle_transaction(
    char type,
    struct pg_conn *conn,
    struct pg_result *res,
    int client_id,
    int value,
    const char *description
) {
    if (type == 'c') {
        return credit_from_client(conn, res, client_id, value, description);
    }

    return debit_from_client(conn, res, client_id, value, description);
}

static status_e set_transaction_fields_values(
    const struct request_handler_t* request,
    transaction_model_t* transaction
){
    json_object* field_value;
    json_object* body_json = json_tokener_parse(request->body);
    if (body_json == NULL) {
        fprintf(stderr, "Invalid JSON format\n");

        return STATUS_UNPROCESSABLE_ENTITY;
    }

    // Check for trailing characters after valid JSON
    const char* body_str = json_object_get_string(body_json);
    if (body_str[strlen(body_str) - 1] != '}') {
        fprintf(stderr, "Trailing characters after JSON object\n");
        json_object_put(body_json);
        
        return STATUS_UNPROCESSABLE_ENTITY;
    }


    // Get values from and assigns it to struct fields;
    // This one is int;
    if (json_object_object_get_ex(body_json, "valor", &field_value)) {
        if (!json_object_is_type(field_value, json_type_int)) {
            fprintf(stderr, "INVALID TYPE\n");

            return STATUS_UNPROCESSABLE_ENTITY;
        }

        transaction->value = json_object_get_int(field_value);
    }
    
    // This one is char;
    if (json_object_object_get_ex(body_json, "tipo", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");

            return STATUS_UNPROCESSABLE_ENTITY;
        }

        strncpy(&transaction->type, json_object_get_string(field_value), 1);
    }

    // This one is string;
    if (json_object_object_get_ex(body_json, "descricao", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");

            return STATUS_UNPROCESSABLE_ENTITY;
        }

        const char* description = json_object_get_string(field_value);
        if (description == NULL) {
            fprintf(stderr, "NULL");

            return STATUS_UNPROCESSABLE_ENTITY;
        }

        if (strlen(description) > 10) {
            fprintf(stderr, "TOO BIG");

            return STATUS_UNPROCESSABLE_ENTITY;
        }

        if (strlen(description) == 0 || strcmp(description, "\"\"") == 0) {
            fprintf(stderr, "Empty or invalid 'descricao' value\n");

            json_object_put(body_json);
            return STATUS_UNPROCESSABLE_ENTITY;
        }

        printf("description %s\n", description);

        strcpy(transaction->description, description);
    }

    char current_time[30]; 
    get_current_time(current_time);

    strcpy(transaction->done, current_time);

    return STATUS_OK;
}
