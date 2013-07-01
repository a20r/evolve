#include <dbg/dbg.h>
#include <al/comparator.h>

#include "selection.h"
#include "evolve.h"
#include "utils.h"


void roulette_wheel_selection(
        struct population **p,
        int *select
)
{
        int i = 0;
        float randnum = 0.0;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        size_t chromo_sz = (*p)->chromosomes->element_size;
        int arr_index = 0;
        void *score;
        void *chromo;
        int selected = 0;
        int max_selection = select == NULL ? (*p)->max_population / 2 : *select;
        struct population *new_p;

        new_p = init_population(
                (*p)->parameters,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->curr_generation = (*p)->curr_generation;
        new_p->solution = (*p)->solution;

        debug("Performing roulette selection!");

        /* noramlize fitness values and sort by descending order */
        normalize_fitness_values(&(*p));
        sort_population(&(*p), float_cmp);

        /* make sure number of selection is an even number! */
        if (max_selection % 2 != 0) max_selection += 1;

        /* select chromosomes */
        for (i = 0; i < (*p)->max_population; i++) {
                randnum = randnum_f(0.0, 1.0);

                /* chromosome score and bit-string */
                score = darray_new((*p)->chromosome_scores);
                chromo = darray_new((*p)->chromosomes);

                memcpy(score, darray_get((*p)->chromosome_scores, i), score_sz);
                memcpy(chromo, darray_get((*p)->chromosomes, i), chromo_sz);

                /* select if chromosome score is larger than random number */
                if (*(float *) score > randnum) {
                        darray_set(new_p->chromosomes, arr_index, chromo);
                        darray_set(new_p->chromosome_scores, arr_index, score);

                        selected++;
                        arr_index++;
                        new_p->curr_population += 1;
                } else {
                        free(score);
                        free(chromo);
                }

                /* loop conditions */
                if (selected == max_selection) {
                        break;
                } else if ((i + 1) >= (*p)->max_population) {
                        i = 0;
                }
        }

        /* clean up */
        destroy_population(&(*p));
        *p = new_p;
}
