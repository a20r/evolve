#ifndef MITOSIS_H
#define MITOSIS_H
#include <dstruct/darray.h>


/* STRUCTS */
struct mitosis {
        /* chromosomes */
        struct darray *chromosomes;

        /* fitness details */
        int parameters;
        float goal;

        /* mitosis details */
        int population;
        int generation;
        int max_population;
        int max_generation;
};

struct gene_pair {
        void * child_1;
        void * child_2;
};

/* FUNCTIONS */
struct mitosis *init_mitosis(int param, float goal, int max_pop, int max_gen);
void init_str_chromosomes(struct mitosis *m, char *(*mutator)(int));
char *str_mutator(int parameters);
struct gene_pair *str_mate(char *c_1, char *c_2);

#endif
