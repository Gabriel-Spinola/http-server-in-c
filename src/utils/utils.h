#ifndef UTIL_H
#define UTIL_H
    typedef enum {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    } http_method_e;

    typedef enum {
        STATUS_OK = 200,
        STATUS_INTERNAL_ERROR = 500 
    } status_e;

    const char* get_file_extension(const char* file_name);
    const char* get_mime_type(const char* file_extension);
    char* decode_url(const char* src);
    
    char* get_http_method(char* request_buffer);

    void print_title();
#endif /* UTIL_H */