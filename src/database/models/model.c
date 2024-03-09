#define _OPEN_SYS_ITOA_EXT

#include "model.h"
#include <libpq-fe.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int update_client_limit(struct pg_conn* conn, struct pg_result* res, client_model_t* model) {
    
}

int get_client_data(
    client_model_t* model,
    struct pg_conn* m_conn,
    struct pg_result* res,
    int user_id
) {
    const char* param_values[2];
    char converted_param[2];

    // Convert into octal numver format
    snprintf(converted_param, 2, "%d", user_id);
    printf("Converted parameter: %s\n", converted_param);
    param_values[0] = converted_param;

    const char* query = "SELECT * FROM clientes WHERE id=$1";
    res = PQexecParams(m_conn, query, 1, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved from clientes database");

        return 0;
    }

    //model->id = PQgetvalue(res, 0, 0);
    //model->name = PQgetvalue(res, 1, 0);
    //model->limit = PQgetvalue(res, 2, 0);

    printf("%d\n", PQntuples(res));
    printf("%s | %s | %s\n", PQgetvalue(res, 0, 0), PQgetvalue(res, 0, 1), PQgetvalue(res, 0, 2));

    PQclear(res);

    res = PQexec(m_conn, "SELECT * FROM clientes");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved from clientes database");

        return 0;
    }

    printf("AAAAA: %d\n", PQntuples(res));

    /* Rows and Columns example
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%s | %s | %s\n", PQgetvalue(res, row, col), PQgetvalue(res, row, col), PQgetvalue(res, row, col));
        }
    }
    */

    return 1;
}