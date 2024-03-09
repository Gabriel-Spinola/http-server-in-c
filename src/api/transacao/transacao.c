#include "transacao.h"
#include "../../http/http.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include <stdio.h>
#include <json-c/json.h>
#include <memory.h>

void transacao_route(const struct request_handler_t* request, char* response) {
    struct pg_conn* m_conn;
    struct pg_result* m_result;
    transaction_model_t transaction;

    char* body_copy;
    memcpy(body_copy, request->body, sizeof(request->body));
    json_object* body_json = json_tokener_parse(body_copy);
    json_object* field_value;
    if (json_object_object_get_ex(body_json, "VALORZAO DE TESTE", &field_value)) {
        if (!json_object_is_type(field_value, json_type_int)) {
            fprintf(stderr, "INVALID TYPE\n");
        }

        printf("Field value: %d\n", json_object_get_int(field_value));
    }

    // stringfies response;
    response = request->body;
    const char* stringfied_response = json_object_to_json_string(body_json);
    printf("body json: %s\n", body_json);
    printf("stringfied response: %s\n", stringfied_response);

    char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
    build_http_response(response, header_buffer, STATUS_OK, "pong");
}

// void jsonify_data(char (*headers)[MAXCHAR], char (*bodies)[MAXCHAR]){
//     json_object* root = json_object_new_object();

//     for (int i = 0; i < MAXKEYS; i++){
//         json_object_object_add(root, headers[i], json_object_new_string(bodies[i]));
//     }

//     printf("\nJSON: %s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
//     json_object_put(root);
// }