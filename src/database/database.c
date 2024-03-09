/// LINK - https://zetcode.com/db/postgresqlc/

#include "database.h"
#include <stdio.h>
#include <libpq-fe.h>
#include <stdlib.h>
#include "./models/model.h"
#include "../utils/utils.h"

int pqlib_version;
struct pg_conn* m_conn;

void exit_failure(PGconn* conn, PGresult* res) {
    if (res != NULL) {
        PQclear(res);
    }

    PQfinish(conn);
    exit(1);
}

void init_database() {
    printf("Starting Database...\n");

    #if defined(DEV)
        const char* conn_info = "host=localhost port=5432 dbname=db_rinha user=postgres password=rinha_pass";
    #else
        const char* conn_info = "host=database port=5432 dbname=db_rinha user=postgres password=rinha_pass";
    #endif

    m_conn = PQconnectdb(conn_info);
    if (PQstatus(m_conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(m_conn));
        exit_failure(m_conn, NULL);
    }

    pqlib_version = PQlibVersion();
    printf("Database have been successfully initialized.\n- Postgres Version: %d\n", pqlib_version);
    
    PGresult* res = NULL;
    client_model_t base_model;
    int id = 1;

    int ok = get_client_data(&base_model, m_conn, res, id);
    if (!ok) {
        fprintf(stderr, "Failed get data from client with id: %d\n", id);
        exit_failure(m_conn, res);
    }

    PQfinish(m_conn);
}
