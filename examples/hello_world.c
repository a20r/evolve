#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "evolve.h"

float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}

int main()
{
        /* initialize evolution */
        struct population *p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                1 /* max_gen */
        );
        run_evolution(&p, fitness_function);

        return 0;
}
