#ifndef _EVOLVE_UTILS_H_
#define _EVOLVE_UTILS_H_

#include "evolve.h"


/* DEFINES */
#define QSORT_MIN_SIZE 20


/* FUNCTIONS */
void print_chromosome(struct population *p, int index);
void print_chromosomes(struct population *p);
void print_population(struct population *p);
void insertion_sort_population(
        struct population *p,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
int partition_population(
        struct population *p,
        int pivot_index,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
void quick_sort_population(
        struct population *m,
        int left,
        int right,
        int(*cmp)(const void *, const void *)
);
void sort_population(
        struct population *p,
        int (*cmp)(const void *, const void *)
);

#endif
