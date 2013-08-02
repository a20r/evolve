#include <dstruct/darray.h>
#include <dbg/dbg.h>

#include "evolve.h"
#include "evolve_utils.h"
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
        char *chromo;
        float score;

        /* evolve until max_gen reached or goal achieved  */
        while ((*p)->curr_generation < max_gen)
        {
                int i = (*p)->curr_generation;

                /* evaluate */
                if (evaluate_chromosomes(eval_func, &(*p))) {
                        break;
                }

                /* record */
                if (m != NULL) {
                        record_generation_stats(*p, m);
                        chromo = (char *) darray_get(m->best_chromosomes, i);
                        score = *(float *) darray_get(m->best_scores, i);

                        debug("GENERATION: %d", i);
                        debug("BEST CHROMOSOME: %s [%f]\n", chromo, score);
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
