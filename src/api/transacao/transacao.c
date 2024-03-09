#include "transacao.h"
#include "../../http/http.h"
#include "../../database/models/model.h"
#include "../../database/database.h"
#include <stdio.h>
#include <json-c/json.h>

void transacao_route(const struct request_handler_t* request, char* response) {
   struct pg_conn* m_conn;
   struct pg_result* m_result;
   transaction_model_t transaction;
   
    // response = request->body;
    json_object* response = json_object_new_object();
    body_json = json_tokener_parse(request->body);
    if (body_json == NULL) {
        const char* err_message = status_code_to_string(STATUS_INTERNAL_ERROR);
        build_http_response(response, header_buffer, STATUS_INTERNAL_ERROR, err_message);
        free(header_buffer);

        return;
    }
    json_object_object_get_ex(body_json, "field", &field_value)

   char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
   build_http_response(response,  header_buffer, STATUS_OK, "pong");
}

// void jsonify_data(char (*headers)[MAXCHAR], char (*bodies)[MAXCHAR]){
//     json_object* root = json_object_new_object();

//     for (int i = 0; i < MAXKEYS; i++){
//         json_object_object_add(root, headers[i], json_object_new_string(bodies[i]));
//     }

//     printf("\nJSON: %s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
//     json_object_put(root);
// }