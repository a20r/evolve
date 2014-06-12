#ifndef __EVOLVE__
#define __EVOLVE__

#include "config.h"


/* STRUCTS */
struct stats
{
    int generation;
    int population_size;

    int stale_counter;
    float best_score;
    void *best;

    int individuals_evaluated;
    float diversity;
};


/* FUNCTIONS */
struct stats *stats_new(void);
void stats_destroy(void *stats);
void stats_update(struct population *p, struct stats *s, struct config *c);

struct population *evolve_reproduce(struct population *p, struct config *c);
struct population *evove_gp(struct config *c);
int evolve_terminate(struct stats *s, struct config *c);
struct population *evolve_gp(struct config *c);

#endif
