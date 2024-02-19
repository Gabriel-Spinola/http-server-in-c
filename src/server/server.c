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
#include <sys/stat.h>
#include <fcntl.h>

static void extract_method_and_uri(const char* buffer, char** method, char** uri);
static void extract_request_body(const char* payload, size_t payload_size, char** body, size_t* body_size);

char* external_req_method;
char* external_req_uri;
char parameters[MAX_REQUEST_PARAMETERS][MAX_REQ_PARAMETER_SIZE];

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

        close(client_fd);
        free(buffer);

        return NULL;
    }

    if (bytes_received == 0) {
        fprintf(stderr, "Client disconnected unexpectedly\n");

        close(client_fd);
        free(buffer);

        return NULL;
    }

    req.payload_size = 0;
    req.payload = (char*) malloc(bytes_received + 1);
    if (req.payload == NULL) {
        fprintf(stderr, "Failed to allocate memory for request payload\n");

        exit(EXIT_FAILURE);
    }

    memcpy(req.payload, buffer, bytes_received);
    req.payload_size = (size_t) bytes_received;

    extract_method_and_uri(buffer, &external_req_method, &external_req_uri);
    req.method = external_req_method;
    req.uri = external_req_uri;

    req.body_size = 0;
    extract_request_body(req.payload, req.payload_size, &req.body, &req.body_size);

    // Build http response
    char* response = (char*) malloc(BUFFER_SIZE * 2 * sizeof(char));
    size_t response_length;

    router(&req, response);
    response_length = strlen(response);

    // Send HTTP response to client
    send(client_fd, response, response_length, 0);

    close(client_fd);

    fflush(stdout);

    free(req.payload);
    free(response);
    free(buffer);

    if (req.body != NULL) {
        free(req.body);
    }

    return NULL;
}

int check_route(const char* method, const char* uri) {
    const char* param_start = strchr(uri, ':');
    if (param_start == NULL) {
        return strcmp(method, external_req_method) == 0 &&
               strcmp(uri, external_req_uri) == 0;
    }

    if (!strcmp(method, external_req_method) == 0) {
        return 0;
    }

    const char* delimiter = "/";

    int route_tok_count;
    int req_tok_count; 
    char** route_tokens = split(uri, delimiter, &route_tok_count);
    char** request_tokens = split(external_req_uri, delimiter, &req_tok_count);

    // if both tokens 
    if (route_tok_count != req_tok_count) {
        return 0;
    }
    
    int param_count = 0;
    for (size_t i = 0; i < route_tok_count; i++) {
        const char* param = strchr(route_tokens[i], ':');

        printf("%d", !strcmp(route_tokens[i], request_tokens[i]) == 0);

        if (param != NULL) {
            strcpy(parameters[param_count], route_tokens[i] + 1);

            param_count++;
        }
        else if (!strcmp(route_tokens[i], request_tokens[i]) == 0) {
            free(route_tokens[i]);
            free(request_tokens[i]);

            return 0;
        }

        free(route_tokens[i]);
        free(request_tokens[i]);
    }

    free(route_tokens);
    free(request_tokens);
    
    return 1;
}

void extract_request_body(const char* payload, size_t payload_size, char** body, size_t* body_size) {
    // Find the position of the blank line separating headers from the body
    const char* blank_line = strstr(payload, "\r\n\r\n");
    int blank_line_count = 4;
    if (blank_line == NULL) {
        *body = NULL;
        *body_size = 0;

        return;
    }

    size_t header_length = blank_line - payload + blank_line_count;
    size_t body_length = payload_size - header_length;
    if (body_length <= 0) {
        *body = NULL;
        *body_size = 0;

        return;
    }

    *body = (char*) malloc((body_length + 1) * sizeof(char));
    memcpy(*body, blank_line + blank_line_count, body_length); // skip blank line
    (*body)[body_length] = '\0';

    *body_size = body_length;
}

void extract_method_and_uri(const char* buffer, char** method, char** uri) {
    // Find the first space character
    const char* space_ptr = strchr(buffer, ' ');
    if (space_ptr == NULL) {
        fprintf(stderr, "Invalid HTTP request format: missing space\n");
        return;
    }

    // Calculate the length of the method
    size_t method_len = space_ptr - buffer;

    // Allocate memory for the method and copy it
    *method = (char*)malloc(method_len + 1);
    strncpy(*method, buffer, method_len);
    (*method)[method_len] = '\0'; // Null-terminate the method string

    // Find the next space character after the method
    const char* space_ptr2 = strchr(space_ptr + 1, ' ');
    if (space_ptr2 == NULL) {
        fprintf(stderr, "Invalid HTTP request format: missing second space\n");
        free(*method);
        return;
    }

    // Calculate the length of the URI
    size_t uri_len = space_ptr2 - (space_ptr + 1);

    // Allocate memory for the URI and copy it
    *uri = (char*)malloc(uri_len + 1);
    strncpy(*uri, space_ptr + 1, uri_len);
    (*uri)[uri_len] = '\0'; // Null-terminate the URI string
}