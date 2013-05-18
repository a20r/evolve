#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mitosis.h"

float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += roundf(target[i] - chromosome[i]);
        }

        return total;
}

int main()
{
        /* initialize mitosis */
        struct mitosis *m = init_mitosis(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                20,  /* max_pop */
                5000  /* max_gen */
        );
        init_str_chromosomes(m, str_mutator);

        int i = 0;
        for (i = 0; i < m->population; i++) {
                printf("%d: %s\n", i, darray_get(m->chromosomes, i));
        }

        return 0;
}
