#include <stdio.h>

#include <al/comparator.h>
#include <dstruct/darray.h>

#include "example_utils.h"


void print_evolve_results(struct population *p, float (*fitness_func)(char *))
{
        if (p->solution != NULL) {
                printf("SUCCESS! FOUND SOLUTION!\n");
                printf("Solution: %s\n", p->solution);
                printf("Score: %.2f\n", fitness_func(p->solution));
                printf("Goal: %.2f\n", p->goal);
                printf("Took %d Generations\n", p->generation);
        } else {
                printf("Failed to find solution . . . \n");
        }
}

void print_top_chromosomes(struct evolve_monitor *m, int top)
{
        int i = 0;

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_desc);

        /* print top chromosomes */
        if (m->top >= top) {
                printf("\nTOP %d CHROMOSOMES:\n", top);
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
