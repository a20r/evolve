#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "population.h"


struct population *population_new(int size, size_t individual_size)
{
    int i;
    struct population *p = NULL;

    /* pre-check */
    check(size > 0, "Invalid population size!");
    check(individual_size > 0, "Invalid individual_size!");

    /* create population */
    p = malloc(sizeof(struct population));

    p->generation = 0;
    p->size = size;

    p->individuals = malloc(individual_size * (unsigned long) size);
    p->individual_size = individual_size;

    /* initialize individuals to NULL */
    for (i = 0; i < size; i++) {
        p->individuals[i] = NULL;
    }

    return p;
error:
    return p;
}

void population_destroy(struct population *p, void (*free_func)(void *))
{
    int i;

    /* pre-check */
    check(p, "population is NULL!");
    check(free_func, "free_func is NULL!");

    for (i = 0; i < p->size; i++) {
        free_func(p->individuals[i]);
    }

    free(p->individuals);
    free(p);

error:
    return;
}

void *population_best(
    struct population *p,
    float *(*get_score)(void *t),
    int (*cmp)(const void *, const void *)
)
{
    int i;
    void *contender = NULL;
    void *best = NULL;

    /* pre-check */
    check(p, "population is NULL!");
    check(get_score, "get_score is NULL!");
    check(cmp, "cmp is NULL!");

    /* find best */
    best = p->individuals[randi(0, p->size -1)];
    for (i = 0; i < p->size; i++) {
        contender = p->individuals[i];

        if (get_score(contender) != NULL) {
            if (cmp(contender, best) == -1) {
                best = contender;
            }
        }
    }

    return best;
error:
    return NULL;
}
