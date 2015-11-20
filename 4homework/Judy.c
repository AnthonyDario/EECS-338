/* Judy is the younger sister and mustv wait for Tina before eating */ 

#include <stdlib.h>
#include <stdio.h>
#include "cookie.h"

// request a cookie from mom
int request_cookie(CLIENT *client) {

    // create the request object
    struct cookie_request request = {0, 0};

    // request the cookie
    int cookie = get_me_my_cookie_1(cookie_request, client);

    // if there are no more cookies then stop asking
    if (cookie == -2) {
        printf("No more cookies for Judy...\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Need to input the RPC server hostname\n");
    }

    CLIENT *client;

    //create the client
    char *server_hostname = argvv[1];
    client = clnt_create(server_hostname, COOKIE_JAR, ASSIGNMENT_6, "udp");

    if (client == NULL) {
        clnt_pcreateerror("Error creating Judy's client");
        exit(EXIT_FAILURE)
    }

    while (1) {

        // wait until hungry then request a cookie
        wait(rand() % 4);
        printf("Judy is hungry, requesting cookie!\n");
        request_cookie(client);
    }
}

