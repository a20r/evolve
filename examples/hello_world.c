#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <al/comparator.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"
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
        int i = 0;
        int max_pop = 100;
        int max_gen = 1000;

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* initialize evolution */
        struct population *p = init_population(
                (int) strlen(TARGET_SOLUTION),  /* param */
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen  /* max_gen */
        );
        struct evolve_monitor *m = init_evolve_monitor(
                p->chromosomes->element_size,  /* chromosome size */
                max_gen
        );

        /* run evolution */
        printf("RUNNING GA!\n");
        gen_init_chromosomes(&p, randstr);
        run_evolution(
                &p,
                fitness_function,
                0.8,
                0.1,
                m
        );

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_asc);

        /* print top 5 chromosomes */
        printf("\nTOP 5 CHROMOSOMES:\n");
        for (i = 0; i < 5; i++) {
                printf(
                        "chromosome: %s\n",
                        (char *) darray_get(m->best_chromosomes, i)
                );
                printf(
                        "score: %f\n",
                        *(float *) darray_get(m->best_scores, i)
                );
                printf(
                        "generation: %d\n\n",
                        *(int *) darray_get(m->generations, i)
                );
        }

        /* clean up */
        destroy_evolve_monitor(&m);
        destroy_population(&p);
        return 0;
}
