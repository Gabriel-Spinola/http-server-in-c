#ifndef SERVER_H
#define SERVER_H
    #include <pthread.h>

    #define PORT 8080
    #define MAX_CONNECTION_QUEUE_SIZE 10000

    typedef int socket_t;
    struct request_handler_t;
    struct response_t;

    void* handle_client(void* client_socket_fd);
    void start_server();

    /// @brief Handles routing in the server
    /// @param request Request received on the client handler
    /// @param response Response pointer from client
    /// @note the router have the resposability to build the response that is going to be sent to the network
    extern void router(const struct request_handler_t* request, char* response);
    extern int check_route(const char* method, const char* uri);

    #define ROUTER_START()        if (0) {
    #define ROUTE(METHOD, URI)    } else if (\
                                    check_route(METHOD, URI)) {
    #define ROUTE_GET(URI)         ROUTE("GET", URI) 
    #define ROUTE_POST(URI)        ROUTE("POST", URI)
    #define ROUTE_PUT(URI)         ROUTE("PUT", URI) 
    #define ROUTE_PATCH(URI)       ROUTE("PATCH", URI)
    #define ROUTE_DELETE(URI)      ROUTE("DELETE", URI) 
    #define NOT_FOUND()            } else 
#endif /* SERVER_H */
