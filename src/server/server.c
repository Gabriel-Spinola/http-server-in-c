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
    
    if (bytes_received > 0) {
        regex_t regex;
        regmatch_t matches[2];

        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);

        if (regexec(&regex, buffer, 2, matches, 0) == 0) {
            // get file name
            buffer[matches[1].rm_eo] = '\0';
            const char* url_encoded_filename = buffer + matches[1].rm_so;

            // decode url
            char* filename = decode_url(url_encoded_filename);
            printf(filename);

            // Get file extension
            char file_extension[32];
            strcpy(file_extension, get_file_extension(filename));

            // Build http response
            char* response = (char*) malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_length;
            build_http_response(filename, file_extension, response, &response_length);

            // FIXME - For some reason when compiled this function is considered a implicit declaration even tough it is not
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
            request_handler_t req = { 0 };
            req.method = "GET";
            route(req);
            #pragma GCC diagnostic pop

            // Send HTTP response to client
            send(client_fd, response, response_length, 0);

            fflush(stdout);
            free(response);
            free(filename);
        }

        regfree(&regex);
    }

    close(client_fd);

    free(client_socket_fd);
    free(buffer);

    return NULL;
}

void build_http_response(
    const char* file_name,
    const char* file_extension,
    char* response,
    size_t* response_length
) {
    const char* mime_type = get_mime_type(file_extension);
    char* header = (char*) malloc(BUFFER_SIZE * sizeof(char));

    snprintf(header, BUFFER_SIZE,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "\r\n",
        mime_type
    );

    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1) {
        snprintf(response, BUFFER_SIZE,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "404 Not Found"
        );

        *response_length = strlen(response);

        return;
    }

    // File size for content length
    struct stat file_stat;
    fstat(file_fd, &file_stat);

    off_t file_size = file_stat.st_size;

    // Copy header to response buffer
    *response_length = 0;
    memcpy(response, header, strlen(header));
    *response_length += strlen(header);

    // Copy file to response buffer
    ssize_t bytes_read = read(
        file_fd,
        response + *response_length,
        BUFFER_SIZE - *response_length
    );
    while (bytes_read > 0) {
        *response_length += bytes_read;
    }

    free(header);
    close(file_fd);
}
