#ifndef __EVOLVE__
#define __EVOLVE__

#include "config.h"


/* STRUCTS */
struct log
{
    int generation;
    int stale_counter;
    int population_size;

    int individuals_evaluated;
    float diversity;
};


/* FUNCTIONS */
struct log *log_new(void);
void log_destroy(void *stats);

struct population *evolve_reproduce(struct population *p, struct evolve *e);

#endif
