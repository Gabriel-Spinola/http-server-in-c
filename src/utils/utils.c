#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void print_title() {
    char big_c[] =
        "         _______\n"
        "        /  _____|\n"
        "       /  /\n"
        "       \\  \\_____\n"
        "        \\_______|\n";

    char title[] = "    RINHA DE DEVS em C\n";

    printf("%s\n", big_c);
    printf("%s", title);
}

char* decode_url(const char* src) {
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    size_t decoded_len = 0;

    // decode %2x to hex
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] == '%' && i + 2 < src_len) {
            int hex_val;
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len++] = hex_val;
            i += 2;
        } else {
            decoded[decoded_len++] = src[i];
        }
    }

    // add null terminator
    decoded[decoded_len] = '\0';
    return decoded;
}

const char* get_file_extension(const char* file_name) {
    const char* dot = strrchr(file_name, '.');

    if (!dot || dot == file_name) {
        return "";
    }

    return dot + 1;
}

const char* get_mime_type(const char* file_extension) {
    // NOTE - Add more types if necessary

    if (strcasecmp(file_extension, "json")) {
        return "application/json";
    }

    return "application/octet-stream";
}

int build_http_response(
    char* response,
    char* header_buffer,
    status_e status,
    const char* body
) {
    const char* status_string = status_code_to_string(status);

    int val = snprintf(header_buffer, BUFFER_SIZE,
        "HTTP/1.1 %s\r\n"
        "Server: webserver-c"
        "Content-type: application/json\r\n"
        "\r\n",
        status_string
    );

    if (body == NULL) {
        return val;
    }

    return snprintf(response, BUFFER_SIZE, "%s%s\r\n", header_buffer, body);
}

const char* http_method_to_string(http_method_e method) {
    switch (method) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case DELETE:
            return "DELETE";
        case UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

const char* status_code_to_string(status_e code) {
    switch (code) {
        case STATUS_OK:
            return "200 OK";
        case STATUS_CREATED:
            return "201 Created";
        case STATUS_ACCEPTED:
            return "202 Accepted";
        case STATUS_NO_CONTENT:
            return "204 No Content";
        case STATUS_MOVED_PERMANENTLY:
            return "301 Moved Permanently";
        case STATUS_FOUND:
            return "302 Found";
        case STATUS_NOT_MODIFIED:
            return "304 Not Modified";
        case STATUS_BAD_REQUEST:
            return "400 Bad Request";
        case STATUS_UNAUTHORIZED:
            return "401 Unauthorized";
        case STATUS_FORBIDDEN:
            return "403 Forbidden";
        case STATUS_NOT_FOUND:
            return "404 Not Found";
        case STATUS_METHOD_NOT_ALLOWED:
            return "405 Method Not Allowed";
        case STATUS_INTERNAL_ERROR:
            return "500 Internal Server Error";
        case STATUS_NOT_IMPLEMENTED:
            return "501 Not Implemented";
        case STATUS_SERVICE_UNAVAILABLE:
            return "503 Service Unavailable";
        default:
            return "UNKNOWN";
    }
}

#define MAX_TOKENS 100 // Adjust this according to your needs
#define MAX_TOKEN_LENGTH 100 // Adjust this according to your needs

char** split(const char* string, const char* delimiter, int* num_tokens) {
    char** final = (char**)malloc(MAX_TOKENS * sizeof(char*));
    if (final == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    char* copy = strdup(string); // Make a copy of the original string
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(copy, delimiter);
    size_t count = 0;
    while (token != NULL && count < strlen(string) - 1) {
        final[count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if (final[count] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(final[count], token);
        count++;
        token = strtok(NULL, delimiter);
    }
    final[count] = NULL; // Null-terminate the array of strings

    free(copy); // Free the copy of the original string
    *num_tokens = count;
    return final;
}