/// LINK - https://zetcode.com/db/postgresqlc/

#include "database.h"
#include <stdio.h>
#include <libpq-fe.h>
#include <stdlib.h>

int pqlib_version;

void exit_failure(PGconn* conn, PGresult* res) {
    if (res != NULL) {
        PQclear(res);
    }

    PQfinish(conn);
    exit(1);
}

void init_database() {
    printf("Starting Database...\n");

    PGconn* conn = PQconnectdb("host=database port=5432 dbname=db_rinha user=postgres password=rinha_pass");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));

        exit_failure(conn, NULL);
    }

    pqlib_version = PQlibVersion();
    printf("Database have been successfully initialized.\n- Postgres Version: %d\n", pqlib_version);
    
    PQfinish(conn);
}