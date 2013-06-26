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

struct chromosome_pair {
        void *child_1;
        void *child_2;
};

/* FUNCTIONS */
int randnum_i(int l_bound, int u_bound);
float randnum_f(float l_bound, float u_bound);
char *randstr(int length);
struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen
);
void destroy_population(struct population **p);
void print_chromosome(struct population *p, int index);
void print_chromosomes(struct population *p);
void print_population(struct population *p);
void gen_init_chromosomes(struct population **p, char *(*mutator)(int));
int evaluate_chromosomes(float (eval_func)(char *), struct population **p);
void normalize_fitness_values(struct population **p);
void sort_population(
        struct population **p,
        int (*cmp)(const void *, const void *)
);
void run_evolution(struct population **p, float (eval_func)(char *));

#endif
