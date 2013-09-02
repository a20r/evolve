#include <stdlib.h>
#include <math.h>

#include <al/utils.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"


struct evolve_monitor *init_evolve_monitor(
        size_t chromo_sz,
        long max_gen,
        char *log_fp
)
{
        struct evolve_monitor *m = malloc(sizeof(struct evolve_monitor));

        m->best_chromosomes = darray_create(chromo_sz, max_gen);
        m->best_scores = darray_create(sizeof(float), max_gen);
        m->generations = darray_create(sizeof(long), max_gen);
        m->convergence_rates = darray_create(sizeof(float), max_gen);
        m->goal_distances = darray_create(sizeof(float), max_gen);

        if (log_fp != NULL) {
                m->log_stats = 1;
                m->log_fp = fopen(log_fp, "w");
                if (m->log_fp == NULL) {
                        printf("Failed to open [%s] for logging!\n", log_fp);
                        exit(-1);
                }
        } else {
                m->log_stats = 0;
                m->log_fp = NULL;
        }

        return m;
}

void destroy_evolve_monitor(struct evolve_monitor **m)
{
        darray_clear_destroy((*m)->best_chromosomes);
        darray_clear_destroy((*m)->best_scores);
        darray_clear_destroy((*m)->generations);
        darray_clear_destroy((*m)->convergence_rates);
        darray_clear_destroy((*m)->goal_distances);
        if ((*m)->log_fp != NULL) fclose((*m)->log_fp);
        free(*m);

        *m = NULL;
}

int expand_evolve_monitor(struct evolve_monitor *m)
{
        int result = 0;

        result = darray_expand(m->best_chromosomes);
        result = darray_expand(m->best_scores);
        result = darray_expand(m->generations);
        result = darray_expand(m->convergence_rates);
        result = darray_expand(m->goal_distances);

        check(result == 0, "Failed to expand evolve monitor!");

        return 0;
error:
        return -1;
}

void log_generation_stats(struct evolve_monitor *m, int generation)
{
        char *chromosome = darray_get(m->best_chromosomes, generation);
        float *score = darray_get(m->best_scores, generation);
        float *conv_rate = darray_get(m->convergence_rates, generation);
        float *goal_dist = darray_get(m->goal_distances, generation);

        fprintf(m->log_fp, "%s\n", chromosome);
        fprintf(m->log_fp, "%f\n", *score);
        fprintf(m->log_fp, "%d\n", generation);
        fprintf(m->log_fp, "%f\n", *conv_rate);
        fprintf(m->log_fp, "%f\n\n", *goal_dist);
}

void record_generation_stats(
        struct population *p,
        struct evolve_monitor *m,
        int(*cmp)(const void *, const void *)
)
{
        int i = 0;
        float goal = p->goal;
        int generation = p->curr_generation;
        size_t chromo_sz = p->chromosomes->element_size;
        size_t score_sz = p->chromosome_scores->element_size;

        char *chromo = calloc(1, chromo_sz);
        float *score = calloc(1, score_sz);
        char *best_chromo = calloc(1, chromo_sz);
        float *best_score = calloc(1, score_sz);

        /* instanciate inital best chromosome */
        memcpy(best_chromo, darray_get(p->chromosomes, 0), chromo_sz);
        memcpy(best_score, darray_get(p->chromosome_scores, 0), score_sz);

        /* find the best chromosome */
        for (i = 1; i < p->curr_population; i++) {
                memcpy(chromo, darray_get(p->chromosomes, i), chromo_sz);
                memcpy(score, darray_get(p->chromosome_scores, i), score_sz);

                /* set value 1 and value 2 for comparison */
                float *val_1 = calloc(1, sizeof(float));
                float *val_2 = calloc(1, sizeof(float));
                *val_1 = *score - goal;
                *val_2 = *best_score - goal;

                if (cmp(val_1, val_2) < 1) {
                        memcpy(best_chromo, chromo, chromo_sz);
                        memcpy(best_score, score, score_sz);
                }

                free(val_1);
                free(val_2);
        }

        /* record generation statistics into evolve_monitor */
        int *gen = darray_new(m->generations);
        float *last_best_score;
        float *conv_rate = darray_new(m->convergence_rates);
        float *goal_dist = darray_new(m->goal_distances);

        *gen = generation;
        if (generation != 0) {
                last_best_score = darray_get(m->best_scores, generation - 1);
                *conv_rate = fabs(*last_best_score - *best_score);
        } else {
                *conv_rate = 0;
        }
        *goal_dist = fabs(p->goal - *best_score);

        /* printf("GEN: %d\n", generation); */
        /* printf("BEST CHROMO: %s\n", best_chromo); */
        /* printf("BEST SCORE: %f\n", *(float *) best_score); */
        darray_set(m->best_chromosomes, generation, best_chromo);
        darray_set(m->best_scores, generation, best_score);
        darray_set(m->generations, generation, gen);
        darray_set(m->convergence_rates, generation, conv_rate);
        darray_set(m->goal_distances, generation, goal_dist);

        /* log generation stats */
        if (m->log_stats) {
                log_generation_stats(m, generation);
        }

        /* cleanup */
        free(chromo);
        free(score);
}

static void swap_darray_elem(struct darray *arr, int index_1, int index_2)
{
        void *tmp_ptr = darray_get(arr, index_1);
        darray_set(arr, index_1, darray_get(arr, index_2));
        darray_set(arr, index_2, tmp_ptr);
}

void insertion_sort_gstats(
        struct evolve_monitor *m,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
)
{
        int j;

        void *score;
        void *chromo;
        void *gen;
        void *conv_rate;
        void *goal_dist;

        /* below implements an insertion sort - sort by ASCENDING ORDER */
        for (j = left + 1; j <= right; j++) {
                int i = j - 1;

                /* obtain pointers pointing to chromosome and score */
                chromo = darray_get(m->best_chromosomes, j);
                score = darray_get(m->best_scores, j);
                gen = darray_get(m->generations, j);
                conv_rate = darray_get(m->convergence_rates, j);
                goal_dist = darray_get(m->goal_distances, j);

                while (
                        i >= 0 &&
                        cmp(
                                darray_get(m->best_scores, i),
                                score
                        ) > 0
                ) {
                        /* chromosome */
                        darray_set(
                                m->best_chromosomes,
                                i + 1,
                                darray_get(m->best_chromosomes, i)
                        );

                        /* score */
                        darray_set(
                                m->best_scores,
                                i + 1,
                                darray_get(m->best_scores, i)
                        );

                        /* generation */
                        darray_set(
                                m->generations,
                                i + 1,
                                darray_get(m->generations, i)
                        );

                        /* convergence rate */
                        darray_set(
                                m->convergence_rates,
                                i + 1,
                                darray_get(m->convergence_rates, i)
                        );

                        /* goal distance */
                        darray_set(
                                m->goal_distances,
                                i + 1,
                                darray_get(m->goal_distances, i)
                        );

                        i--;
                }

                darray_set(m->best_chromosomes, i + 1, chromo);
                darray_set(m->best_scores, i + 1, score);
                darray_set(m->generations, i + 1, gen);
                darray_set(m->convergence_rates, i + 1, conv_rate);
                darray_set(m->goal_distances, i + 1, goal_dist);
        }
}

int partition_gstats(
        struct evolve_monitor *m,
        int pivot_index,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
)
{
        int i = 0;
        int store = 0;

        /* pivot value */
        void *pivot = darray_get(m->best_scores, pivot_index);


        /* move pivot elements to the end of the array */
        swap_darray_elem(m->best_chromosomes, pivot_index, right);
        swap_darray_elem(m->best_scores, pivot_index, right);
        swap_darray_elem(m->generations, pivot_index, right);
        swap_darray_elem(m->convergence_rates, pivot_index, right);
        swap_darray_elem(m->goal_distances, pivot_index, right);

        /* values <= pivot, moved to front of pivot
         * values > pivot, insert after pivot */
        store = left;
        for (i = left; i < right; i++) {
                if (cmp(darray_get(m->best_scores, i), pivot) <= 0) {

                        swap_darray_elem(m->best_chromosomes, i, store);
                        swap_darray_elem(m->best_scores, i, store);
                        swap_darray_elem(m->generations, i, store);
                        swap_darray_elem(m->convergence_rates, i, store);
                        swap_darray_elem(m->goal_distances, i, store);

                        store++;
                }
        }

        swap_darray_elem(m->best_chromosomes, right, store);
        swap_darray_elem(m->best_scores, right, store);
        swap_darray_elem(m->generations, right, store);
        swap_darray_elem(m->convergence_rates, right, store);
        swap_darray_elem(m->goal_distances, right, store);

        return store;
}

void quick_sort_gstats(
        struct evolve_monitor *m,
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
        pivot_index = partition_gstats(m, pivot_index, left, right, cmp);

        if (pivot_index - 1 - left <= QSORT_MIN_SIZE) {
                insertion_sort_gstats(m, left, pivot_index - 1, cmp);
        } else {
                quick_sort_gstats(m, left, pivot_index - 1, cmp);
        }

        if (right - pivot_index - 1 <= QSORT_MIN_SIZE) {
                insertion_sort_gstats(m, pivot_index + 1, right, cmp);
        } else {
                quick_sort_gstats(m, pivot_index + 1, right, cmp);
        }
}

void sort_generation_stats(
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
)
{
        int right = m->generations->end;
        quick_sort_gstats(m, 0, right, cmp);
}

void print_generation_stats(struct evolve_monitor *m)
{
        int i = 0;
        int elements = m->generations->end + 1;
        struct darray *c = m->best_chromosomes;
        struct darray *s = m->best_scores;
        struct darray *g = m->generations;
        struct darray *conv_rates = m->convergence_rates;
        struct darray *goal_dists = m->goal_distances;

        for (i = 0; i < elements; i++) {
                printf("GENERATION: %d\n", *(int *) darray_get(g, i));
                printf("chromosome: [%s]\n", (char *) darray_get(c, i));
                printf("score: %.2f\n", *(float *) darray_get(s, i));
                printf(
                        "convergence rate: %.2f\n",
                        *(float *) darray_get(conv_rates, i)
                );
                printf(
                        "goal distance: %.2f\n\n",
                        *(float *) darray_get(goal_dists, i)
                );
        }
}
