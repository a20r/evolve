#include <dstruct/darray.h>
#include <dbg/dbg.h>
#include <al/comparator.h>

#include "evolve.h"
#include "population.h"


int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        void (*selection_func)(struct population **, int *),
        int *select,
        int (*crossover_func)(void **, void **, int),
        float crossover_prob,
        int pivot_index,
        void (*mutation_func)(char **),
        float mutate_prob,
        struct evolve_monitor *m
)
{
        int max_gen = (*p)->max_generation;

        /* evolve until max_gen reached or goal achieved  */
        while ((*p)->curr_generation < max_gen)
        {
                /* evaluate */
                if (evaluate_chromosomes(eval_func, &(*p))) {
                        if (m != NULL) {
                                record_generation_stats(*p, m, float_cmp_desc);
                        }
                        break;
                }

                /* record */
                if (m != NULL) {
                        record_generation_stats(*p, m, float_cmp_desc);
                }

                /* select */
                selection_func(&(*p), select);

                /* populate population for next generation run */
                populate(
                        &(*p),
                        crossover_func,
                        crossover_prob,
                        pivot_index,
                        mutation_func,
                        mutate_prob
                );

                (*p)->curr_generation++;
        }

        return 0;
}
