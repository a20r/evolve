#ifndef _EVOLVE_H_
#define _EVOLVE_H_

#include <dstruct/darray.h>



/* STRUCTS */
struct population {
        /* chromosomes */
        struct darray *chromosomes;
        struct darray *chromosome_scores;
        float total_score;

        /* fitness details */
        int parameters;
        float goal;

        /* evolution details */
        int curr_population;
        int curr_generation;
        int max_population;
        int max_generation;
        char *solution;
};

/* FUNCTIONS */
struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen
);
void destroy_population(struct population **p);

void gen_init_chromosomes(struct population **p, char *(*mutator)(int));
int evaluate_chromosomes(float (eval_func)(char *), struct population **p);

void normalize_fitness_values(struct population **p);
void sort_population(
        struct population **p,
        int (*cmp)(const void *, const void *)
);
int populate(
        struct population **p,
        float crossover_prob,
        float mutation_prob
);

#include "evolve_monitor.h"

int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        float crossover_prob,
        float mutate_prob,
        struct evolve_monitor **m
);

#endif
