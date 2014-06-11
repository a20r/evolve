#ifndef __EVOLVE__
#define __EVOLVE__

#include "config.h"


/* STRUCTS */
struct log
{
    int generation;
    int stale_counter;
    int population_size;
    float best_score;

    int individuals_evaluated;
    float diversity;
};


/* FUNCTIONS */
struct log *log_new(void);
void log_destroy(void *stats);

struct population *evolve_reproduce(struct population *p, struct config *c);
struct population *evove_gp(struct config *c);
int evolve_terminate(struct log *l, struct config *c);
struct population *evolve_gp(struct config *c);

#endif
