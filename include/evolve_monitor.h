#ifndef _EVOLVE_MONITOR_H_
#define _EVOLVE_MONITOR_H_

#include <stdlib.h>

#include "utils.h"

/* DEFINES */
#ifndef QSORT_MIN_SIZE
#define QSORT_MIN_SIZE 20
#endif


/* STRUCT */
struct evolve_monitor
{
        struct darray *best_chromosomes;
        struct darray *best_scores;
        struct darray *generations;
        struct darray *convergence_rates;
        struct darray *goal_distances;
};

/* FUNCTIONS */
struct evolve_monitor *init_evolve_monitor(size_t chromo_sz, long max_gen);
void destroy_evolve_monitor(struct evolve_monitor **m);

#include "evolve.h"
void record_generation_stats(
        struct population *p,
        struct evolve_monitor *m,
        int(*cmp)(const void *, const void *)
);
void insertion_sort_gstats(
        struct evolve_monitor *m,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
void quick_sort_gstats(
        struct evolve_monitor *m,
        int left,
        int right,
        int(*cmp)(const void *, const void *)
);
int partition_gstats(
        struct evolve_monitor *m,
        int pivot_index,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
void sort_generation_stats(
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
);
void print_generation_stats(struct evolve_monitor *m);
#endif
