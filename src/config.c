#include <stdlib.h>

#include "config.h"
#include "utils.h"
#include "gp/tree.h"


struct config *config_new(int s_method, int c_method, int m_method)
{
    struct config *c = malloc(sizeof(struct config));

    c->max_generations = -1;
    c->population_size = 0;

    c->stale_limit = -1;
    c->target_score = -1;

    c->tree = NULL;

    c->selection = selection_config_new(s_method);
    c->crossover = crossover_config_new(c_method);
    c->mutation = mutation_config_new(m_method);

    return c;
}

void config_destroy(void *config)
{
    struct config *c = (struct config *) config;

    if (config) {
        free_mem(c->tree, tree_config_destroy);
        selection_config_destroy(c->selection);
        crossover_config_destroy(c->crossover);
        mutation_config_destroy(c->mutation);
        free(config);
    }
}

struct tree_config *tree_config_new(void)
{
    struct tree_config *tc = malloc(sizeof(struct tree_config));

    tc->build_method = -1;
    tc->max_depth = -1;
    tc->fs = NULL;
    tc->ts = NULL;

    return tc;
}

void tree_config_destroy(void *config)
{
    struct tree_config *tc = (struct tree_config *) config;

    if (config) {
        function_set_destroy(tc->fs);
        terminal_set_destroy(tc->ts);
        free(tc);
    }
}

struct selection_config *selection_config_new(int method)
{
    struct selection_config *sc = malloc(sizeof(struct selection_config));

    sc->method = method;
    sc->tournament_size = -1;

    return sc;
}

void selection_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

struct crossover_config *crossover_config_new(int method)
{
    struct crossover_config *cc = malloc(sizeof(struct crossover_config));

    cc->method = method;
    cc->probability = 0.0f;

    return cc;
}

void crossover_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

struct mutation_config *mutation_config_new(int method)
{
    struct mutation_config *mc = malloc(sizeof(struct mutation_config));

    mc->method = method;
    mc->probability = 0.0f;

    return mc;
}

void mutation_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}
