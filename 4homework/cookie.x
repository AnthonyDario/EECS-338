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
  
        int get_me_my_cookie(struct cookie_request) = 1;

    } = 1;
} = 0x84467008;
