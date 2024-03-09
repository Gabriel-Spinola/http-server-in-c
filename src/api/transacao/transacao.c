#include "transacao.h"
#include "../../http/http.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include <stdio.h>
#include <json-c/json.h>
#include <memory.h>
#include <time.h>

void set_transaction_fields_values(const struct request_handler_t* request, transaction_model_t* transaction);

void transacao_route(const struct request_handler_t* request, char* response) {
    struct pg_conn* m_conn;
    struct pg_result* m_result;
    transaction_model_t* transaction = malloc(sizeof(transaction_model_t));
    // int* client_id = malloc((int64_t)ext_uri_parameters[0]);
    // printf("client_id: %i\n", *client_id);
    // Hard code for tests;
    int client_id = 3;
    
    set_transaction_fields_values(request, transaction);

    // Executes debit or credit basec on transaction->type;
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

    // Get current time
    time_t current_time;
    time(&current_time);

    // Convert current time to struct tm (time components)
    struct tm* time_info = gmtime(&current_time);

    if (time_info == NULL) {
        fprintf(stderr, "Failed to get time information\n");
        return;  // Return an error code
    }

    // Format the time
    char formatted_time[30];  // Adjust the size based on your needs
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%dT%H:%M:%S", time_info);

    // Get fractional seconds using microseconds
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    // Append fractional seconds to the formatted time
    snprintf(formatted_time + strlen(formatted_time), sizeof(formatted_time) - strlen(formatted_time),
             ".%06ldZ", ts.tv_nsec / 1000);

    // Print the formatted time
    printf("Formatted time: %s\n", formatted_time);
    strcpy(transaction->done, formatted_time);
    printf("Time value %s\n", transaction->done);
}