#ifndef MITOSIS_H
#define MITOSIS_H
#include <dstruct/darray.h>


/* STRUCTS */
struct evolve {
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

struct chromosome_pair {
        void *child_1;
        void *child_2;
};

/* FUNCTIONS */
struct evolve *init_evolve(int param, float goal, int max_pop, int max_gen);
void init_str_chromosomes(struct evolve **e, char *(*mutator)(int));

char *rand_str(int length);
struct chromosome_pair *str_mate(char *c_1, char *c_2);
void mutate_gene_str(
        struct chromosome_pair **g_pair,
        float mutate_chance,
        void (mutator)(char **)
);
void str_mutator(char **gene);

#endif
