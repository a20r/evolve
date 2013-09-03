#include <dstruct/darray.h>
#include <dbg/dbg.h>
#include <al/comparator.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"


int run_evolution(
        struct population **p,
        float (*eval_func)(char *),
        void (*selection_func)(struct population **, int *),
        int *select,
        int (*crossover_func)(void **, void **, int),
        float crossover_prob,
        int pivot_index,
        void (*mutation_func)(char **),
        float mutate_prob,
        struct evolve_monitor *m,
        int wait_signal,
        volatile sig_atomic_t *stop_signal
)
{
        int max_gen = (*p)->max_generation;
        int goal_reached = 0;
        int last_gen = 0;

        /* loop til max_gen reached, goal achieved, or receive stop signal */
        while ((*p)->curr_generation < max_gen && *stop_signal == 0)
        {
                /* evaluate and record */
                goal_reached = evaluate_chromosomes(eval_func, &(*p));
                if (m != NULL) {
                        record_generation_stats(*p, m, float_cmp_desc);
                }

                /* stop condition */
                if (goal_reached) {
                        break;
                }

                /* extend max generations if in signal mode */
                last_gen = ((*p)->curr_generation + 1 == max_gen);
                if (wait_signal == 1 && last_gen == 1) {
                        extend_max_generation(*p, DEFAULT_EXPAND_RATE);
                        max_gen = (*p)->max_generation;
                        last_gen = 0;
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
