#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "config.h"
#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "gp/crossover.h"
#include "gp/mutation.h"
#include "gp/regression.h"


struct stats *stats_new(void)
{
    struct stats *s = malloc(sizeof(struct stats));

    s->generation = -1;
    s->population_size = 0;

    s->stale_counter = 0;
    s->best = NULL;
    s->best_score = 0.0f;

    s->individuals_evaluated = 0;
    s->diversity = 0.0f;

    return s;
}

void stats_destroy(void *stats, struct config *c)
{
    if (stats) {
        c->free_func(((struct stats *) stats)->best);
        free(stats);
    }
}

void stats_update(struct population *p, struct config *c, struct stats *s)
{
    int cmp_res;
    void *p_best = NULL;

    /* pre-check */
    check(c->free_func, EFREEFUNC);
    check(c->get_score, EGETSCORE);
    check(c->cmp, ECMP);

    /* update */
    p_best = population_best(p, c->get_score, c->cmp);

    if (s->generation == -1) {
        s->best_score = *c->get_score(p_best);
        s->best = c->copy_func(p_best);

    } else {
        cmp_res = c->cmp(p_best, s->best);

        if (cmp_res == -1) {
            c->free_func(s->best);
            s->best = c->copy_func(p_best);
            s->best_score = *c->get_score(p_best);
            s->stale_counter = 0;
        } else {
            s->stale_counter++;
        }

    }

    s->generation++;
    s->population_size = p->size;

error:
    return;
}


int evolve_terminate(struct stats *s, struct config *c)
{
    if (s->generation == c->max_generations) {
        log_info(MAX_GEN_REACHED);
        return 1;
    } else if (s->stale_counter == c->stale_limit) {
        log_info(STALE_LIMIT_REACHED);
        return 1;
    } else if (fltcmp(&s->best_score, &c->target_score) == 0) {
        log_info(TARGET_SCORE_REACHED);
        return 1;
    }

    return 0;
}

struct population *evolve_reproduce(struct population *p, struct config *c)
{
    int i;
    float random;
    struct population *child_1;
    struct population *child_2;
    struct population *new_generation;
    struct crossover_config *crossover = c->crossover;
    struct mutation_config *mutation = c->mutation;

    /* pre-check */
    check(crossover->crossover_func, ECFUNC);
    check(crossover->probability >= 0.0f, ECPROB);
    check(mutation->mutation_func, EMFUNC);
    check(mutation->probability >= 0.0f, EMPROB);

    /* setup new generation and select parents */
    struct selection_config *sc = c->selection;
    new_generation = sc->select_func(p, c);
    population_destroy(p, c->free_func);

    for (i = 0; i < new_generation->size; i += 2) {
        child_1 = new_generation->individuals[i];
        child_2 = new_generation->individuals[i + 1];

        /* genetic operators */
        random = randf(0, 1.0);
        if (crossover->probability > random) {
            crossover->crossover_func(child_1, child_2);
        }

        random = randf(0, 1.0);
        if (mutation->probability > random) {
            mutation->mutation_func(child_1, c);
        }

        random = randf(0, 1.0);
        if (mutation->probability > random) {
            mutation->mutation_func(child_2, c);
        }
    }

    return new_generation;
error:
    return NULL;
}

void evolve_gp(struct config *c, struct data *d)
{
    struct stats *s = stats_new();
    struct population *p = c->population_generator(c);

    /* pre-check */
    check(c->evaluate_population, EEVAL);

    /* evolve */
    c->evaluate_population(p, d);
    stats_update(p, c, s);

    while (evolve_terminate(s, c) != 1) {
        p = evolve_reproduce(p, c);
        c->evaluate_population(p, d);
        stats_update(p, c, s);

        c->print_func(population_best(p, c->get_score, c->cmp));
        log_info(
            "score[%d]: %f",
            s->generation,
            *c->get_score(population_best(p, c->get_score, c->cmp))
        );
        printf("\n");
    }


    /* clean up */
    population_destroy(p, c->free_func);
    stats_destroy(s, c);

error:
    return;
}
