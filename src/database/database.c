/// LINK - https://zetcode.com/db/postgresqlc/

#include "database.h"
#include <stdio.h>
#include <libpq-fe.h>
#include <stdlib.h>
#include "./models/model.h"
#include "../utils/utils.h"

int pqlib_version;
struct pg_conn* m_conn;

// set PGconn arg;
static void dev_populate_db();
static int convert_seed_file(PGconn* conn);

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

    client_model_t base_model;
    int id = 1;

    PGresult* res = PQexec(m_conn, "SELECT * FROM clientes;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Failed to get client %s\n", PQerrorMessage(m_conn));
        exit_failure(m_conn, res);
    }

    printf("Ocurrencies: %d\n", PQntuples(res));
    //printf("BASE CLIENTE MODEL: %d | %s | %d\n", PQnres, base_model.name, base_model.limit);
    PQclear(res);

    // ANCHOR - Test 1
    /*int ok = get_client_data(&base_model, m_conn, res, id);
    if (!ok) {
        fprintf(stderr, "Failed get data from client %s\n", PQerrorMessage(m_conn));
        exit_failure(m_conn, res);
    }

    printf("BASE CLIENTE MODEL: %d | %s | %d\n", base_model.id, base_model.name, base_model.limit);
    PQclear(res);*/
/*
    ok = debit_from_client(m_conn, res, 1, 100, "123456789");
    if (!ok) {
        fprintf(stderr, "Failed to debit from client %s\n", PQerrorMessage(m_conn));
        exit_failure(m_conn, res);
    }

    PQclear(res);*/

    /*balance_model_t last_two_balances;
    int ok = get_client_balances(&last_two_balances, m_conn, res, id);
    if (!ok) {
        fprintf(stderr, "Failed to get client balances: %s\n", PQerrorMessage(m_conn));

        exit_failure(m_conn, res);
    }*/

    PQfinish(m_conn);
}
