/* The mother will hand out cookies from the cookie jar */
#include "cookie.h"

int cookies = 20;

// gives a cookie to Tina or Judy
int get_me_my_cookie(int a, int b) {

    // if there are no more cookies then let the children know
    if (cookies < 1) {
        printf("\"No more cookies for you!\n");
        return -2;
    }

    // if this is Tina then give her a cookie
    if (b == 1) {
        printf("You can have a cookie Tina\n");
        return 1;
    }

    // if this is Judy see if Tina has gotten 2
    if (b == 0) {
        if (tinaCount < 2) {
            printf("You may have a cookie Judy\n");
            return -1;
        } else {
            printf("You have to wait for Tina before getting a cookie Judy\n");
            return 1;
        }
    }

}