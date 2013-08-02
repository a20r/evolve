#include <stdio.h>

#include <dstruct/darray.h>

#include "evolve_utils.h"


void print_chromosome(struct population *p, int index)
{
        char *chromo = (char *) darray_get(p->chromosomes, index);
        float score = *(float *) darray_get(p->chromosome_scores, index);

        printf("chromosome [%s]\n", chromo);
        printf("chromosome score: %f\n\n", score);
}

void print_chromosomes(struct population *p)
{
        int i = 0;
        float score = 0.0;
        char *chromo = '\0';
        float total_score = (double) p->total_score;

        for (i = 0; i < p->curr_population; i++) {
                chromo = (char *) darray_get(p->chromosomes, i);
                score = *(float *) darray_get(p->chromosome_scores, i);

                printf("chromosome [%s]\n", chromo);
                printf( "chromosome score: [%f]\n\n", score);
        }

        printf("chromosome total_score: %f\n\n", total_score);
}

void print_population(struct population *p)
{
        printf("population[paramters]: %d\n", p->parameters);
        printf("population[goal]: %f\n", p->goal);
        printf("population[curr_population]: %d\n", p->curr_population);
        printf("population[max_population]: %d\n", p->max_population);
        printf("population[curr_generation]: %d\n", p->curr_generation);
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
        size_t score_sz = p->chromosome_scores->element_size;
        size_t chromo_sz = p->chromosomes->element_size;
        void *score;
        void *chromo;

        /* below implements an insertion sort */
        for (j = left + 1; j <= right; j++) {
                int i = j - 1;

                /* obtain chromosome and score */
                chromo = darray_new(p->chromosomes);
                score = darray_new(p->chromosome_scores);

                memcpy(chromo, darray_get(p->chromosomes, j), chromo_sz);
                memcpy(score, darray_get(p->chromosome_scores, j), score_sz);

                /* very important! */
                free(darray_get(p->chromosomes, j));
                free(darray_get(p->chromosome_scores, j));

                while (
                        i >= 0 &&
                        cmp(
                                darray_get(p->chromosome_scores, i),
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
                                p->chromosome_scores,
                                (i + 1),
                                darray_get(p->chromosome_scores, i)
                        );

                        i--;
                }

                /* chromosome and score */
                darray_set(p->chromosomes, (i + 1), chromo);
                darray_set(p->chromosome_scores, (i + 1), score);
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
        void *pivot = darray_get(p->chromosome_scores, pivot_index);


        /* move pivot elements to the end of the array */
        swap_darray_elem(p->chromosomes, pivot_index, right);
        swap_darray_elem(p->chromosome_scores, pivot_index, right);

        /* values <= pivot, moved to front of pivot
         * values > pivot, insert after pivot */
        store = left;
        for (i = left; i < right; i++) {
                if (cmp(darray_get(p->chromosome_scores, i), pivot) <= 0) {

                        swap_darray_elem(p->chromosomes, i, store);
                        swap_darray_elem(p->chromosome_scores, i, store);

                        store++;
                }
        }

        swap_darray_elem(p->chromosomes, right, store);
        swap_darray_elem(p->chromosome_scores, right, store);

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
        insertion_sort_population(p, 0, p->chromosomes->end, cmp);
}
