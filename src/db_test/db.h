#ifndef DB_H 
#define DB_H

    #define MAXCHAR 50
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include "utils.c"
    int check_db(int id);
    void tokenize_args(char headers[], char bodies[]);
    void jsonify_data(char headers[], char bodies[]);

#endif