/* Tina is the younger sister and must wait for Judy before eating */

#include <stdio.h>
#include <stdlib.h>
#include <cookie.h>

//request a cookie from mom
int requestCookie() {

    int cookie = 0;

    // create the request object
    struct cookie_request request = {cookie, 1};

    // request the cookie

    // if there are no more cookies then stop asking
    if (cookie == -2) {
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {

    // create the client
    char *server_hostname = argv[1];
    client = clnt_create(server_hostname, , , "udp");

    if (client == NULL) {
        clnt_pcreateerror("Error creating client");
        exit(EXIT_FAILURE);
    }

    while (true) {
    
        // wait until hungry then request a cookie
        wait(1);
        cookie = requestCookie();

    }
}
