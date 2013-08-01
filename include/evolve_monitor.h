#ifndef _EVOLVE_MONITOR_H_
#define _EVOLVE_MONITOR_H_

#include <dbg/dbg.h>

#include "utils.h"

/* DEFINES */
#define QSORT_MIN_SIZE 20


/* STRUCT */
struct evolve_monitor
{
        struct darray *best_chromosomes;
        struct darray *best_scores;
        struct darray *generations;
};

/* FUNCTIONS */
struct evolve_monitor *init_evolve_monitor(size_t chromo_sz, long max_gen);
void destroy_evolve_monitor(struct evolve_monitor **m);

#include "evolve.h"
char *find_best_chromosome(struct population *p);
void record_generation_stats(struct population *p, struct evolve_monitor *m);
void sort_generation_stats(
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
);
void print_generation_stats(struct evolve_monitor *m);
#endif
