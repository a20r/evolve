#ifndef _POPULATION_H_
#define _POPULATION_H_

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
void populate(
        struct population **p,
        float crossover_prob,
        float mutation_prob
);

#endif
