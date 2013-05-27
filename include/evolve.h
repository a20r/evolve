#ifndef EVOLVE_H
#define EVOLVE_H
#include <dstruct/darray.h>


/* STRUCTS */
struct population {
        /* chromosomes */
        struct darray *chromosomes;
        struct darray *chromosome_scores;

        /* fitness details */
        int parameters;
        float goal;

        /* evolution details */
        int curr_population;
        int curr_generation;
        int max_population;
        int max_generation;
};

struct chromosome_pair {
        void *child_1;
        void *child_2;
};

/* FUNCTIONS */
int randnum_i(int l_bound, int u_bound);
char *randstr(int length);
struct population *init_population(int param, float goal, int max_pop, int max_gen);
void clean_population(struct population **p);
void gen_init_chromosomes(struct population **p, char *(*mutator)(int));
void evaluate_chromosomes(float (eval_func)(char *), struct population **p);
void run_evolution(struct population **p, float (eval_func)(char *));

#endif
