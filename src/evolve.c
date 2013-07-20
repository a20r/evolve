#include <dstruct/darray.h>
#include <dbg/dbg.h>

#include "evolve.h"
#include "evolve_utils.h"
#include "selection.h"
#include "crossover.h"
#include "mutate.h"


struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen
)
{
        struct population *p = malloc(sizeof(struct population));

        debug("Initializing struct population");

        /* chromosomes */
        p->chromosomes = darray_create(sizeof(char) * (param + 1), max_pop);
        p->chromosome_scores = darray_create(sizeof(float), max_pop);
        p->total_score = 0.0;

        /* population details */
        p->curr_population = 0;
        p->curr_generation = 0;
        p->max_population = max_pop;
        p->max_generation = max_gen;
        p->solution = NULL;

        /* fitness details */
        p->parameters = param;
        p->goal = goal;

        return p;
}

void destroy_population(struct population **p)
{
        debug("Destroying population!");
        if ((*p)) {

                if ((*p)->chromosomes){
                        darray_clear_destroy((*p)->chromosomes);
                }

                if ((*p)->chromosome_scores) {
                        darray_clear_destroy((*p)->chromosome_scores);
                }

                free(*p);
                *p = NULL;
        }
}


void gen_init_chromosomes(struct population **p, char *(*mutator)(int))
{
        int i = 0;
        int param = (*p)->parameters;

        check(param != 0, "Parameters should not be 0!");

        /* fill initial random chromosome */
        for (i = 0; i < (*p)->max_population; i++) {
                darray_set((*p)->chromosomes, i, (*mutator)(param));
                (*p)->curr_population++;
        }

error:
        return;
}

int evaluate_chromosomes(float (eval_func)(char *), struct population **p)
{
        int i = 0;
        int goal_reached = 0;
        float *old_score;
        float *score;
        char *chromosome;

        debug("Evaluating chromosomes!");
        for (i = 0; i < (*p)->max_population; i++) {
                /* obtain and evaluate chromosome from population */
                chromosome = darray_get((*p)->chromosomes, i);
                score = malloc(sizeof(float));
                *score = (eval_func)(chromosome);

                /* check goal is reached with the current chromosome */
                if (*score == (*p)->goal) {
                        (*p)->solution = chromosome;
                        goal_reached = 1;
                }

                /* set the score and total_score */
                old_score = darray_get((*p)->chromosome_scores, i);
                if (old_score != NULL) {
                        free(old_score);
                }
                darray_set((*p)->chromosome_scores, i, score);
                (*p)->total_score += *score;
        }

        if (goal_reached == 1) {
                return 1;
        } else {
                return 0;
        }
}

void normalize_fitness_values(struct population **p)
{
       int i = 0;
       float total_score = (*p)->total_score;
       float *score;
       float *normalized_score;

       debug("Normalizing chromsome scores!");
       for (i = 0; i < (*p)->max_population; i++) {
                /* get score */
                score = darray_get((*p)->chromosome_scores, i);

                /* normalize score and set it back into scores*/
                normalized_score = malloc(sizeof(float));
                memset(normalized_score, 0, sizeof(float));

                *normalized_score = *score / total_score;
                memcpy(score, normalized_score, sizeof(float));

                /* clean up */
                free(normalized_score);
       }
}

void sort_population(
        struct population **p,
        int (*cmp)(const void *, const void *)
)
{
        int j;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        size_t chromo_sz = (*p)->chromosomes->element_size;
        void *score;
        void *chromo;

        /* below implements an insertion sort */
        for (j = 1; j < (*p)->max_population; j++) {
                int i = j - 1;

                /* obtain chromosome and score */
                chromo = darray_new((*p)->chromosomes);
                score = darray_new((*p)->chromosome_scores);

                memcpy(chromo, darray_get((*p)->chromosomes, j), chromo_sz);
                memcpy(score, darray_get((*p)->chromosome_scores, j), score_sz);

                /* very important! */
                free(darray_get((*p)->chromosomes, j));
                free(darray_get((*p)->chromosome_scores, j));

                while (
                        i >= 0 &&
                        cmp(
                                darray_get((*p)->chromosome_scores, i),
                                score
                        ) < 0
                ) {
                        /* chromosome */
                        darray_set(
                                (*p)->chromosomes,
                                (i + 1),
                                darray_get((*p)->chromosomes, i)
                        );

                        /* chromosome score */
                        darray_set(
                                (*p)->chromosome_scores,
                                (i + 1),
                                darray_get((*p)->chromosome_scores, i)
                        );

                        i--;
                }

                /* chromosome and score */
                darray_set((*p)->chromosomes, (i + 1), chromo);
                darray_set((*p)->chromosome_scores, (i + 1), score);
        }
}

int populate(
        struct population **p,
        float crossover_prob,
        float mutate_prob
)
{
        int i = 0;
        int j = 0;
        struct darray *chromosomes = (*p)->chromosomes;
        int population = (*p)->curr_population + 2;
        int e = 0;
        void *p_1;  /* parents 1 */
        void *p_2;  /* parents 2 */
        void *c_1;  /* child 1 */
        void *c_2;  /* child 2 */
        int c_1_len = 0;
        int c_2_len = 0;

        debug("Populating!");

        /* crossover and mutate */
        for (i = 0; i < population; i += 2) {
                c_1_len = strlen(darray_get(chromosomes, i));
                c_2_len = strlen(darray_get(chromosomes, i + 1));

                /* obtain parents for reproduction */
                p_1 = darray_get(chromosomes, i);
                p_2 = darray_get(chromosomes, i + 1);

                for (j = 0; j < 2; j++) {
                        /* prepare children */
                        c_1 = calloc(1, sizeof(char) * (c_1_len + 1));
                        c_2 = calloc(1, sizeof(char) * (c_2_len + 1));
                        memcpy(c_1, p_1, chromosomes->element_size);
                        memcpy(c_2, p_2, chromosomes->element_size);

                        /* crossover and mutate */
                        crossover(
                                &c_1,
                                &c_2,
                                DEFAULT_PIVOT,
                                one_ptr_crossover,
                                crossover_prob
                        );
                        mutate(&c_1, mutate_prob, mutate_str);
                        mutate(&c_2, mutate_prob, mutate_str);

                        /* gen 4 offsprings (or 2 if last chromosome set) */
                        if (j == 0 && ((i + 2) < population)) {
                                e = chromosomes->end;
                                darray_set(chromosomes, e + 1, c_1);
                                darray_set(chromosomes, e + 2, c_2);
                                (*p)->curr_population += 2;
                        } if (j == 0 && ((i + 2) == population)) {
                                darray_update(chromosomes, i, c_1);
                                darray_update(chromosomes, i + 1, c_2);
                                break;
                        } else if (j == 1) {
                                darray_update(chromosomes, i, c_1);
                                darray_update(chromosomes, i + 1, c_2);
                        }
                }
        }

        return 0;
}

int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        float crossover_prob,
        float mutate_prob,
        struct evolve_monitor **m
)
{
        int max_gen = (*p)->max_generation;

        debug("Running Evolution!");

        /* evolve until max_gen reached or goal achieved  */
        while ((*p)->curr_generation < max_gen)
        {
                debug("GENERATION: %d", (*p)->curr_generation);

                /* evaluate */
                if (evaluate_chromosomes(eval_func, &(*p))) {
                        break;
                }

                /* record */
                if (m != NULL) {
                        record_generation_stats(*p, &(*m));
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
