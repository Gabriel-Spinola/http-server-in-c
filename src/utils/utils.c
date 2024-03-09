#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

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

const char* get_file_extension(const char* file_name) {
    const char* dot = strrchr(file_name, '.');

    if (!dot || dot == file_name) {
        return "";
    }

    return dot + 1;
}

int string_to_int(const char* target) {
    int output = 0;
    int offset = 48;

    for (int i = 0; target[i] != '\0'; ++i) {
        output = output * 10 + (target[i] - offset);
    }

    return output;
}

void get_current_time(char time_buffer[30]) {
    // Get current time
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Convert current time to struct tm (time components)
    struct tm* time_info = localtime(&tv.tv_sec);

    if (time_info == NULL) {
        fprintf(stderr, "Failed to get time information\n");
        return;  // Return an error code
    }

    size_t buffer_size = 30;
    strftime(time_buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", time_info);

    // Append fractional seconds to the formatted time
    snprintf(time_buffer + strlen(time_buffer), buffer_size - strlen(time_buffer),
             ".%06ldZ", tv.tv_usec);
}