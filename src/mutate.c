#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "evolve.h"
#include "utils.h"

#define CHAR_LOWER_BOUND 32
#define CHAR_UPPER_BOUND 122

void mutate_str(char **str)
{
        /* decide which char to mutate and by moving char val up or down */
        int str_len = strlen(*str);
        int index = randnum_i(0, str_len);
        int diff = randnum_i(0, 100) <= 50 ? -1 : 1;
        int c = (*str)[index];

        if ((c + diff) <= CHAR_UPPER_BOUND && (c + diff) >= CHAR_LOWER_BOUND) {
                (*str)[index] = (int) (*str)[index] + diff;
        }
}

void mutate(
        void **chromosome,
        float mutate_prob,
        void (mutate_func)(char **)
)
{
        /* to mutate or not mutate */
        if (mutate_prob> randnum_f(0.0, 1.0)) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        (*mutate_func)((char **) chromosome);
no_mutate:
        return;
}
