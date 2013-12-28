#ifndef _EVOLVE_MONITOR_H_
#define _EVOLVE_MONITOR_H_

#include <stdio.h>
#include <stdlib.h>

#include "population.h"

/* DEFINES */
#ifndef QSORT_MIN_SIZE
#define QSORT_MIN_SIZE 20
#endif


/* STRUCT */
struct evolve_monitor
{
        long top;

        /* leader board */
        struct darray *best_individuals;
        struct darray *best_scores;
        struct darray *generations;
        struct darray *convergence_rates;
        struct darray *goal_distances;

        /* current generation stats */
        char *individual;
        float score;
        float generation;
        float convergence_rate;
        float goal_distance;

        int log_stats;
        FILE *log_fp;
};

/* FUNCTIONS */
struct evolve_monitor *init_evolve_monitor(
        size_t chromo_sz,
        long top,
        char *log_fp
);
void destroy_evolve_monitor(struct evolve_monitor **m);
void destroy_evolve_monitor_tree(
        struct evolve_monitor *m,
        void (*free_func)(void *)
);
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
void sort_generation_stats(
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
);
void print_generation_stats(struct evolve_monitor *m);
#endif
