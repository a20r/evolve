#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <dstruct/darray.h>

/* DEFINES */
#define QSORT_MIN_SIZE 20

/* STRUCTS */
struct population {
        /* individuals */
        struct darray *individuals;
        struct darray *scores;
        float total_score;

        /* fitness details */
        float goal;

        /* evolution details */
        int population;
        int generation;
        int max_population;
        int max_generation;
        char *solution;
};

/* FUNCTIONS */
struct population *init_population(
        size_t individual_size,
        float goal,
        int max_pop,
        int max_gen
);
void destroy_population(struct population **p);
void extend_max_generation(struct population *p, int extension_size);
void gen_init_individuals(struct population **p, char *(*mutator)(int));
int evaluate_individuals(float (eval_func)(char *), struct population **p);
void normalize_fitness_values(struct population **p);
void print_individual(struct population *p, int index);
void print_individuals(struct population *p);
void print_population(struct population *p);
void insertion_sort_population(
        struct population *p,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
int partition_population(
        struct population *p,
        int pivot_index,
        int left,
        int right,
        int (*cmp)(const void *, const void *)
);
void quick_sort_population(
        struct population *m,
        int left,
        int right,
        int(*cmp)(const void *, const void *)
);
void sort_population(
        struct population *p,
        int (*cmp)(const void *, const void *)
);
void populate(
        struct population **p,
        int (*crossover_func)(void **, void **, int),
        float *crossover_prob,
        int pivot_index,
        void (*mutation)(char **),
        float *mutate_prob
);

#endif
