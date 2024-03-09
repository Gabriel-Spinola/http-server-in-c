#include <db.h>

void remove_spaces(char s[]) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }

    } while (*s++ = *d++);
}