#include "http.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

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

void build_error_response(
    char* response,
    char* header_buffer,
    status_e error_code
) {
    const char* err_message = status_code_to_string(error_code);
    build_http_response(response, header_buffer, error_code, err_message);

    free(header_buffer);
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
        case STATUS_UNPROCESSABLE_ENTITY:
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