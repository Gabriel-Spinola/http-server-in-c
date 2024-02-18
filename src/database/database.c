#include <stdio.h>
#include <sqlite3.h>

int main()
{
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_open("database.db", &db);

    int rc = sqlite3_exec(
        db, 
        "CREATE TABLE IF NOT EXISTS algolancer(x1 INT, x2 INT, name varchar(60));", 
        NULL, 
        NULL, 
        &err
    );

    if(rc != SQLITE_OK) {
        printf("%c", *err);
    }
}
