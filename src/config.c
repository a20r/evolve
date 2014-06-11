#include <stdlib.h>

#include "config.h"


struct evolve *evolve_new(int s_method, int c_method, int m_method)
{
    struct evolve *e = malloc(sizeof(struct evolve));

    e->selection = selection_config_new(s_method);
    e->crossover = crossover_config_new(c_method);
    e->mutation = mutation_config_new(m_method);

    return e;
}

void evolve_destroy(void *e)
{
    struct evolve *c = (struct evolve *) e;

    if (e) {
        selection_config_destroy(c->selection);
        crossover_config_destroy(c->crossover);
        mutation_config_destroy(c->mutation);
        free(e);
    }
}

struct selection_config *selection_config_new(int method)
{
    struct selection_config *sc = malloc(sizeof(struct selection_config));
    sc->method = method;
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
    return mc;
}

void mutation_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}
