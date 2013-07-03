#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "evolve.h"
#include "utils.h"

#define TARGET_SOLUTION "hello world!"


float fitness_function(char *chromosome)
{
        char *target = TARGET_SOLUTION;
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}


int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* initialize evolution */
        struct population *pop = init_population(
                (int) strlen(TARGET_SOLUTION),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                1000000 /* max_gen */
        );

        gen_init_chromosomes(&pop, randstr);
        run_evolution(
                &pop,
                fitness_function,
                0.8,
                0.2,
                NULL
        );

        destroy_population(&pop);

        return 0;
}
