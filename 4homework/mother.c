/* The mother will hand out cookies from the cookie jar */
#include "cookie.h"

int cookies = 20;

// gives a cookie to Tina or Judy
int get_me_my_cookie(int a, int b) {

    // if there are no more cookies then let the children know
    if (cookies < 1) {
        return -2;
    }

    // if this is Tina then give her a cookie
    if (b == 1) {
        return 1;
    }

    // if this is Judy see if Tina has gotten 2
    if (b == 0) {
        if (tinaCount < 2) {
            return -1;
        } else {
            return 1;
        }
    }

}
