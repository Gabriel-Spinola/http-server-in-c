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
   
   response = request->body;
   char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

   build_http_response(response,  header_buffer, STATUS_OK, "pong");
}

void jsonify_data(char (*headers)[MAXCHAR], char (*bodies)[MAXCHAR]){
    json_object* root = json_object_new_object();

    for (int i = 0; i < MAXKEYS; i++){
        json_object_object_add(root, headers[i], json_object_new_string(bodies[i]));
    }

    printf("\nJSON: %s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    json_object_put(root);
}