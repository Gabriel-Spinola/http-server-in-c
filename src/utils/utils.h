#ifndef UTIL_H
#define UTIL_H
    #define DEBUG 1
    
    const char* get_file_extension(const char* file_name);
    char** split(const char* string, const char* delimiter, int* num_tokens);
    char* decode_url(const char* src);

    int string_to_int(const char* target);

    void print_title();
#endif /* UTIL_H */