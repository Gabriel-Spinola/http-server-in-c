#include "db.h"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

void check_db(); // => JSON_OBJ, 

int main(){
    check_db();

    return 0;
}

// Returns user transaction/extract JSON from DB (prototype for further DB);
void check_db (){
    FILE* fp;
    fp = fopen("user_db.csv", "r");

    char row[MAXCHAR];
    char* token;

    while(feof(fp) != true) 
    {
        fgets(row, MAXCHAR, fp);
        printf("Row: %s\n", row);

        token = strtok(row, ",");

        while(token != NULL)
        {
            printf("Token %s\n", token);
            token = strtok(NULL, ",");
        }
    }
}