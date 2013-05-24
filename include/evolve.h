#ifndef EVOLVE_H
#define EVOLVE_H
#include <dstruct/darray.h>


/* STRUCTS */
struct evolve {
        /* chromosomes */
        struct darray *chromosomes;

        /* fitness details */
        int parameters;
        float goal;

        /* mitosis details */
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
struct evolve *init_evolve(int param, float goal, int max_pop, int max_gen);
void clean_evolve(struct evolve **e);
void init_chromosomes(struct evolve **e, char *(*mutator)(int));
void run_evolution(struct evolve **e, float (eval_func)(char *));

#endif
