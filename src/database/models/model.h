#ifndef MODEL_H
#define MODEL_H
    #include <time.h>

    struct pg_conn;
    struct pg_result;

    typedef struct {
        int id;
        char name[50];
        int limit;
    } client_model_t;

    typedef struct {
        int id;
        int client_id;
        int value;
        char type;
        char description[10];
        time_t done;
    } transaction_model_t;

    typedef struct {
        int id;
        int client_id;
        int value;
    } balance_model_t;

    int debit_from_client(
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id,
        int value,
        char description[10]
    );

    int credit_from_client(
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id,
        int value,
        char description[10]
    );

    int get_client_data(
        client_model_t* model,
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );

    int get_client_transactions(
        transaction_model_t* model[2],
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );

    int get_client_balances(
        balance_model_t* model,
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );
#endif