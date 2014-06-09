#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "random.h"
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

int population_destroy(struct population *p, int (*callback)(void *))
{
    int i;
    int res = 0;

    for (i = 0; i < p->size; i++) {
        res = callback(p->individuals[i]);

        if (res == -1) {
            free(p->individuals);
            free(p);
            return -1;
        }
    }

    free(p->individuals);
    free(p);
    return 0;
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
