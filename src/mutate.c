#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "evolve.h"

void mutate_str(char **str)
{
        /* decide which char to mutate and by moving char val up or down */
        int str_len = strlen((char *) *str);
        int index = randnum_i(0, str_len);
        int up_or_down = randnum_i(0, 100) <= 50 ? -1 : 1;

        (*str)[index] = (*str)[index] + up_or_down;
}

void mutator(
        struct chromosome_pair **c_pair,
        float mutate_chance,
        void (mutator)(char **))
{
        /* to mutate or not mutate */
        if (randnum_i(0, 100) > mutate_chance) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        debug("mutate!", "hwero");
        (*mutator)((char **) &(*c_pair)->child_1);
no_mutate:
        debug("skip mutate!");
        return;
}
