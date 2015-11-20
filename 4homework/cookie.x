/* Program definition for RPC cookie jar */

// struct to pass a request for cookie
struct cookie_request {
    int a;  // the response to the request
    int b;  // which sister is requesting
};

program COOKIE_JAR {
    version COOKIE_JAR_VERSION {
  
        struct cookie_request REQUEST_COOKIE(struct cookie_supplies) = 1;

    } = 1;
} = 2020;
