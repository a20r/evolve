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
        int index = randnum_i(0, strlen(*str) - 2);
        int diff = randnum_i(0, 100) <= 50 ? -1 : 1;
        int new_c = (*str)[index] + diff;

mutate:
        if (new_c <= CHAR_UPPER_BOUND && new_c >= CHAR_LOWER_BOUND) {
                (*str)[index] = (int) (*str)[index] + diff;
        } else {
                /* new_c is not a valid mutation, mutate again */
                diff = randnum_i(0, 100) <= 50 ? -1 : 1;
                new_c = (*str)[index] + diff;
                goto mutate;
        }
}

void mutate_dna(char **dna_str)
{
        char dna[4] = { 'A', 'C' , 'G', 'T' };
        int index = randnum_i(0, strlen(*dna_str) - 2);
        char new_base;

        while ((new_base = dna[randnum_i(0, 3)]) == (*dna_str)[index]) {}
        (*dna_str)[index] = new_base;
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
