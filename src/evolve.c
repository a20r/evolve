#include <dstruct/darray.h>
#include <dbg/dbg.h>
#include <al/comparator.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"
#include "selection.h"
#include "utils.h"


int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        float crossover_prob,
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
                roulette_wheel_selection(&(*p), DEFAULT_SELECT);

                /* populate population for next generation run */
                populate(
                        &(*p),
                        crossover_prob,
                        mutate_prob
                );

                (*p)->curr_generation++;
        }

        return 0;
}
