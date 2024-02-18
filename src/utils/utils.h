#ifndef UTIL_H
#define UTIL_H
    #define TYPE_OF(x) _Generic((x), \
        int: "int", \
        float: "float", \
        double: "double", \
        char *: "string", \
        default: "unknown")

    #define BUFFER_SIZE 1024*5

    typedef enum {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    } http_method_e;

    typedef enum {
        STATUS_OK = 200,
        STATUS_CREATED = 201,
        STATUS_ACCEPTED = 202,
        STATUS_NO_CONTENT = 204,
        STATUS_MOVED_PERMANENTLY = 301,
        STATUS_FOUND = 302,
        STATUS_NOT_MODIFIED = 304,
        STATUS_BAD_REQUEST = 400,
        STATUS_UNAUTHORIZED = 401,
        STATUS_FORBIDDEN = 403,
        STATUS_NOT_FOUND = 404,
        STATUS_METHOD_NOT_ALLOWED = 405,
        STATUS_INTERNAL_ERROR = 500,
        STATUS_NOT_IMPLEMENTED = 501,
        STATUS_SERVICE_UNAVAILABLE = 503
    } status_e;

    const char* get_file_extension(const char* file_name);
    const char* get_mime_type(const char* file_extension);
    const char* http_method_to_string(http_method_e method);
    const char* status_code_to_string(status_e code);

    char* decode_url(const char* src);
    
    /// @brief Build http reponse
    /// @param response Mutate response
    /// @param header_buffer  Build and attach header 
    /// @param status 
    /// @param body Attach body if not null
    /// @return Maximum chars of output to write in the BUFFER_SIZE (MAXLEN)
    int build_http_response(
        char* response,
        char* header_buffer,
        status_e status,
        const char* body
    );

    void print_title();
#endif /* UTIL_H */