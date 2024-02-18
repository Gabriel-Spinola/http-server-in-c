#include "../utils/utils.h"
#include "server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <pthread.h>
#include <regex.h>
#include <sys/stat.h>
#include <fcntl.h>

char* external_req_method;
char* external_req_uri;

void start_server() {
    print_title();

    struct sockaddr_in address;
    socket_t server_fd;
    int option = 1;

    socklen_t address_length = sizeof(address);

    // Create new socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed\n");

        exit(EXIT_FAILURE);
    }

    // Setup socket options
    if (setsockopt(
        server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &option, sizeof(option)
    )) {
        perror("Failed to set socket option\n");

        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the port 8080
    if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("Failed to bind server\n");

        exit(EXIT_FAILURE);
    }

    // Listen to server
    // REVIEW - __n value might not be the correct for the task
    if (listen(server_fd, MAX_CONNECTION_QUEUE_SIZE) < 0) {
        perror("Failed to listen\n");

        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    while (1) {
        // Client info
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        socket_t* client_fd = malloc(sizeof(socket_t));

        *client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_address_length);
        if (client_fd < 0) {
            perror("Failed to acceppet client connection\n");

            continue;
        }

        // Create new thread to handle client request
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*) client_fd);
        pthread_detach(thread_id);
    }

    close(server_fd);
}

void* handle_client(void* client_socket_fd) {
    request_handler_t req;

    // NOTE - Cast the void pointer into the correct type
    socket_t client_fd = *((socket_t*) client_socket_fd);
    char* buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

    // Receive request data
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        fprintf(stderr, "Failed to read client buffer\n");
    }

    if (bytes_received == 0) {
        fprintf(stderr, "Client disconnected unexpectedly\n");
    }

    // NOTE - At this point the buffer is storing all the request data 
    req.payload_size = 0;
    req.payload = (char*) malloc(bytes_received + 1);
    if (req.payload == NULL) {
        fprintf(stderr, "Failed to allocate memory for request payload\n");

        exit(EXIT_FAILURE);
    }

    req.payload = memcpy(req.payload, buffer, bytes_received);
    // FIXME - Can't get the payload size, it's throwing a seg fault 
    // req.payload_size = (size_t) bytes_received;

    if (bytes_received > 0) {
        regex_t regex;
        regmatch_t matches[2];

        regcomp(&regex, "^(GET|POST|PUT|DELETE) /([^ ]*) HTTP/1", REG_EXTENDED);

        // NOTE - Mutate buffer into method
        if (regexec(&regex, buffer, 2, matches, 0) == 0) {
            // get file name
            buffer[matches[1].rm_eo] = '\0';

            external_req_method = buffer;
            external_req_uri = "/";            

            req.method = external_req_method;

            // Build http response
            char* response = (char*) malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_length;

            // build_http_response("filename", "json", response, &response_length);
            router(req, response);
            response_length = strlen(response);

            // Send HTTP response to client
            send(client_fd, response, response_length, 0);

            fflush(stdout);
            free(response);
        }

        regfree(&regex);
    }

    close(client_fd);
    free(buffer);

    return NULL;
}
