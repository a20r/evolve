#ifndef __EVOLVE__
#define __EVOLVE__

#include "config.h"

/* ERRORS */
#define EEVAL "Evaluator is NULL! Check config->evaluate_population!"
#define EFREEFUNC "Free function is NULL! Check config->free_func!"
#define ECOPYFUNC "Copy function is NULL! Check config->copy_func!"
#define EGETSCORE "Get score is NULL! Check config->get_score!"
#define ECMP "Population individual comparator is NULL! Check config->cmp!"

#define ECFUNC "Crossover function is not set!"
#define ECPROB "Crossover probability is not set!"
#define EMFUNC "Mutation function is not set!"
#define EMPROB "Mutation probability is not set!"

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
