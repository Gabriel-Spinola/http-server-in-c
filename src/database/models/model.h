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
        char done[25];
    } transaction_model_t;

    typedef struct {
        int id;
        int client_id;
        int value;
    } balance_model_t;

    /// @note Always clear `*res` after use
    int debit_from_client(
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id,
        int value,
        const char* description
    );

    /// @note Always clear `*res` after use    
    int credit_from_client(
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id,
        int value,
        const char* description
    );

    /// @note Always clear `*res` after use
    /// @return 0 => Error | 1 => Success | (1 && model->id == NULL) => Data not found 
    int get_client_data(
        client_model_t* model,
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );

    /// @note Always clear `*res` after use
    /// @return 0 => Error | 1 => Success | (1 && model->id == NULL) => Data not found 
    int get_client_transactions(
        transaction_model_t** model,
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );

    /// @note Always clear `*res` after use
    /// @return 0 => Error | 1 => Success | (1 && model->id == NULL) => Data not found 
    int get_client_balances(
        balance_model_t* model,
        struct pg_conn* conn,
        struct pg_result* res,
        int client_id
    );
#endif