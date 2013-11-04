#include <math.h>
#include <unistd.h>

#include <dbg/dbg.h>
#include <al/comparator.h>
#include <al/utils.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "population.h"
#include "selection.h"


static struct population *create_empty_population(struct population *p)
{
        struct population *new_p;
        new_p = population_create(
                p->individuals->element_size,
                p->goal,
                p->max_population,
                p->max_generation
        );
        new_p->generation = p->generation;
        new_p->solution = p->solution;

        return new_p;
}

void roulette_wheel_selection(struct population **p, int *select)
{
        int i = 0;
        int selected = 0;
        int max_select = 0;
        float randnum = 0.0;

        float curr_score;
        void *score;
        void *chromo;
        size_t chromo_sz;
        size_t score_sz;

        struct population *new_p;
        float cumulative_prob = 0.0;
        int max_pop;

        /* setup */
        chromo_sz = (*p)->individuals->element_size;
        score_sz = (*p)->scores->element_size;
        new_p = create_empty_population(*p);
        max_select = (select == NULL ? (*p)->max_population / 2 : *select);

        /* noramlize fitness values */
        normalize_fitness_values(&(*p));

        /* make sure number of selection is an even number! */
        if (max_select % 2 != 0) {
                max_select += 1;
        }

        /* select individuals */
        max_pop = (*p)->max_population;
        while (selected != max_select) {

                /* spin the wheel */
                randnum = randnum_f(0.0, 1.0);
                cumulative_prob = 0.0;
                for (i = 0; i < max_pop; i++) {

                        /* select if score is larger than random number */
                        curr_score = *(float *) darray_get((*p)->scores, i);
                        cumulative_prob += curr_score;
                        if (cumulative_prob >= randnum) {
                                /* make a copy of individual and score */
                                chromo = calloc(1, chromo_sz);
                                score = calloc(1, score_sz);
                                memcpy(
                                        chromo,
                                        darray_get((*p)->individuals, i),
                                        chromo_sz
                                );
                                memcpy(
                                        score,
                                        darray_get((*p)->scores, i),
                                        score_sz
                                );

                                /* set the selected individual and score */
                                darray_set(
                                        new_p->individuals,
                                        selected,
                                        chromo
                                );
                                darray_set(
                                        new_p->scores,
                                        selected,
                                        score
                                );

                                selected++;
                                new_p->population++;
                                break;
                        }
                }
        }

        population_destroy(&(*p), free);
        *p = new_p;
}

static void sort_tournament(
        struct darray *chromos,
        struct darray *scores,
        int (*cmp)(const void *, const void *)
)
{
        int i;
        int j;
        void *score;
        void *chromo;
        size_t score_sz = scores->element_size;
        size_t chromo_sz = chromos->element_size;

        /* below implements an insertion sort */
        for (j = 1; j <= chromos->end; j++) {
                i = j - 1;

                /* obtain individual and score */
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
                        /* individual */
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

                /* individual and score */
                darray_set(chromos, (i + 1), chromo);
                darray_set(scores, (i + 1), score);
        }
}

void tournament_selection(struct population **p, int *select)
{
        int i = 0;
        int j = 0;
        int max_select = 0;
        int t_size = 2;  /* tournament size */
        float r = 0.0;

        void *chromo;
        void *score;
        size_t chromo_sz;
        size_t score_sz;

        struct population *new_p;
        struct darray *t_chromos;  /* tournament individuals */
        struct darray *t_scores;  /* tournament scores */


        /* setup */
        chromo_sz = (*p)->individuals->element_size;
        score_sz = (*p)->scores->element_size;
        t_chromos = darray_create(chromo_sz, t_size);
        t_scores = darray_create(score_sz, t_size);
        max_select = (select == NULL ? (*p)->max_population / 2 : *select);
        max_select = (max_select % 2 == 0) ? max_select : max_select + 1;
        new_p = create_empty_population(*p);

        /* make sure number of selection is an even number! */
        if (max_select % 2 != 0) {
                max_select += 1;
        }

        /* tournament selection */
        for (j = 0; j < max_select; j++) {

                /* randomly select N individuals (i.e tournament size) */
                for (i = 0; i < t_size; i++) {
                        r = randnum_i(0, (*p)->population - 1);

                        /* individual and score */
                        chromo = calloc(1, chromo_sz);
                        score = calloc(1, score_sz);
                        memcpy(
                                chromo,
                                darray_get((*p)->individuals, r),
                                chromo_sz
                        );
                        memcpy(
                                score,
                                darray_get((*p)->scores, r),
                                score_sz
                        );

                        if (j == 0) {
                                darray_set(t_chromos, i, chromo);
                                darray_set(t_scores, i, score);
                        } else {
                                /* second+ update to avoid mem leaks */
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

                darray_set(new_p->individuals, j, chromo);
                darray_set(new_p->scores, j, score);
                new_p->population += 1;
        }

        /* clean up */
        darray_clear_destroy(t_chromos);
        darray_clear_destroy(t_scores);
        population_destroy(&(*p), free);
        *p = new_p;
}
