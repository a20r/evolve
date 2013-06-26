#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <dbg/dbg.h>

#include "utils.h"


int randnum_i(int l_bound, int u_bound)
{
        int divisor = RAND_MAX / (u_bound + 1);
        int retval;

        do {
                retval = rand() / divisor;
        } while (retval < l_bound || retval > u_bound);

        return retval;
}

float randnum_f(float l_bound, float u_bound)
{
        float divisor = (float) RAND_MAX / (float) (u_bound + 1.0);
        float retval;

        do {
                retval = (float) rand() / divisor;
        } while (retval < l_bound || retval > u_bound);

        return retval;
}

char *randstr(int length)
{
        int i = 0;
        int el_size = sizeof(char) * (length + 1);
        char *randstr = malloc(el_size);

        check_mem(randstr);
        memset(randstr, '\0', el_size);

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                randstr[i] = randnum_i(32, 128);
        }

        return randstr;
error:
        return NULL;
}
