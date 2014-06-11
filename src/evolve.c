#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "config.h"
#include "evolve.h"
#include "population.h"


struct log *log_new(void)
{
    struct log *l = malloc(sizeof(struct log));

    l->generation = 0;
    l->stale_counter = 0;
    l->population_size = 0;
    l->best_score = 0.0f;

    l->individuals_evaluated = 0;
    l->diversity = 0.0f;

    return l;
}

void log_destroy(void *stats)
{
    if (stats) {
        free(stats);
    }
}

int evolve_terminate(struct log *l, struct config *c)
{
    if (l->generation == c->max_generations) {
        return 1;
    } else if (l->stale_counter == c->stale_limit) {
        return 1;
    } else if (fltcmp(&l->best_score, &c->target_score) == 0) {
        return 1;
    }

    return 0;
}

struct population *evolve_reproduce(struct population *p, struct config *c)
{
    int i;
    struct population *parents;
    struct population *child_1;
    struct population *child_2;
    struct population *new_generation;

    /* setup new generation and select parents */
    new_generation = population_new(p->size, p->individual_size);
    parents = c->selection->select_func(p, c);

    for (i = 0; i < parents->size; i += 2) {
        child_1 = c->copy_func(parents->individuals[i]);
        child_2 = c->copy_func(parents->individuals[i + 1]);

        /* genetic operators */
        c->crossover->crossover_func(child_1, child_2);
        c->mutation->mutation_func(child_1, c);
        c->mutation->mutation_func(child_2, c);

        /* append children to new population */
        new_generation->individuals[i] = child_1;
        new_generation->individuals[i + 1] = child_2;
    }
    population_destroy(parents, c->free_func);

    return new_generation;
}

/* struct population *evolve_gp(struct config *c) */
/* { */
/*     struct population *p = NULL; */
/*  */
/*  */
/*  */
/*     return p; */
/* } */
