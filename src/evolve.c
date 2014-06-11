#include <stdio.h>
#include <stdlib.h>

#include "evolve.h"
#include "gp/tree.h"
#include "config.h"
#include "population.h"


struct log *log_new(void)
{
    struct log *stats = malloc(sizeof(struct log));
    return stats;
}

void log_destroy(void *stats)
{
    if (stats) {
        free(stats);
    }
}

struct population *evolve_reproduce(struct population *p, struct evolve *e)
{
    int i;
    struct population *parents;
    struct population *child_1;
    struct population *child_2;
    struct population *new_generation;

    /* setup new generation and select parents */
    new_generation = population_new(p->size, p->individual_size);
    parents = e->selection->select_func(p, e);

    for (i = 0; i < parents->size; i += 2) {
        child_1 = e->copy_func(parents->individuals[i]);
        child_2 = e->copy_func(parents->individuals[i + 1]);

        /* genetic operators */
        e->crossover->crossover_func(child_1, child_2);
        e->mutation->mutation_func(child_1, e);
        e->mutation->mutation_func(child_2, e);

        /* append children to new population */
        new_generation->individuals[i] = child_1;
        new_generation->individuals[i + 1] = child_2;
    }
    population_destroy(parents, e->free_func);

    return new_generation;
}
