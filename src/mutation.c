#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "evolve.h"
#include "utils.h"

#ifndef CHAR_LOWER_BOUND
  #define CHAR_LOWER_BOUND 32
#endif

#ifndef CHAR_UPPER_BOUND
  #define CHAR_UPPER_BOUND 122
#endif

#ifndef NUM_LOWER_BOUND
  #define NUM_LOWER_BOUND 48
#endif

#ifndef NUM_UPPER_BOUND
  #define NUM_UPPER_BOUND 57
#endif


void mutate_str(char **str)
{
        /* decide which char to mutate and by moving char val up or down */
        int index = randnum_i(0, strlen(*str) - 2);
        int diff = randnum_i(0, 100) <= 50 ? -1 : 1;
        int new_c = (*str)[index] + diff;

        /* if new_c is not a valid mutation, mutate again */
        while ((new_c <= CHAR_UPPER_BOUND && new_c >= CHAR_LOWER_BOUND) == 0) {
                diff = randnum_i(0, 100) <= 50 ? -1 : 1;
                new_c = (*str)[index] + diff;
        }

        (*str)[index] = (int) (*str)[index] + diff;
}

void mutate_num(char **num_str)
{
        int i = 0;
        int num_index = 0;
        char num[10];
        int index = randnum_i(0, strlen(*num_str) - 2);
        int new_num = (int) num[randnum_i(0, 9)];

        /* create num array */
        for (i = NUM_LOWER_BOUND; i <= NUM_UPPER_BOUND; i++) {
                num[num_index] = i;
                num_index++;
        }

        /* if new_base is not a valid mutation, mutate again */
        while ((new_num = num[randnum_i(0, num_index - 1)]) == (*num_str)[index]) {}

        (*num_str)[index] = new_num;
}

void mutate_dna(char **dna_str)
{
        char dna[4] = { 'A', 'C' , 'G', 'T' };
        int index = randnum_i(0, strlen(*dna_str) - 2);
        char new_base;

        /* if new_base is not a valid mutation, mutate again */
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
