/// USED LINKS AND FONTS
/// https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739
/// https://www.geeksforgeeks.org/socket-programming-cc/
/// https://gist.github.com/retr00exe/3196b0a0de61ba6d46f09b1e49561a52

#include "server/server.h"
#include "utils/utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

int main() {
    start_server();

    return EXIT_SUCCESS;
}

// TODO - read url parameters like: /[id]
void router(request_handler_t request, char* response) {
    ROUTER_START()
    
    ROUTE_GET("/ping") {
        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response,  header_buffer, STATUS_OK, "pong");

        free(header_buffer);
    }

    ROUTE_POST("/") {
        json_object* json_response = json_object_new_object();
        json_object* string_data = json_object_new_string("data");

        json_object_object_add(json_response, "asdasdasd", string_data);
        const char* stringfied_response = json_object_to_json_string(json_response);

        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response, header_buffer, STATUS_OK, stringfied_response);

        free(header_buffer);
    }

    NOT_FOUND() {
        char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
        build_http_response(response,  header_buffer, STATUS_NOT_FOUND, status_code_to_string(STATUS_NOT_FOUND));

        free(header_buffer);
    }

    free(request.payload);
}
