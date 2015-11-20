/* The mother will hand out cookies from the cookie jar */
#include "cookie.h"

int cookies = 20;
int tinaCount = 0;

// gives a cookie to Tina or Judy
int get_me_my_cookie_svc(struct cookie_request, struct svc_req * req) {

    // if there are no more cookies then let the children know
    if (cookies < 1) {
        printf("\"No more cookies for you!\n");
        return -2;
    }

    // if this is Tina then give her a cookie
    if (b == 1) {
        printf("You can have a cookie Tina\n");
        tinaCount++;
        cookies--;
        return 1;
    }

    // if this is Judy see if Tina has gotten 2
    if (b == 0) {
        if (tinaCount < 2) {
            printf("You may have a cookie Judy\n");
            cookies--;
            tinaCount = 0;
            return -1;
        } else {
            printf("You have to wait for Tina to get %d more ", 2 - tinaCount);
            printf("cookies before getting a cookie Judy\n");
            return 1;
        }
    }

}
