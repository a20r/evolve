#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "evolve_monitor.h"

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

static void print_evolve_results(struct population *p)
{
        if (p->solution != NULL) {
                printf("SUCCESS! FOUND SOLUTION!\n");
                printf("Solution: %s\n", p->solution);
                printf("Score: %.2f\n", fitness_function(p->solution));
                printf("Goal: %.2f\n", p->goal);
                printf("Took %d Generations\n\n", p->curr_generation);
        } else {
                printf("Failed to find solution . . . \n\n");
        }
}

static void print_top_chromosomes(struct evolve_monitor *m, int top)
{
        int i = 0;

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_desc);

        /* print top 5 chromosomes */
        printf("\nTOP %d CHROMOSOMES:\n", top);
        if (m->generations->end != 1) {
                for (i = 0; i < top; i++) {
                        printf(
                                "chromosome: %s\n",
                                (char *) darray_get(m->best_chromosomes, i)
                        );
                        printf(
                                "score: %.2f\n",
                                *(float *) darray_get(m->best_scores, i)
                        );
                        printf(
                                "generation: %d\n",
                                *(int *) darray_get(m->generations, i)
                        );
                        printf(
                                "convergence rate: %.2f\n",
                                *(float *) darray_get(m->convergence_rates, i)
                        );
                        printf(
                                "goal distance: %.2f\n\n",
                                *(float *) darray_get(m->goal_distances, i)
                        );
                }
        }
}

int main(int argc, char *argv[])
{
        int max_pop = 100;
        int max_gen = 10000;
        float p_c = (argv[1] == NULL) ? 0.8 : atof(argv[1]);
        float p_m = (argv[1] == NULL) ? 0.1 : atof(argv[2]);

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
                max_gen,
                "hello_world.dat"
        );

        /* run evolution */
        printf("RUNNING GA!\n");
        printf("Crossover Probability [%.4f]!\n", p_c);
        printf("Mutation Probability [%.4f]!\n", p_m);
        gen_init_chromosomes(&p, randstr);
        run_evolution(
                &p,
                fitness_function,

                /* selection */
                roulette_wheel_selection,
                DEFAULT_SELECT,

                /* crossover */
                one_pt_crossover,
                p_c, /* crossover probability */
                DEFAULT_PIVOT,

                /* mutation */
                mutate_str,
                p_m , /* mutation probability */

                m,
                0
        );

        /* print results */
        print_evolve_results(p);
        print_top_chromosomes(m, 5);

        /* clean up */
        destroy_evolve_monitor(&m);
        destroy_population(&p);

        return 0;
}
