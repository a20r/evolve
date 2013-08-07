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


static float fitness_function(char *chromosome)
{
        char *target = TARGET_SOLUTION;
        float max_score = 122 * strlen(chromosome);
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return max_score - total;
}

int main()
{
        int i = 0;
        int max_pop = 100;
        int max_gen = 10000;

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* initialize evolution */
        struct population *p = init_population(
                (int) strlen(TARGET_SOLUTION),  /* param */
                122 * strlen(TARGET_SOLUTION),  /* goal */
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

        /* print solution */
        if (p->solution != NULL) {
                printf("SUCCESS! FOUND SOLUTION!\n");
                printf("Solution: %s\n", p->solution);
                printf("Score: %.2f\n", fitness_function(p->solution));
                printf("Goal: %.2f\n", p->goal);
                printf("Took %d Generations\n", p->curr_generation);
        } else {
                printf("Failed to find solution . . . \n");
        }

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_desc);

        /* print top 5 chromosomes */
        printf("\nTOP 5 CHROMOSOMES:\n");
        for (i = 0; i < 5; i++) {
                printf(
                        "chromosome: %s\n",
                        (char *) darray_get(m->best_chromosomes, i)
                );
                printf(
                        "score: %.2f\n",
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
