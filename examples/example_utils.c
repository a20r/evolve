#include <stdio.h>

#include <al/comparator.h>
#include <dstruct/darray.h>

#include "example_utils.h"


void print_evolve_results(struct population *p, float (*fitness_func)(void *))
{
        if (p->solution != NULL) {
                printf("SUCCESS! FOUND SOLUTION!\n");
                printf("Solution: %s\n", (char *) p->solution);
                printf("Score: %.2f\n", fitness_func(p->solution));
                printf("Goal: %.2f\n", p->goal);
                printf("Took %d Generations\n", p->generation);
        } else {
                printf("Failed to find solution . . . \n");
        }
}

void print_top_individuals(struct evolve_monitor *m, int top)
{
        int i = 0;
        char *chromo;
        float score;
        int generation;
        float rate;
        float goal_dist;

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_desc);

        /* print top individuals */
        if (m->top >= top) {
                printf("\nTOP %d INDIVIDUALS:\n", top);

                for (i = 0; i < top; i++) {
                        chromo = (char *) darray_get(m->best_individuals, i);
                        score = *(float *) darray_get(m->best_scores, i);
                        generation = *(int *) darray_get(m->generations, i);
                        rate = *(float *) darray_get(m->convergence_rates, i);
                        goal_dist = *(float *) darray_get(m->goal_distances, i);

                        printf("individual: %s\n", chromo);
                        printf("score: %.2f\n", score);
                        printf("generation: %d\n", generation);
                        printf("convergence rate: %.2f\n", rate);
                        printf("goal distance: %.2f\n\n", goal_dist);
                }
        }
}
