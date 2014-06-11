#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "population.h"


struct population *population_new(int size, size_t individual_size)
{
    int i;
    struct population *p = malloc(sizeof(struct population));

    p->generation = 0;
    p->size = size;

    p->individuals = malloc(individual_size * (unsigned long) size);
    p->individual_size = individual_size;

    /* initialize individuals to NULL */
    for (i = 0; i < size; i++) {
        p->individuals[i] = NULL;
    }

    return p;
}

void population_destroy(struct population *p, void (*free_func)(void *))
{
    int i;

    for (i = 0; i < p->size; i++) {
        free_func(p->individuals[i]);
    }

    free(p->individuals);
    free(p);
}

void *population_best(
    struct population *p,
    int (*cmp)(const void *, const void *)
)
{
    int i;
    void *contender = NULL;
    void *best = p->individuals[randi(0, p->size -1)];

    for (i = 0; i < p->size; i++) {
        contender = p->individuals[i];

        if (cmp(contender, best) == 1) {
            best = contender;
        }
    }

    return best;
}
