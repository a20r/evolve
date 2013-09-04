#include <math.h>
#include <unistd.h>

#include <dbg/dbg.h>
#include <al/comparator.h>
#include <al/utils.h>
#include <dstruct/darray.h>

#include "selection.h"
#include "population.h"
#include "evolve.h"


void roulette_wheel_selection(
        struct population **p,
        int *select
)
{
        int i = 0;
        float randnum = 0.0;
        size_t chromo_sz = (*p)->chromosomes->element_size;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        int arr_index = 0;
        void *score;
        void *chromo;
        int selected = 0;
        int max_select = (select == NULL ? (*p)->max_population / 2 : *select);
        struct population *new_p;
        float cumulative_prob = 0.0;


        /* initialize new population */
        new_p = init_population(
                (*p)->parameters,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->curr_generation = (*p)->curr_generation;
        new_p->solution = (*p)->solution;

        /* noramlize fitness values and sort by descending order */
        normalize_fitness_values(&(*p));
        sort_population(*p, float_cmp_desc);

        /* make sure number of selection is an even number! */
        if (max_select % 2 != 0) {
                max_select += 1;
        }

        /* select chromosomes */
        randnum = randnum_f(0.0, 1.0);
        for (i = 0; i < (*p)->max_population; i++) {

                /* chromosome and score */
                chromo = calloc(1, chromo_sz);
                score = calloc(1, score_sz);
                memcpy(chromo, darray_get((*p)->chromosomes, i), chromo_sz);
                memcpy(score, darray_get((*p)->chromosome_scores, i), score_sz);

                /* select if chromosome score is larger than random number */
                cumulative_prob += *(float *) score;
                if (cumulative_prob >= randnum) {
                        darray_set(new_p->chromosomes, arr_index, chromo);
                        darray_set(new_p->chromosome_scores, arr_index, score);

                        i = 0; /* reset wheel */
                        cumulative_prob = 0.0;  /* reset cumulative_prob */
                        randnum = randnum_f(0.0, 1.0);  /* reset probability */
                        selected++;
                        arr_index++;
                        new_p->curr_population += 1;
                } else {
                        free(score);
                        free(chromo);
                }

                /* loop conditions */
                if (selected == max_select) {
                        break;
                } else if ((i + 1) >= (*p)->max_population) {
                        i = 0; /* reset wheel */
                        cumulative_prob = 0.0;  /* reset cumulative_prob */
                        randnum = randnum_f(0.0, 1.0);  /* reset probability */
                }
        }

        /* clean up */
        destroy_population(&(*p));
        *p = new_p;
}

static void sort_tournament(
        struct darray *chromos,
        struct darray *scores,
        int (*cmp)(const void *, const void *)
)
{
        int j;
        void *score;
        void *chromo;
        size_t score_sz = scores->element_size;
        size_t chromo_sz = chromos->element_size;

        /* below implements an insertion sort */
        for (j = 1; j <= chromos->end; j++) {
                int i = j - 1;

                /* obtain chromosome and score */
                chromo = darray_new(chromos);
                score = darray_new(scores);
                memcpy(chromo, darray_get(chromos, j), chromo_sz);
                memcpy(score, darray_get(scores, j), score_sz);

                /* very important! */
                free(darray_get(chromos, j));
                free(darray_get(scores, j));

                while (
                        i >= 0 &&
                        cmp(
                                darray_get(scores, i),
                                score
                        ) > 0
                ) {
                        /* chromosome */
                        darray_set(
                                chromos,
                                (i + 1),
                                darray_get(chromos, i)
                        );

                        /* score */
                        darray_set(
                                scores,
                                (i + 1),
                                darray_get(scores, i)
                        );

                        i--;
                }

                /* chromosome and score */
                darray_set(chromos, (i + 1), chromo);
                darray_set(scores, (i + 1), score);
        }
}

void tournament_selection(
        struct population **p,
        int *select
)
{
        int i = 0;
        int j = 0;
        int max_select = 0;
        int t_size = 2;  /* tournament size */
        float r = 0.0;

        size_t chromo_sz = (*p)->chromosomes->element_size;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        void *chromo;
        void *score;

        struct population *new_p;
        struct darray *t_chromos;  /* tournament chromosomes */
        struct darray *t_scores;  /* tournament scores */


        /* setup */
        t_chromos = darray_create(chromo_sz, t_size);
        t_scores = darray_create(score_sz, t_size);

        /* make sure number of selection is an even number! */
        max_select = (select == NULL ? (*p)->max_population / 2 : *select);
        if (max_select % 2 != 0) {
                max_select += 1;
        }

        /* initialize new population */
        new_p = init_population(
                (*p)->parameters,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->curr_generation = (*p)->curr_generation;
        new_p->solution = (*p)->solution;

        /* tournament selection */
        for (j = 0; j < max_select; j++) {

                /* randomly select N chromosomes (i.e tournament size) */
                for (i = 0; i < t_size; i++) {
                        r = randnum_i(0, (*p)->curr_population - 1);

                        /* chromosome and score */
                        chromo = calloc(1, chromo_sz);
                        score = calloc(1, score_sz);
                        memcpy(
                                chromo,
                                darray_get((*p)->chromosomes, r),
                                chromo_sz
                        );
                        memcpy(
                                score,
                                darray_get((*p)->chromosome_scores, r),
                                score_sz
                        );

                        /* on first run set, second+ update */
                        if (j == 0) {
                                darray_set(t_chromos, i, chromo);
                                darray_set(t_scores, i, score);
                        } else {
                                darray_update(t_chromos, i, chromo);
                                darray_update(t_scores, i, score);
                        }
                }

                /* find the best by sorting */
                sort_tournament(t_chromos, t_scores, float_cmp_desc);

                /* record the best into new population */
                chromo = calloc(1, chromo_sz);
                score = calloc(1, score_sz);
                memcpy(chromo, darray_get(t_chromos, 0), chromo_sz);
                memcpy(score, darray_get(t_scores, 0), score_sz);

                darray_set(new_p->chromosomes, j, chromo);
                darray_set(new_p->chromosome_scores, j, score);
                new_p->curr_population += 1;
        }

        /* clean up */
        darray_clear_destroy(t_chromos);
        darray_clear_destroy(t_scores);
        destroy_population(&(*p));
        *p = new_p;
}
