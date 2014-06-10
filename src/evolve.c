#include <stdio.h>
#include <stdlib.h>

#include "evolve.h"
#include "population.h"


/* def play_ga_reproduce(play): */
/*     max_pop = play.config["max_population"] */
/*  */
/*         # make a copy of selected individuals and delete them from population */
/*         play.selection.select(play.population) */
/*     parents = list(play.population.individuals) */
/*     del play.population.individuals[:] */
/*  */
/*         # make individuals even numbered */
/*         if len(parents) % 2 == 1: */
/*                 parents.append(random.sample(parents, 1)[0]) */
/*  */
/*     # reproduce individuals */
/*     new_gen = [] */
/*         for i in xrange(0, len(parents) / 2): */
/*                     # get 2 parents */
/*                     child_1 = parents.pop() */
/*         child_2 = parents.pop() */
/*  */
/*         # crossover */
/*         if play.crossover: */
/*                     play.crossover.crossover(child_1, child_2) */
/*  */
/*         # mutation */
/*         if play.mutation: */
/*                     play.mutation.mutate(child_1) */
/*             play.mutation.mutate(child_2) */
/*  */
/*         # append children to new generation */
/*         new_gen.append(child_1) */
/*         new_gen.append(child_2) */
/*  */
/*     # remove the extra at the end */
/*     if len(new_gen) > max_pop: */
/*             new_gen.pop() */
/*  */
/*     # assign new generation to population */
/*     play.population.individuals = new_gen */

/* struct population *reproduce( */
/*     struct population *p,  */

struct evolve_config *evolve_config_new(void)
{
    struct evolve_config *config = malloc(sizeof(struct evolve_config));
    return config;
}

void evolve_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

struct evolve_stats *evolve_stats_new(void)
{
    struct evolve_stats *stats = malloc(sizeof(struct evolve_stats));
    return stats;
}

void evolve_stats_destroy(void *stats)
{
    if (stats) {
        free(stats);
    }
}
