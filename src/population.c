#include <stdlib.h>

#include <dstruct/darray.h>
#include <al/comparator.h>
#include <al/utils.h>

#include "population.h"
#include "crossover.h"
#include "mutation.h"


struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen
)
{
        struct population *p = malloc(sizeof(struct population));

        /* checks */
        check(param > 0, "Parameters has to be bigger than 0!");
        check((max_pop % 2) == 0, "Max population has to be even!");
        check(max_gen > 0, "Max generation has to be bigger than 0!");

        /* chromosomes */
        p->chromosomes = darray_create(sizeof(char) * (param + 1), max_pop);
        p->scores = darray_create(sizeof(float), max_pop);
        p->total_score = 0.0;

        /* population details */
        p->population = 0;
        p->generation = 0;
        p->max_population = max_pop;
        p->max_generation = max_gen;
        p->solution = NULL;

        /* fitness details */
        p->parameters = param;
        p->goal = goal;

        return p;
error:
        free(p);
        return p;
}

void destroy_population(struct population **p)
{
        if ((*p)) {

                if ((*p)->chromosomes){
                        darray_clear_destroy((*p)->chromosomes);
                }

                if ((*p)->scores) {
                        darray_clear_destroy((*p)->scores);
                }

                free(*p);
                *p = NULL;
        }
}

void extend_max_generation(struct population *p, int extension_size)
{
        p->max_generation += extension_size;
}

void gen_init_chromosomes(struct population **p, char *(*mutator)(int))
{
        int i = 0;
        int param = (*p)->parameters;

        check(param != 0, "Parameters should be greater than 0!");

        /* fill initial random chromosome */
        for (i = 0; i < (*p)->max_population; i++) {
                darray_set((*p)->chromosomes, i, (*mutator)(param));
                (*p)->population++;
        }

error:
        return;
}

int evaluate_chromosomes(float (eval_func)(char *), struct population **p)
{
        int i = 0;
        float *goal = &(*p)->goal;
        int goal_reached = 0;
        float *old_score;
        float *score;
        char *chromosome;
        float epsilon = 0.001;

        for (i = 0; i < (*p)->max_population; i++) {
                /* obtain and evaluate chromosome from population */
                chromosome = darray_get((*p)->chromosomes, i);
                score = malloc(sizeof(float));
                *score = (eval_func)(chromosome);

                /* check goal is reached with the current chromosome */
                if (float_epsilon_cmp_asc(score, goal, epsilon) == 0) {
                        (*p)->solution = chromosome;
                        goal_reached = 1;
                }

                /* set the score and total_score */
                old_score = darray_get((*p)->scores, i);
                if (old_score != NULL) {
                        free(old_score);
                }
                darray_set((*p)->scores, i, score);
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
                score = darray_get((*p)->scores, i);

                /* normalize score and set it back into scores*/
                normalized_score = malloc(sizeof(float));
                memset(normalized_score, 0, sizeof(float));

                *normalized_score = *score / total_score;
                memcpy(score, normalized_score, sizeof(float));

                /* clean up */
                free(normalized_score);
       }
}

void print_chromosome(struct population *p, int index)
{
        char *chromo = (char *) darray_get(p->chromosomes, index);
        float score = *(float *) darray_get(p->scores, index);

        printf("chromosome [%s]\n", chromo);
        printf("chromosome score: %f\n\n", score);
}

void print_chromosomes(struct population *p)
{
        int i = 0;
        float score = 0.0;
        char *chromo = '\0';
        float total_score = (double) p->total_score;

        for (i = 0; i < p->population; i++) {
                chromo = (char *) darray_get(p->chromosomes, i);
                score = *(float *) darray_get(p->scores, i);

                printf("chromosome [%s]\n", chromo);
                printf( "chromosome score: [%f]\n\n", score);
        }

        printf("chromosome total_score: %f\n\n", total_score);
}

void print_population(struct population *p)
{
        printf("population[paramters]: %d\n", p->parameters);
        printf("population[goal]: %f\n", p->goal);
        printf("population[population]: %d\n", p->population);
        printf("population[max_population]: %d\n", p->max_population);
        printf("population[generation]: %d\n", p->generation);
        printf("population[max_generation]: %d\n\n", p->max_generation);

        print_chromosomes(p);
}

static void swap_darray_elem(struct darray *arr, int index_1, int index_2)
{
        void *tmp_ptr = darray_get(arr, index_1);
        darray_set(arr, index_1, darray_get(arr, index_2));
        darray_set(arr, index_2, tmp_ptr);
}

void insertion_sort_population(
        struct population *p,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
)
{
        int j;
        size_t score_sz = p->scores->element_size;
        size_t chromo_sz = p->chromosomes->element_size;
        void *score;
        void *chromo;

        /* below implements an insertion sort */
        for (j = left + 1; j <= right; j++) {
                int i = j - 1;

                /* obtain chromosome and score */
                chromo = darray_new(p->chromosomes);
                score = darray_new(p->scores);

                memcpy(chromo, darray_get(p->chromosomes, j), chromo_sz);
                memcpy(score, darray_get(p->scores, j), score_sz);

                /* very important! */
                free(darray_get(p->chromosomes, j));
                free(darray_get(p->scores, j));

                while (
                        i >= 0 &&
                        cmp(
                                darray_get(p->scores, i),
                                score
                        ) > 0
                ) {
                        /* chromosome */
                        darray_set(
                                p->chromosomes,
                                (i + 1),
                                darray_get(p->chromosomes, i)
                        );

                        /* chromosome score */
                        darray_set(
                                p->scores,
                                (i + 1),
                                darray_get(p->scores, i)
                        );

                        i--;
                }

                /* chromosome and score */
                darray_set(p->chromosomes, (i + 1), chromo);
                darray_set(p->scores, (i + 1), score);
        }
}

int partition_population(
        struct population *p,
        int pivot_index,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
)
{
        int i = 0;
        int store = 0;

        /* pivot value */
        void *pivot = darray_get(p->scores, pivot_index);


        /* move pivot elements to the end of the array */
        swap_darray_elem(p->chromosomes, pivot_index, right);
        swap_darray_elem(p->scores, pivot_index, right);

        /* values <= pivot, moved to front of pivot
         * values > pivot, insert after pivot */
        store = left;
        for (i = left; i < right; i++) {
                if (cmp(darray_get(p->scores, i), pivot) <= 0) {

                        swap_darray_elem(p->chromosomes, i, store);
                        swap_darray_elem(p->scores, i, store);

                        store++;
                }
        }

        swap_darray_elem(p->chromosomes, right, store);
        swap_darray_elem(p->scores, right, store);

        return store;
}

void quick_sort_population(
        struct population *m,
        int left,
        int right,
        int(*cmp)(const void *, const void *)
)
{
        if (right <= left) {
                return;
        }

        /* partition */
        int pivot_index = randnum_i(left, right);
        pivot_index = partition_population(m, pivot_index, left, right, cmp);

        if (pivot_index - 1 - left <= QSORT_MIN_SIZE) {
                insertion_sort_population(m, left, pivot_index - 1, cmp);
        } else {
                quick_sort_population(m, left, pivot_index - 1, cmp);
        }

        if (right-pivot_index-1 <= QSORT_MIN_SIZE) {
                insertion_sort_population(m, pivot_index + 1, right, cmp);
        } else {
                quick_sort_population(m, pivot_index + 1, right, cmp);
        }
}

void sort_population(
        struct population *p,
        int (*cmp)(const void *, const void *)
)
{
        quick_sort_population(p, 0, p->chromosomes->end, cmp);
}

void populate(
        struct population **p,
        int (*crossover_func)(void **, void **, int),
        float *crossover_prob,
        int pivot_index,
        void (*mutation_func)(char **),
        float *mutate_prob
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

        struct darray *old_chromos = (*p)->chromosomes;
        int population = (*p)->population;


        /* initialize new population */
        struct population *new_p = init_population(
                (*p)->parameters,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->generation = (*p)->generation;
        new_p->solution = (*p)->solution;
        struct darray *new_chromos = new_p->chromosomes;

        /* crossover and mutate */
        for (i = 0; i < population; i += 2) {
                /* obtain parents for reproduction */
                c_1_len = strlen(darray_get(old_chromos, i));
                c_2_len = strlen(darray_get(old_chromos, i + 1));
                p_1 = darray_get(old_chromos, i);
                p_2 = darray_get(old_chromos, i + 1);

                for (j = 0; j < 2; j++) {
                        /* make sure number of offspring < max_population */
                        if (i + j + offspring_pair  == (*p)->max_population) {
                                break;
                        }

                        /* prepare children c_1 and c_2 */
                        c_1 = calloc(1, sizeof(char) * (c_1_len + 1));
                        c_2 = calloc(1, sizeof(char) * (c_2_len + 1));
                        memcpy(c_1, p_1, old_chromos->element_size);
                        memcpy(c_2, p_2, old_chromos->element_size);

                        /* crossover and mutate */
                        crossover(
                                &c_1,
                                &c_2,
                                pivot_index,
                                crossover_func,
                                crossover_prob
                        );
                        mutate(&c_1, mutate_prob, mutation_func);
                        mutate(&c_2, mutate_prob, mutation_func);

                        /* put children into new population */
                        darray_set(new_chromos, i + j + offspring_pair, c_1);
                        darray_set(new_chromos, i + j + offspring_pair + 1, c_2);
                        new_p->population += 2;
                        offspring_pair++;
                }
        }

        /* clean up */
        destroy_population(&(*p));
        *p = new_p;
}
