/* Program definition for RPC cookie jar */

/* struct to pass a request for cookie 
 * a is the response to the request
 * b is the which sister is requesting
 */
struct cookie_request {
    int a;  
    int b; 
};

program COOKIE_JAR {
    version ASSIGNMENT_6 {
  
        struct cookie_request REQUEST_COOKIE(struct cookie_supplies) = 1;

    } = 1;
} = 2020;