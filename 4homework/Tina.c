/* Tina is the older sister and can eat whenever she wants */

#include <stdio.h>
#include <stdlib.h>
#include "cookie.h"

int cookies = 0;

//request a cookie from mom
void request_cookie(CLIENT *client) {

    // create the request object
    struct cookie_request request = {0, 1};

    // request the cookie
    int *cookie = get_me_my_cookie_1(&request, client);

    // if there are no more cookies then stop asking
    if (*cookie == -2) {
        printf("No more cookies for Tina...\n");
        printf("At least I got %d cookies\n", cookies);

        done_with_cookies_1(client);
        clnt_destroy(client);
        exit(EXIT_SUCCESS);
    } else {
        cookies++;
        printf("I got a cookie!\n\t Tina has gotten %d cookies\n", cookies);
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
        sleep(rand() % 4);
        printf("Tina is hungry, requesting cookie!\n");
        request_cookie(client);

    }
}
