/// USED LINKS AND FONTS
/// https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739
/// https://www.geeksforgeeks.org/socket-programming-cc/
/// https://gist.github.com/retr00exe/3196b0a0de61ba6d46f09b1e49561a52

#include "server/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

int main() {
    start_server();

    return 0;
}

// https://github.com/pedro-vicente/lib_netsockets/blob/master/examples/json_client.cc
// https://stackoverflow.com/questions/32667109/send-and-receive-json-over-sockets-in-server-and-client-application-in-c
void route(request_handler_t request, char* response) {
    printf("Starting router...");

    ROUTE_START()
        ROUTE_POST("/") {
            printf("\n\n===REQDATA====\n");
            printf("\nmethod: %s", request.method);
            printf("\nuri: %s", request.uri);
            printf("\npayload_size: %s", request.payload_size);
            printf("\npayload: %s", request.payload);
            printf("\n\n===REQDATA====\n");

            json_object* json_response = json_object_new_object();
            json_object* string_data = json_object_new_string("data");

            json_object_object_add(json_response, "asdasdasd", string_data);
            const char* stringfied_response = json_object_to_json_string(json_response);

            char* header_buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
            snprintf(header_buffer, BUFFER_SIZE,
                "HTTP/1.1 200 OK\r\n"
                "Server: webserver-c"
                "Content-type: application/json\r\n"
                "\r\n"
            );

            snprintf(response, BUFFER_SIZE, "%s%s\r\n", header_buffer, stringfied_response);

            free(header_buffer);
        }

        ROUTE_GET("/cu") {

        }
    ROUTE_END()   

    free(request.payload);
}