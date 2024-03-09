#ifndef DATABASE_H
#define DATABASE_H
    struct pg_conn;

    extern int pqlib_version;
    extern struct pg_conn* m_conn;

    void init_database();
#endif