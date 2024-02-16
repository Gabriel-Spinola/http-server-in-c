#ifndef UTIL_H
#define UTIL_H
    const char* get_file_extension(const char* file_name);
    const char* get_mime_type(const char* file_extension);
    char* decode_url(const char* src);
    
    void print_title();
#endif /* UTIL_H */