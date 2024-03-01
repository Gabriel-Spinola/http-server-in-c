#include "db.h"
#include <stdlib.h>

int main(){
    check_db(2);

    return 0;
}

// Returns user transaction/extract JSON from DB (prototype for further DB);
// Basically a demonstration to use jsonify_data and tokenize_data;
int check_db(int id){
    //... { Connect with DB }
    //... { Make query to get rows }
    FILE* fp;
    fp = fopen("user_db.csv", "r");
    if (fp == NULL){
        printf("Couldn't read file");
        return 1;
    }

    char row[MAXCHAR];
    char headers[MAXCHAR];
    // Ignores headers row;
    int targered_row = id+1;
    int current_row = 1;

    // Get row of id;
    while (current_row <= targered_row){
        fgets(row, MAXCHAR, fp);
        // Gets header row;
        if (current_row == 1){ memcpy(headers, row, strlen(row)); };
        current_row++;
    }
    if (row == NULL){
        printf("Couldn't read row");
        return 1;
    }

    free(fp);

    remove_spaces(row); remove_spaces(headers);

    printf("Targered Row: %s\n", row);
    printf("Row Headers: %s", headers);

    jsonify_data(headers, row);
}

// Receives DB table headers and query data to make JSON struct:
// headers[1] + bodies[1] => { headers[1]: bodies[1] }
// Can be used recursively;
void jsonify_data(char headers[], char bodies[]){

}