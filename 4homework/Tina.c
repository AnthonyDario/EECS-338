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
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {

    CLIENT *client;

    // create the client
    char *server_hostname = argv[1];
    client = clnt_create(server_hostname, COOKIE_JAR, ASSIGNMENT_6, "udp");

    if (client == NULL) {
        clnt_pcreateerror("Error creating client");
        exit(EXIT_FAILURE);
    }

    if (1) {
        printf("1 is true\n");
    }

    while (1) {
    
        // wait until hungry then request a cookie
        wait(1);
        request_cookie(client);

    }
}
