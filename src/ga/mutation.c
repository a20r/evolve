#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <al/utils.h>

#include "ga/evolve.h"

char ALPHA[52] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

char NUMERIC[10] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

char ALPHANUM[62] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

char DNA[4] = {
        'A', 'C' , 'G', 'T'
};


void mutate_str(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        int diff = randnum_i(0, 100) <= 50 ? -1 : 1;
        int new_c = (*str)[index] + diff;

        /* if new_c is not a valid mutation, mutate again */
        if ((new_c >= 32 && new_c <= 126) == 0) {
                diff = diff * -1;
        }

        (*str)[index] = (*str)[index] + diff;
}

void mutate_alphanum(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        char new_c;

        /* if new_c is same as previous, mutate again */
        while ((new_c = ALPHANUM[randnum_i(0, 61)]) == (*str)[index]) {}

        (*str)[index] = new_c;
}

void mutate_num(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        char new_num;

        /* if new_num is same as previous, mutate again */
        while((new_num = NUMERIC[randnum_i(0, 9)]) == (*str)[index]) {}

        (*str)[index] = new_num;
}

void mutate_dna(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        char new_base;

        /* if new_base is same as previous, mutate again */
        while ((new_base = DNA[randnum_i(0, 3)]) == (*str)[index]) {}

        (*str)[index] = new_base;
}

void mutate(
        void **chromosome,
        float *mutate_prob,
        void (mutate_func)(char **)
)
{
        /* to mutate or not mutate */
        if (*mutate_prob > randnum_f(0.0, 1.0)) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        (*mutate_func)((char **) chromosome);
no_mutate:
        return;
}
