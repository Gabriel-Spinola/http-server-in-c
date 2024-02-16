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