#include <stdlib.h>

#include "config.h"


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

