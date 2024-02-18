#ifndef SERVER_H
#define SERVER_H
    #include <pthread.h>

    #define PORT 8080
    #define MAX_CONNECTION_QUEUE_SIZE 1000

    typedef int socket_t;
    typedef struct {
        char *method,
             *uri,
             *query_parameter,
             *body;

        char* payload;
        size_t payload_size;
        size_t body_size;
    } request_handler_t;

    //REVIEW - NOT IMPLEMENTED
    typedef struct {
        char* status_code;
        char* payload;
    } response_t;

    void* handle_client(void* client_socket_fd);
    void start_server();

    /// @brief Handles routing in the server
    /// @param request Request received on the client handler
    /// @param response Response pointer from client
    /// @note the router have the resposability to build the response that is going to be sent to the network
    extern void router(const request_handler_t* request, char* response);
    
    /// @brief helper variable (method) for the ROUTER macros
    extern char* external_req_method;

    /// @brief helper variable (uri) for the ROUTER macros
    extern char* external_req_uri;

    #define ROUTER_START()       if (0) {
    #define ROUTE(METHOD, URI)  } else if (\
                                    strcmp(URI, external_req_uri) == 0 &&\
                                    strcmp(METHOD, external_req_method) == 0) {
    #define ROUTE_GET(URI)      ROUTE("GET", URI) 
    #define ROUTE_POST(URI)     ROUTE("POST", URI)
    #define ROUTE_PUT(URI)      ROUTE("PUT", URI) 
    #define ROUTE_PATCH(URI)    ROUTE("PATCH", URI)
    #define ROUTE_DELETE(URI)   ROUTE("DELETE", URI) 
    #define NOT_FOUND()         } else 
#endif /* SERVER_H */