#ifndef SERVER_H
#define SERVER_H
    #include <pthread.h>

    #define PORT 8080
    #define BUFFER_SIZE 1024*5
    #define MAX_CONNECTION_QUEUE_SIZE 1000

    typedef int socket_t;
    typedef struct {
        char *method,
             *uri,
             *query_parameter,
             *protype;

        char* payload;
        int payload_size;
    } request_handler_t;

    void* handle_client(void* client_socket_fd);
    void start_server();

    void build_http_response(
        const char* file_name,
        const char* file_extension,
        char* response,
        size_t* response_length
    );

    void route(request_handler_t req);
/*
    // some interesting macro for `route()`
    #define ROUTE_START()       if (0) {
    #define ROUTE(METHOD, URI)  } else if (strcmp(URI, uri) == 0 && strcmp(METHOD, method) == 0) {
    #define ROUTE_GET(URI)      ROUTE("GET", URI) 
    #define ROUTE_POST(URI)     ROUTE("POST", URI) 
    #define ROUTE_END()         } else printf(\
                                    "HTTP/1.1 500 Not Handled\r\n\r\n" \
                                    "The server has no handler to the request.\r\n" \
                                );*/
#endif /* SERVER_H */