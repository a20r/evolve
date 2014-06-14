#ifndef __POPULATION__
#define __POPULATION__

#include <stdio.h>


/* STRUCTURES */
struct population
{
    int generation;
    int size;

    void **individuals;
    size_t individual_size;
};


/* FUNCTIONS */
struct population *population_new(int size, size_t individual_size);
void population_destroy(struct population *p, void (*free_func)(void *));
void *population_best(
    struct population *p,
    float *(*get_score)(void *t),
    int (*cmp)(const void *, const void *)
);

#endif
