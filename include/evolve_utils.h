#ifndef _EVOLVE_UTILS_H_
#define _EVOLVE_UTILS_H_

#include <dbg/dbg.h>

#include "evolve.h"


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
void sort_population(
        struct population *p,
        int (*cmp)(const void *, const void *)
);

#endif
