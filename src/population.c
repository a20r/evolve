#include <stdlib.h>

#include <dstruct/darray.h>
#include <al/comparator.h>
#include <al/utils.h>
#include <dbg/dbg.h>

#include "population.h"
#include "ga/crossover.h"
#include "ga/mutation.h"


struct population *population_create(
        size_t individual_size,
        float goal,
        int max_pop,
        int max_gen
)
{
        struct population *p;

        p = malloc(sizeof(struct population));
        check((max_pop % 2) == 0, "Max population has to be even!");
        check(max_gen > 0, "Max generation has to be bigger than 0!");

        /* individuals */
        p->individuals = darray_create(individual_size, max_pop);
        p->scores = darray_create(sizeof(float), max_pop);
        p->total_score = 0.0;

        /* population details */
        p->population = 0;
        p->generation = 0;
        p->max_population = max_pop;
        p->max_generation = max_gen;
        p->solution = NULL;

        /* fitness details */
        p->goal = goal;

        return p;
error:
        free(p);
        return p;
}

void population_destroy(struct population **p, void (*free_func)(void *))
{
        int i = 0;

        if ((*p)) {
                /* individuals */
                if ((*p)->individuals) {
                        for (i = 0; i < (*p)->population; i++) {
                                free_func(darray_get((*p)->individuals, i));
                        }
                }
                darray_destroy((*p)->individuals);

                /* scores */
                release_mem((*p)->scores, darray_clear_destroy);

                free(*p);
                *p = NULL;
        }
}

void extend_max_generation(struct population *p, int extension_size)
{
        p->max_generation += extension_size;
}

int evaluate_individuals(float (eval_func)(char *), struct population **p)
{
        int i = 0;
        float *goal = &(*p)->goal;
        int goal_reached = 0;
        float *old_score;
        float *score;
        char *individual;
        float epsilon = 0.001;

        for (i = 0; i < (*p)->max_population; i++) {
                /* obtain and evaluate individual from population */
                individual = darray_get((*p)->individuals, i);
                score = malloc(sizeof(float));
                *score = (eval_func)(individual);

                /* check goal is reached with the current individual */
                if (float_epsilon_cmp_asc(score, goal, epsilon) == 0) {
                        (*p)->solution = individual;
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

void print_individual(struct population *p, int index)
{
        char *chromo = (char *) darray_get(p->individuals, index);
        float score = *(float *) darray_get(p->scores, index);

        printf("individual [%s]\n", chromo);
        printf("individual score: %f\n\n", score);
}

void print_individuals(struct population *p)
{
        int i = 0;
        float score = 0.0;
        char *chromo = NULL;
        float total_score = (double) p->total_score;

        for (i = 0; i < p->population; i++) {
                chromo = (char *) darray_get(p->individuals, i);
                score = *(float *) darray_get(p->scores, i);

                printf("individual [%s]\n", chromo);
                printf( "individual score: [%f]\n\n", score);
        }

        printf("individual total_score: %f\n\n", total_score);
}

void print_population(struct population *p)
{
        printf("population[goal]: %f\n", p->goal);
        printf("population[population]: %d\n", p->population);
        printf("population[max_population]: %d\n", p->max_population);
        printf("population[generation]: %d\n", p->generation);
        printf("population[max_generation]: %d\n\n", p->max_generation);

        print_individuals(p);
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
        size_t chromo_sz = p->individuals->element_size;
        void *score;
        void *chromo;

        /* below implements an insertion sort */
        for (j = left + 1; j <= right; j++) {
                int i = j - 1;

                /* obtain individual and score */
                chromo = darray_new(p->individuals);
                score = darray_new(p->scores);

                memcpy(chromo, darray_get(p->individuals, j), chromo_sz);
                memcpy(score, darray_get(p->scores, j), score_sz);

                /* very important! */
                free(darray_get(p->individuals, j));
                free(darray_get(p->scores, j));

                while (i >= 0 && cmp(darray_get(p->scores, i), score) > 0) {
                        /* individual */
                        darray_set(
                                p->individuals,
                                (i + 1),
                                darray_get(p->individuals, i)
                        );

                        /* individual score */
                        darray_set(
                                p->scores,
                                (i + 1),
                                darray_get(p->scores, i)
                        );

                        i--;
                }

                /* individual and score */
                darray_set(p->individuals, (i + 1), chromo);
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
        swap_darray_elem(p->individuals, pivot_index, right);
        swap_darray_elem(p->scores, pivot_index, right);

        /* values <= pivot, moved to front of pivot
         * values > pivot, insert after pivot */
        store = left;
        for (i = left; i < right; i++) {
                if (cmp(darray_get(p->scores, i), pivot) <= 0) {

                        swap_darray_elem(p->individuals, i, store);
                        swap_darray_elem(p->scores, i, store);

                        store++;
                }
        }

        swap_darray_elem(p->individuals, right, store);
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
        quick_sort_population(p, 0, p->individuals->end, cmp);
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
        int child_pair = 0;

        void *p_1;  /* parents 1 */
        void *p_2;  /* parents 2 */
        void *c_1;  /* child 1 */
        void *c_2;  /* child 2 */

        struct darray *old_chromos = (*p)->individuals;
        int population = (*p)->population;

        /* initialize new population */
        struct population *new_p = population_create(
                (*p)->individuals->element_size,
                (*p)->goal,
                (*p)->max_population,
                (*p)->max_generation
        );
        new_p->generation = (*p)->generation;
        new_p->solution = (*p)->solution;
        struct darray *new_chromos = new_p->individuals;

        /* crossover and mutate */
        for (i = 0; i < population; i += 2) {
                /* obtain parents for reproduction */
                p_1 = darray_get(old_chromos, i);
                p_2 = darray_get(old_chromos, i + 1);

                for (j = 0; j < 2; j++) {
                        /* make sure number of offspring < max_population */
                        if (i + j + child_pair == (*p)->max_population) {
                                break;
                        }

                        /* prepare children c_1 and c_2 */
                        c_1 = calloc(1, old_chromos->element_size + 1);
                        c_2 = calloc(1, old_chromos->element_size + 1);
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
                        darray_set(new_chromos, i + j + child_pair, c_1);
                        darray_set(new_chromos, i + j + child_pair + 1, c_2);
                        new_p->population += 2;
                        child_pair++;
                }
        }

        /* clean up */
        population_destroy(&(*p), free);
        *p = new_p;
}
