#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "config.h"
#include "evolve.h"
#include "population.h"


struct stats *stats_new(void)
{
    struct stats *l = malloc(sizeof(struct stats));

    l->generation = 0;
    l->stale_counter = 0;
    l->population_size = 0;
    l->best_score = 0.0f;

    l->individuals_evaluated = 0;
    l->diversity = 0.0f;

    return l;
}

void stats_destroy(void *stats)
{
    if (stats) {
        free(stats);
    }
}

void stats_update(struct population *p, struct stats *s, struct config *c)
{
    int cmp_res;
    void *best = population_best(p, c->cmp);
    float best_score = c->get_score(best);

    s->generation++;
    s->population_size = p->size;

    cmp_res = fltcmp(&best_score, &s->best_score);
    if (cmp_res == 0 || cmp_res == -1) {
        s->best_score = best_score;
        s->best = best;
        s->stale_counter = 0;
    } else {
        s->stale_counter++;
    }

}


int evolve_terminate(struct stats *l, struct config *c)
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
