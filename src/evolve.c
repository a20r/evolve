#include <dstruct/darray.h>
#include <dbg/dbg.h>

#include "evolve.h"
#include "evolve_utils.h"
#include "selection.h"
#include "crossover.h"
#include "mutate.h"
#include "utils.h"


struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen
)
{
        struct population *p = malloc(sizeof(struct population));

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

void populate(
        struct population **p,
        float crossover_prob,
        float mutate_prob
)
{
        int i = 0;
        int j = 0;
        int offspring_pair = 0;

        void *p_1;  /* parents 1 */
        void *p_2;  /* parents 2 */
        void *c_1;  /* child 1 */
        void *c_2;  /* child 2 */

        int c_1_len = 0;
        int c_2_len = 0;

        struct darray *old_chromosomes = (*p)->chromosomes;
        int population = (*p)->curr_population;


        /* initialize new population */
        struct population *new_p = init_population(
                (*p)->parameters,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->curr_generation = (*p)->curr_generation;
        new_p->solution = (*p)->solution;
        struct darray *new_chromosomes = new_p->chromosomes;

        /* crossover and mutate */
        for (i = 0; i < population; i += 2) {
                /* obtain parents for reproduction */
                c_1_len = strlen(darray_get(old_chromosomes, i));
                c_2_len = strlen(darray_get(old_chromosomes, i + 1));
                p_1 = darray_get(old_chromosomes, i);
                p_2 = darray_get(old_chromosomes, i + 1);

                for (j = 0; j < 2; j++) {
                        /* make sure number of offspring < max_population */
                        if (i + j + offspring_pair  == (*p)->max_population) {
                                break;
                        }

                        /* prepare children c_1 and c_2 */
                        c_1 = calloc(1, sizeof(char) * (c_1_len + 1));
                        c_2 = calloc(1, sizeof(char) * (c_2_len + 1));
                        memcpy(c_1, p_1, old_chromosomes->element_size);
                        memcpy(c_2, p_2, old_chromosomes->element_size);

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

                        /* put children into new population */
                        darray_set(new_chromosomes, i + j + offspring_pair, c_1);
                        darray_set(new_chromosomes, i + j + offspring_pair + 1, c_2);
                        new_p->curr_population += 2;
                        offspring_pair++;
                }
        }

        /* clean up */
        destroy_population(&(*p));
        *p = new_p;
}

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
                debug("GENERATION: %d\n", (*p)->curr_generation);

                /* evaluate */
                if (evaluate_chromosomes(eval_func, &(*p))) {
                        break;
                }

                /* record */
                if (m != NULL) {
                        record_generation_stats(*p, m);
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
