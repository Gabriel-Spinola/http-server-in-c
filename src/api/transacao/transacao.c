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

void set_transaction_fields_values(const struct request_handler_t* request, transaction_model_t* transaction);

void transacao_route(const struct request_handler_t* request, char* response) {
    transaction_model_t* transaction = malloc(sizeof(transaction_model_t));
    // int* client_id = malloc((int64_t)ext_uri_parameters[0]);
    // printf("client_id: %i\n", *client_id);
    // Hard code for tests;
    int client_id = 3;

    // TA PRONTIN TA FEITO VIADO 1!1!
    // as requests tao funcionando normalmente
    // so ta faltando tratar os parametos do ext_uri_parameters
    // acho uma boa fazer um ponteiro apontando pro array ja q ele ja ta no 
    // server.c; provavelmente vo ta no 3 sono quando ce tiver lendo; flw/
    // https://prnt.sc/GOyukD9Fl4_d
    
    set_transaction_fields_values(request, transaction);
    free(transaction);

    // Executes debit or credit based on transaction->type;
    // transaction->type == 'c' ? credit_from_client(m_conn, m_result, client_id, transaction->value, transaction->description) 
    // : 
    // debit_from_client(m_conn, m_result, client_id, transaction->value, transaction->description); 

    // Pass body as response for test purposes;
    strcpy(response, request->body);
    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
    build_http_response(response, header_buffer, STATUS_OK, "tes tes");
}

void set_transaction_fields_values(const struct request_handler_t* request, transaction_model_t* transaction){
    json_object* body_json = json_tokener_parse(request->body);
    json_object* field_value;

    // Get values from and assigns it to struct fields;
    // This one is int;
    if (json_object_object_get_ex(body_json, "valor", &field_value)) {
        if (!json_object_is_type(field_value, json_type_int)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        transaction->value = json_object_get_int(field_value);
        printf("Value value: %d\n", transaction->value);
    }
    
    // This one is char;
    if (json_object_object_get_ex(body_json, "tipo", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        strncpy(&transaction->type, json_object_get_string(field_value), 1);
        printf("Type value: %c\n", transaction->type);
    }

    // This one is string;
    if (json_object_object_get_ex(body_json, "descricao", &field_value)) {
        if (!json_object_is_type(field_value, json_type_string)) {
            fprintf(stderr, "INVALID TYPE\n");
        }
        
        strcpy(transaction->description, json_object_get_string(field_value));
        printf("Description value: %s\n", transaction->description);
    }

    char formatted_time[30]; 
    get_current_time(formatted_time);

    // Print the formatted time
    printf("Formatted time: %s\n", formatted_time);
    strcpy(transaction->done, formatted_time);
    printf("Time value %s\n", transaction->done);
}