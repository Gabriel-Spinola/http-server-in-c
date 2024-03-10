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

static void set_transaction_fields_values(const struct request_handler_t* request, transaction_model_t* transaction);
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
    balance_model_t balance;
    client_model_t client;

    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
    if (header_buffer == NULL) {
        perror("Failed to allocate memory for header buffer");

        return;
    }
    
    transaction_model_t* transaction = malloc(sizeof(transaction_model_t));
    if (transaction == NULL) {
        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }

    set_transaction_fields_values(request, transaction);
    
    struct pg_result* res = NULL;
    int client_id = string_to_int(ext_uri_parameters[0]);

    // TODO - Add into validation function
    if (transaction->type != 'c' && transaction->type != 'd') {
        fprintf(stderr, "Invalid transaction type\n");

        return build_error_response(response, header_buffer, STATUS_BAD_REQUEST);
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

static void set_transaction_fields_values(
    const struct request_handler_t* request,
    transaction_model_t* transaction
){
    json_object* body_json = json_tokener_parse(request->body);
    json_object* field_value;

    // Get values from and assigns it to struct fields;
    // This one is int;
    if (json_object_object_get_ex(body_json, "valor", &field_value)) {
        if (!json_object_is_type(field_value, json_type_int)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        transaction->value = json_object_get_int(field_value);
    }
    
    // This one is char;
    if (json_object_object_get_ex(body_json, "tipo", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        strncpy(&transaction->type, json_object_get_string(field_value), 1);
    }

    // This one is string;
    if (json_object_object_get_ex(body_json, "descricao", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        strcpy(transaction->description, json_object_get_string(field_value));
    }

    char current_time[30]; 
    get_current_time(current_time);

    // Print the formatted time
    strcpy(transaction->done, current_time);
}
