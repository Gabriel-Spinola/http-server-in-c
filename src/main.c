/// USED LINKS AND FONTS
/// https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739
/// https://www.geeksforgeeks.org/socket-programming-cc/
/// https://gist.github.com/retr00exe/3196b0a0de61ba6d46f09b1e49561a52

/**
 * Implementation repository of https://github.com/zanfranceschi/rinha-de-backend-2024-q1.
 * 
 * REVIEW - At the moment the compiling process is done by the bootstrap.sh script
 * The api/_route.c is a special file storing the API routes. The router function within that file is called in the start_server function.
 * 
 * ## Folders
 * - http/ stores all HTTP-related data and helper functions.
 * - server/ deals with server initialization & client handling.
 * - api/ contains API implementations.
 * - utils/ provides utility functions.
 * 
 * ## Depedencies
 * - json-c
 * - pthread
 * - unix socket api
 * - (Optional) https://github.com/Gabriel-Spinola/PokeGelo-CLI
 * 
 * # Todos
 * ## Framwork TODOS
 * TODO - Write tests library
 * TODO - Write api tests (tests/poke)
 * 
 * ## API TODOs
 * TODO - Extrato route
 * TODO - Transacao route
 * TODO - Deploy
 * TODO - Gatsling
*/

#include "server/server.h"
#include "database/database.h"
#include <stdlib.h>

int main() {
    // NOTE - psql -h 0.0.0.0 -p 5432 -U postgres -d db_rinha
    init_database();  
    start_server();

    return EXIT_SUCCESS;
}
