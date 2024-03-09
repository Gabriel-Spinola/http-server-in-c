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

/// NOTE - psql -h 0.0.0.0 -p 5432 -U postgres -d db_rinha
void init_database() {
    printf("Starting Database...\n");

#if defined(DEV)
        const char* conn_info = "host=0.0.0.0 port=5432 dbname=db_rinha user=postgres password=rinha_pass";
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

    struct pg_result* res = NULL;
    int ok = debit_from_client(m_conn, res, 1, 100, "desc");
    if (!ok) {
        fprintf(stderr, "failed to debit from client: %s\n", PQerrorMessage(m_conn));
        exit_failure(m_conn, res);
    }

    PQclear(res);
}

void close_database() {
    PQfinish(m_conn);
}