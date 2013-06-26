#include <dbg/dbg.h>

#include "selection.h"
#include "evolve.h"


struct population *roulette_wheel_selection(
        struct population **p
)
{
        int i = 0;
        float randnum = 0.0;
        float score;
        char *chromosome;
        int selected = 0;
        int max_selected = (*p)->max_population / 2;
        struct population *new_p = malloc(sizeof(struct population));

        /* chromosomes */
        new_p->chromosome_scores = darray_create(
                sizeof(float),
                (*p)->max_population
        );
        new_p->total_score = 0.0;

        /* population details */
        new_p->curr_population = (*p)->curr_population;
        new_p->curr_generation = (*p)->curr_generation;
        new_p->max_population = (*p)->max_population;
        new_p->max_generation = (*p)->max_generation;
        new_p->solution = NULL;

        /* fitness details */
        new_p->parameters = (*p)->parameters;
        new_p->goal = (*p)->goal;

        /* noramlize fitness values */
        normalize_fitness_values(&(*p));

        /* select chromosomes */
        for (i = 0; i < (*p)->max_population; i++) {
                randnum = randnum_f(0.0, 1.0);

                score = *(float *) darray_get((*p)->chromosome_scores, i);
                chromosome = (char *) darray_get((*p)->chromosomes, i);

                /* select if chromosome score is larger than random number */
                if (score > randnum) {
                        debug("chromosome [%s] <--- SELECTED!", chromosome);
                        debug("score: %f", score);
                        debug("randnum: %f", randnum);
                        debug("");

                        selected++;
                }

                /* break loop condition */
                if (selected == max_selected) {
                        break;
                }

                /* continue loop condition */
                if ((i + 1) < (*p)->max_population) {
                        i = 0;
                }
        }

        return new_p;
}
