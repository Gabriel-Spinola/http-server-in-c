/// USED LINKS AND FONTS
/// https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739
/// https://www.geeksforgeeks.org/socket-programming-cc/
/// https://gist.github.com/retr00exe/3196b0a0de61ba6d46f09b1e49561a52

#include "server/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    start_server();

    return 0;
}

void route(request_handler_t request) {
    printf("Starting router...");
    
    ROUTE_START()
        ROUTE_POST("/") {
            printf("\n\n===REQDATA====:\n");
            printf("\nmethod: %s", request.method);
            printf("\nuri: %s", request.uri);
            printf("\npayload_size: %s", request.payload_size);
            printf("\npayload: %s", request.payload);
            printf("\n\n===REQDATA====:\n");


            printf("\nFOUND ROUTE\n");
        }
    ROUTE_END()   
}