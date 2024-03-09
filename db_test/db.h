#ifndef DB_H 
#define DB_H

    #define MAXCHAR 50
    #define MAXKEYS 3
    #include <stdio.h>
    #include <json-c/json.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "utils.c"
    int check_db(int id);
    void jsonify_data(char (*headers)[], char (*bodies)[]);

#endif