#include <dstruct/darray.h>
#include <dbg/dbg.h>

#include "evolve.h"
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

        debug("population[paramters]: %d", p->parameters);
        debug("population[goal]: %f", p->goal);
        debug("population[max_population]: %d", p->max_population);
        debug("population[max_generation]: %d", p->max_generation);

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

void print_chromosome(struct population *p, int index)
{
        char *chromo = (char *) darray_get(p->chromosomes, index);
        float score = *(float *) darray_get(p->chromosome_scores, index);
        float total_score = (double) p->total_score;

        printf("chromosome [%s]\n", chromo);
        printf("chromosome score: %f\n", score);
        printf("chromosome total_score: %f\n\n", total_score);
}

void print_chromosomes(struct population *p)
{
        int i = 0;
        float score = 0.0;
        char *chromo = '\0';

        for (i = 0; i < p->curr_population; i++) {
                chromo = (char *) darray_get(p->chromosomes, i);
                score = *(float *) darray_get(p->chromosome_scores, i);

                printf("chromosome [%s]\n", chromo);
                printf( "chromosome score: [%f]\n\n", score);
        }
}

void print_population(struct population *p)
{
        printf("population[paramters]: %d\n", p->parameters);
        printf("population[goal]: %f\n", p->goal);
        printf("population[curr_population]: %d\n", p->curr_population);
        printf("population[max_population]: %d\n\n", p->max_population);
        printf("population[curr_generation]: %d\n", p->curr_generation);
        printf("population[max_generation]: %d\n", p->max_generation);

        print_chromosomes(p);
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
       char *chromosome;

       debug("Normalizing chromsome scores!");
       for (i = 0; i < (*p)->max_population; i++) {
                /* get score */
                chromosome = darray_get((*p)->chromosomes, i);
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
        int cmp_res = 0;
        size_t score_sz = (*p)->chromosome_scores->element_size;
        size_t chromo_sz = (*p)->chromosomes->element_size;
        void *score;
        void *chromo;

        /* below implements an insertion sort */
        for (j = 1; j < (*p)->max_population; j++) {
                int i = j - 1;

                /* chromosome score and bit-string */
                score = malloc(score_sz);
                chromo = malloc(chromo_sz);
                memset(score, '\0', score_sz);
                memset(chromo, '\0', chromo_sz);

                memcpy(score, darray_get((*p)->chromosome_scores, j), score_sz);
                memcpy(chromo, darray_get((*p)->chromosomes, j), chromo_sz);

                while (
                        i >= 0 &&
                        (cmp_res = cmp(
                                darray_get((*p)->chromosome_scores, i),
                                score
                        )) < 0
                ) {
                        /* chromosome score */
                        darray_set(
                                (*p)->chromosome_scores,
                                (i + 1),
                                darray_get((*p)->chromosome_scores, i)
                        );

                        /* chromosome bit-string */
                        darray_set(
                                (*p)->chromosomes,
                                (i + 1),
                                darray_get((*p)->chromosomes, i)
                        );

                        i--;
                }

                /* chromosome score and bit-string */
                darray_set((*p)->chromosome_scores, (i + 1), score);
                darray_set( (*p)->chromosomes, (i + 1), chromo);
        }
}

void run_evolution(
        struct population **p,
        float (eval_func)(char *)
)
{
        /* evolution vars */
        int max_gen = (*p)->max_generation;
        int goal_achieved = 0;

        debug("Running Evolution!");

        /* evolve until max_gen reached or goal achieved  */
        while ((*p)->curr_generation < max_gen && goal_achieved == 0)
        {
                debug("GENERATION: %d", (*p)->curr_generation);

                evaluate_chromosomes(eval_func, &(*p));
                roulette_wheel_selection(&(*p), NULL);
                print_population(*p);

                (*p)->curr_generation++;
        }

        /* clean up */
        destroy_population(&(*p));
        debug("Evolution Completed!");
}
