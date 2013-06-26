#include <dbg/dbg.h>
#include <al/comparator.h>

#include "selection.h"
#include "evolve.h"


static struct population *next_gen_population(struct population **p)
{
        struct population *new_p = malloc(sizeof(struct population));

        /* chromosomes */
        new_p->chromosomes = darray_create(
                (*p)->chromosomes->element_size,
                (*p)->max_population
        );
        new_p->chromosome_scores = darray_create(
                (*p)->chromosome_scores->element_size,
                (*p)->max_population
        );
        new_p->total_score = 0.0;

        /* population details */
        new_p->curr_population = 0;
        new_p->curr_generation = (*p)->curr_generation;
        new_p->max_population = (*p)->max_population;
        new_p->max_generation = (*p)->max_generation;
        new_p->solution = (*p)->solution;

        /* fitness details */
        new_p->parameters = (*p)->parameters;
        new_p->goal = (*p)->goal;

        return new_p;
}

struct population *roulette_wheel_selection(
        struct population **p,
        int *select
)
{
        int i = 0;
        float randnum = 0.0;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        size_t chromo_sz = (*p)->chromosomes->element_size;
        void *score;
        void *chromo;
        int selected = 0;
        int max_selected = select == NULL ? (*p)->max_population / 2 : *select;
        struct population *new_p = next_gen_population(&(*p));

        log_info("Performing roulette selection!");

        /* noramlize fitness values and sort by descending order */
        normalize_fitness_values(&(*p));
        sort_population(&(*p), float_cmp);
        print_population((*p));

        /* select chromosomes */
        for (i = 0; i < (*p)->max_population; i++) {
                randnum = randnum_f(0.0, 1.0);

                /* chromosome score and bit-string */
                score = malloc(score_sz);
                chromo = malloc(chromo_sz);
                memset(score, '\0', score_sz);
                memset(chromo, '\0', chromo_sz);

                memcpy(score, darray_get((*p)->chromosome_scores, i), score_sz);
                memcpy(chromo, darray_get((*p)->chromosomes, i), chromo_sz);

                /* select if chromosome score is larger than random number */
                if (*(float *) score > randnum) {
                        debug("chromosome [%s] <--- SELECTED!", (char *) chromo);
                        debug("score: %f", *(float *) score);
                        debug("randnum: %f", randnum);
                        debug("");

                        darray_set(new_p->chromosomes, new_p->curr_population, chromo);
                        darray_set(new_p->chromosome_scores, new_p->curr_population, score);

                        debug("");

                        selected++;
                        new_p->curr_population += 1;
                }

                /* loop conditions */
                if (selected == max_selected) {
                        break;
                } else if ((i + 1) >= (*p)->max_population) {
                        i = 0;
                }
        }

        /* clean up */
        destroy_population(&(*p));

        return new_p;
}
