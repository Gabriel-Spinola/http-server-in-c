#define _OPEN_SYS_ITOA_EXT

#include "model.h"
#include <libpq-fe.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../utils/utils.h"
#include "../../http/http.h"

/// @note Always clear `*res` after use
/// @return 0 => Error | 1 => Success | (1 && model[i]->id == NULL) => Data not found 
int get_client_data(
    client_model_t* model,
    struct pg_conn* m_conn,
    struct pg_result* res,
    int client_id
) {
    const char* param_values[1];
    char stringfied_user_id[2];

    // Convert into octal numver format
    snprintf(stringfied_user_id, 2, "%d", client_id);;
    param_values[0] = stringfied_user_id;

    const char* query = "SELECT * FROM clientes WHERE id=$1";
    res = PQexecParams(m_conn, query, 1, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "No data retrieved from clientes database\n");

        return 0;
    }

    if (PQntuples(res) <= 0) {
        fprintf(stderr, "User with id %s not found\n", param_values[0]);

        return 1;
    }

    if (PQntuples(res) > 1) {
        fprintf(stderr, "Undefined Behaviour: multiple ocurrencies of user: %d\n", client_id);

        return 0;
    }

    model->id = string_to_int(PQgetvalue(res, 0, 0));
    strcpy(model->name, PQgetvalue(res, 0, 1));
    model->limit = string_to_int(PQgetvalue(res, 0, 2));

    return 1;
}

/// @note Always clear `*res` after use
/// @return 0 => Error | 1 => Success | (1 && model[i]->id == NULL) => Data not found 
int get_client_transactions(
    transaction_model_t* model[2],
    struct pg_conn* conn,
    struct pg_result* res,
    int client_id
) {
    const char* query = "SELECT * FROM transacoes WHERE cliente_id = $1 ORDER BY realizada_em DESC LIMIT 10";
    const char* param_values[1];

    char stringfied_user_id[2];
    snprintf(stringfied_user_id, 2, "%d", client_id);

    param_values[0] = stringfied_user_id;

    res = PQexecParams(conn, query, 1, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Failed to get client transactions\n");

        return 0;
    }

    int rows = PQntuples(res);
    if (rows <= 0) {
        printf("No trasactions found\n");

        return 1;
    }

    for (int row = 0; row < rows; row++) {
        model[row]->id = string_to_int(PQgetvalue(res, row, 0));
        model[row]->client_id = string_to_int(PQgetvalue(res, row, 1));
        model[row]->value = string_to_int(PQgetvalue(res, row, 2));
        model[row]->type = PQgetvalue(res, row, 3)[0];

        strcpy(model[row]->description, PQgetvalue(res, row, 4));
    }

    return 1;
}

/// @note Always clear `*res` after use
/// @return 0 => Error | 1 => Success | (1 && model->id == NULL) => Data not found 
int get_client_balances(
    balance_model_t* model,
    struct pg_conn* conn,
    struct pg_result* res,
    int client_id
) {
    const char* query = "SELECT * FROM saldos WHERE cliente_id=$1";
    const char* param_values[1];

    char stringfied_user_id[2];
    snprintf(stringfied_user_id, 2, "%d", client_id);

    param_values[0] = stringfied_user_id;

    res = PQexecParams(conn, query, 1, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Failed to get client balances\n");

        return 0;
    }

    if (PQntuples(res) <= 0) {
        printf("No balance found\n");

        return 1;
    }

    model->id = string_to_int(PQgetvalue(res, 0, 0));
    model->client_id = string_to_int(PQgetvalue(res, 0, 1));
    model->value = string_to_int(PQgetvalue(res, 0, 2));

    return 1;
}

/// @note Always clear `*res` after use
int debit_from_client(
    struct pg_conn* conn,
    struct pg_result* res,
    int client_id,
    int value,
    char description[10]
) {
    const char* query = "SELECT * FROM debitar($1, $2, $3)";
    const char* param_values[3];

    char stringfied_user_id[2];
    char stringfied_user_value[500000];

    snprintf(stringfied_user_id, 2, "%d", client_id);
    snprintf(stringfied_user_value, 500000, "%d", value);
    if (stringfied_user_value == NULL) {
        fprintf(stderr, "Failed to allocate memory for user value parameter\n");

        return 0;
    }

    param_values[0] = stringfied_user_id;
    param_values[1] = stringfied_user_value;
    param_values[2] = description;

    res = PQexecParams(conn, query, 3, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Failed to debit\n");

        return 0;
    }

    // TODO - Handle debit function error 
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        printf(
            "novo_saldo: %s, possui_erro: %s, mensagem: %s\n",
            PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2)
        );
    }

#if DEBUG
    printf("-----------------------------\n");
    printf("MODIFIED:\n");

    client_model_t base_model;
    int ok = get_client_data(&base_model, conn, res, 1);
    if (!ok) {
        return 0;
    }

    printf("BASE CLIENTE MODEL: %d | %s | %d\n", base_model.id, base_model.name, base_model.limit);
#endif

    return 1;
}

/// @note Always clear `*res` after use
int credit_from_client(
    struct pg_conn* conn,
    struct pg_result* res,
    int client_id,
    int value,
    char description[10]
) {
    const char* query = "SELECT * FROM creditar($1, $2, $3)";
    const char* param_values[3];

    char stringfied_user_id[2];
    char stringfied_user_value[500000];

    snprintf(stringfied_user_id, 2, "%d", client_id);
    snprintf(stringfied_user_value, 500000, "%d", value);
    if (stringfied_user_value == NULL) {
        fprintf(stderr, "Failed to allocate memory for user value parameter\n");

        return 0;
    }

    param_values[0] = stringfied_user_id;
    param_values[1] = stringfied_user_value;
    param_values[2] = description;

    printf("VALUES: %s\n", stringfied_user_value);

    res = PQexecParams(conn, query, 3, NULL, param_values, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Failed to credit\n");

        return 0;
    }

    // TODO - Handle debit function error 
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        printf("novo_saldo: %s, possui_erro: %s, mensagem: %s\n",
               PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
    }

#if DEBUG
    printf("-----------------------------\n");
    printf("MODIFIED:\n");

    client_model_t base_model;
    int ok = get_client_data(&base_model, conn, res, 1);
    if (!ok) {
        return 0;
    }

    printf("BASE CLIENTE MODEL: %d | %s | %d\n", base_model.id, base_model.name, base_model.limit);
#endif
    return 1;
}
