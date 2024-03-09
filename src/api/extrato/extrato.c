#include <stdio.h>
#include <libpq-fe.h>
#include <json-c/json.h>
#include <string.h>
#include "extrato.h"
#include "../../http/http.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include "../../server/server.h"
#include "../../utils/utils.h"

static const char* build_response_json_body(
    int total,
    char extract_time[30],
    int limit,
    transaction_model_t* last_transactions[10]
) {
    json_object* response_object = json_object_new_object();
    json_object* balance_object = json_object_new_object();
    json_object* last_transactions_array = json_object_new_array();

    // Construct balance object
    json_object_object_add(balance_object, "total", json_object_new_int(total));
    json_object_object_add(balance_object, "data_extrato", json_object_new_string(extract_time));
    json_object_object_add(balance_object, "limite", json_object_new_int(limit));

    // Construct last transactions array 
    for (int i = 0; i < 10; i++) {
        if (last_transactions[i] == NULL) {
            break;
        }

        json_object* transaction_object = json_object_new_object();

        transaction_model_t* current = last_transactions[i];

        char type_str[2];
        sprintf(type_str, "%c", current->type);

        json_object_object_add(transaction_object, "valor", json_object_new_int(current->value));
        json_object_object_add(transaction_object, "tipo", json_object_new_string(type_str));
        json_object_object_add(transaction_object, "descricao", json_object_new_string(current->description));
        json_object_object_add(transaction_object, "realizada_em", json_object_new_string(current->done));

        json_object_array_add(last_transactions_array, transaction_object);
    }

    // Add balance & last_transactions objects to response object
    json_object_object_add(response_object, "saldo", balance_object);
    json_object_object_add(response_object, "ultimas_transacoes", last_transactions_array);

    const char* json_string = json_object_to_json_string(response_object);

    char* output = malloc(strlen(json_string) + 1);
    if (output != NULL) {
        strcpy(output, json_string);
    }

    json_object_put(response_object);

    return output;
}

void extrato_route(const struct request_handler_t* request, char* response) {
    client_model_t client;
    balance_model_t balance;
    transaction_model_t* last_transactions[10] = { NULL };

    int client_id = string_to_int(ext_uri_parameters[0]);
    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

    struct pg_result* res = NULL;
    int ok = get_client_balances(&balance, m_conn, res, client_id);
    if (!ok) {
        PQclear(res);

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }
    PQclear(res);

    printf("Balance: %d | %d | %d\n", balance.id, balance.client_id, balance.value);

    ok = get_client_data(&client, m_conn, res, client_id);
    if (!ok) {
        PQclear(res);

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }
    PQclear(res);

    char extract_time[30];
    get_current_time(extract_time);

    ok = get_client_transactions(last_transactions, m_conn, res, client_id);
    if (!ok) {
        PQclear(res);

        return build_error_response(response, header_buffer, STATUS_INTERNAL_ERROR);
    }
    PQclear(res);

    if (last_transactions[0] != NULL) {
        for (int i = 0; i < 10; i++) {
            if (last_transactions[i] == NULL) {
                continue;
            }

            printf("GOT TRANSACTION: %s\n", last_transactions[i]->id);
        }
    }

    const char* stringfied_response = build_response_json_body(balance.value, extract_time, client.limit, last_transactions);
    build_http_response(response, header_buffer, STATUS_ACCEPTED, stringfied_response);

    free(header_buffer);
}