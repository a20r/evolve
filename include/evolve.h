#ifndef __EVOLVE__
#define __EVOLVE__

#include "config.h"

/* ERRORS */
#define E_EVAL "Evaluator is NULL! Check config->evaluate_population!"
#define E_FREE_FUNC "Free function is NULL! Check config->free_func!"
#define E_COPY_FUNC "Copy function is NULL! Check config->copy_func!"
#define E_GET_SCORE "Get score is NULL! Check config->get_score!"
#define E_CMP "Population individual comparator is NULL! Check config->cmp!"

/* MESSAGES */
#define MAX_GEN_REACHED "Max generation reached!"
#define STALE_LIMIT_REACHED "Stale limit reached!"
#define TARGET_SCORE_REACHED "Target score reached!"


/* STRUCTS */
struct stats
{
    int generation;
    int population_size;

    int stale_counter;
    void *best;
    float best_score;

    int individuals_evaluated;
    float diversity;
};


/* FUNCTIONS */
struct stats *stats_new(void);
void stats_destroy(void *stats, struct config *c);
void stats_update(struct population *p, struct config *c, struct stats *s);

struct population *evolve_reproduce(struct population *p, struct config *c);
struct population *evove_gp(struct config *c);
int evolve_terminate(struct stats *s, struct config *c);
void evolve_gp(struct config *c, struct data *d);

#endif
