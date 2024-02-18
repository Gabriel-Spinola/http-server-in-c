#ifndef SERVER_H
#define SERVER_H
    #include <pthread.h>

    #define PORT 8080
    #define MAX_CONNECTION_QUEUE_SIZE 1000

    typedef int socket_t;
    typedef char* buffer_t;
    typedef struct {
        char *method,
             *uri,
             *query_parameter,
             *protype;

        char* payload;
        size_t payload_size;
    } request_handler_t;

    typedef struct {
        char* status_code;
        char* payload;
    } response_t;

    typedef struct {
        socket_t client_fd;
        request_handler_t request_handler;
    } client_thread_t;

    void* handle_client(void* client_socket_fd);
    void start_server();

    extern client_thread_t client_thread_data;
    extern void router(request_handler_t request, char* response);
    
    extern char* external_req_method;
    extern char* external_req_uri;

    #define ROUTE_START()       if (0) {
    #define ROUTE(METHOD, URI)  } else if (\
                                    strcmp(URI, external_req_uri) == 0 &&\
                                    strcmp(METHOD, external_req_method) == 0) {
    #define ROUTE_GET(URI)      ROUTE("GET", URI) 
    #define ROUTE_POST(URI)     ROUTE("POST", URI)
    #define ROUTE_PUT(URI)      ROUTE("PUT", URI) 
    #define ROUTE_PATCH(URI)    ROUTE("PATCH", URI)
    #define ROUTE_DELETE(URI)   ROUTE("DELETE", URI) 
    #define ROUTE_END()         } else printf(\
                                "HTTP/1.1 500 Not Handled\r\n\r\n" \
                                "The server has no handler to the request.\r\n" \
                                );
#endif /* SERVER_H */