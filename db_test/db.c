#include "db.h"

int main(){
    // Values higher than permited returns the last row;
    check_db(6);

    return 0;
}

// Returns user transaction/extract JSON from DB (prototype for further DB);
// Basically a demonstration to use jsonify_data;
int check_db(int user_id){
    //... { Connect with DB }
    //... { Make query to get rows }

    // Starts CSV HELL;
    FILE* fp;
    fp = fopen("user_db.csv", "r");
    if (fp == NULL){
        printf("Couldn't read file");
        return 1;
    }

    char row[MAXCHAR];
    char values[MAXCHAR];
    // Ignores headers row;
    int targered_row = user_id+1;
    int current_row = 1;

    // Get row of chosen id;
    while (current_row <= targered_row){
        fgets(row, MAXCHAR, fp);
        // Gets header row;
        if (current_row == 1){ memcpy(values, row, strlen(row)); };
        current_row++;
    }
    if (row == NULL){
        printf("Couldn't read row");
        return 1;
    }

    remove_spaces(row); remove_spaces(values);

    // Miscelaneous to extract keys and values from variables;
    char headers[MAXKEYS][MAXCHAR];
    int index = 0;
    char* token = strtok(values, ",");
    while (token != NULL && index < MAXKEYS){
        // Removes line break char;
        size_t len = strcspn(token, "\n");
        token[len] = '\0';
        strcpy(headers[index], token);
        // printf("headers: %s\n", headers[index]);
        index++;
        token = strtok(NULL, ",");
    }

    char bodies[MAXKEYS][MAXCHAR];
    index = 0;
    token = strtok(row, ",");
    while (token != NULL && index < MAXKEYS){
        size_t len = strcspn(token, "\n");
        token[len] = '\0';
        strcpy(bodies[index], token);
        // printf("bodies: %s\n", bodies[index]);
        token = strtok(NULL, ",");
        index++;
    }
    // Ends CSV HELL;

    jsonify_data(headers, bodies);
}

// Receives DB table headers and query data to make JSON struct:
// headers[1] + bodies[1] => { "headers[1]": "bodies[1]" }
// Can be used recursively;
void jsonify_data(char (*headers)[MAXCHAR], char (*bodies)[MAXCHAR]){
    json_object* root = json_object_new_object();

    for (int i = 0; i < MAXKEYS; i++){
        json_object_object_add(root, headers[i], json_object_new_string(bodies[i]));
    }

    // Use as wanted;
    printf("\nJSON: %s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    json_object_put(root);
}