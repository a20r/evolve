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
        char *r_str = malloc(sizeof(char *) * length);

        check_mem(r_str);
        memset(r_str, '\0', sizeof(char *) * length);

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                r_str[i] = randnum_i(32, 122);
        }
        r_str[length] = '\0';  /* add null terminator for safety */

        return r_str;
error:
        if (r_str) free(r_str);
        return NULL;
}

char *randdna(int length)
{
        int i = 0;
        char dna[4] = { 'A', 'C' , 'G', 'T' };
        char *r_dna = malloc(sizeof(char *) * length);

        check_mem(r_dna);
        memset(r_dna, '\0', sizeof(char *) * length);

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                r_dna[i] = dna[randnum_i(0, 3)];
        }
        r_dna[length] = '\0';  /* add null terminator for safety */

        return r_dna;
error:
        if (r_dna) free(r_dna);
        return NULL;
}
