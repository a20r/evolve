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

        /* create initial random chromosomes */
        init_str_chromosomes(m, str_mutator);
        /* int i = 0; */
        /* for (i = 0; i < m->population; i++) { */
        /*         printf("%d: %s\n", i, darray_get(m->chromosomes, i)); */
        /* } */

        struct gene_pair *g_pair = str_mate("hello world!", "world hello!");

        printf("child 1: %s\n", g_pair->child_1);
        printf("child 2: %s\n", g_pair->child_2);
        printf("str len: %d\n", (int) strlen(g_pair->child_1));

        return 0;
}
