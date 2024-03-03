#include <stdio.h>
#include <sqlite3.h>

int sqlite3_retorno(void* NotUsed, int argc, char **argv, char **coluna) {

    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", coluna[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
    
}

int main()
{
    char* err;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc = sqlite3_open("database.sqlite", &db);

    if(rc != SQLITE_OK) {
        printf("%c", *err);
        sqlite3_close(db);
        return 1;
    }

//tabela cliente
    char cliente[] = "CREATE TABLE clientes ("
	"id SERIAL PRIMARY KEY,"
	"nome VARCHAR(50) NOT NULL,"
	"limite INTEGER NOT NULL);";

    rc = sqlite3_exec(db, cliente, sqlite3_retorno, 0, &err);

    if(rc != SQLITE_OK) {
        printf("CLiente ERROR: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

//tabela transações
    char transacoes[] = "CREATE TABLE transacoes ("
	"id SERIAL PRIMARY KEY,"
	"cliente_id INTEGER NOT NULL,"
	"valor INTEGER NOT NULL,"
	"tipo CHAR(1) NOT NULL,"
	"descricao VARCHAR(10) NOT NULL,"
	"realizada_em TIMESTAMP NOT NULL DEFAULT NOW(),"
	"CONSTRAINT fk_clientes_transacoes_id"
	"FOREIGN KEY (cliente_id) REFERENCES clientes(id)"
    ");";

    rc = sqlite3_exec(db, transacoes, sqlite3_retorno, 0, &err);

    if(rc != SQLITE_OK) {
        printf("Transações ERROR: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

//tabela saldos
    char saldos[] = "CREATE UNLOGGED TABLE saldos ("
	"id SERIAL PRIMARY KEY,"
	"cliente_id INTEGER NOT NULL,"
	"valor INTEGER NOT NULL,"
	"CONSTRAINT fk_clientes_saldos_id"
		"FOREIGN KEY (cliente_id) REFERENCES clientes(id);";

    rc = sqlite3_exec(db, saldos, sqlite3_retorno, 0, &err);

    if(rc != SQLITE_OK) {
        printf("Saldos ERROR: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }   

//inserindo valores
    char def_clientes[] = "DO $$"
                        "BEGIN"
                        "INSERT INTO clientes (nome, limite)"
                        "VALUES"
                        "('o barato sai caro', 1000 * 100),"
                        "('zan corp ltda', 800 * 100),"
                        "('les cruders', 10000 * 100),"
                        "('padaria joia de cocaia', 100000 * 100),"
                        "('kid mais', 5000 * 100);"
                    
                        "INSERT INTO saldos (cliente_id, valor)"
                        "SELECT id, 0 FROM clientes;"
                        "END;"
                        "$$;";

    rc = sqlite3_exec(db, def_clientes, sqlite3_retorno, 0, &err);

    if(rc != SQLITE_OK) {
        printf("def ERROR: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }  

//função debitar



    sqlite3_free(err);
    sqlite3_close(db);
    return 0;
}