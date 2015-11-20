/* Tina is the older sister and can eat whenever she wants */

#include <stdio.h>
#include <stdlib.h>
#include "cookie.h"

//request a cookie from mom
int request_cookie(CLIENT *client) {

    int cookie = 0;

    // create the request object
    struct cookie_request request = {cookie, 1};

    // request the cookie
    request_cookie_1(cookie_request, client);

    // if there are no more cookies then stop asking
    if (cookie == -2) {
        printf("No more cookies for Tina...\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Need to input the RPC server hostname\n");
    }

    CLIENT *client;

    // create the client
    char *server_hostname = argv[1];
    client = clnt_create(server_hostname, COOKIE_JAR, ASSIGNMENT_6, "udp");

    if (client == NULL) {
        clnt_pcreateerror("Error creating client");
        exit(EXIT_FAILURE);
    }

    while (1) {
    
        // wait until hungry then request a cookie
        wait(rand() % 4);
        printf("Tina is hungry, requesting cookie!\n");
        request_cookie(client);

    }
}
