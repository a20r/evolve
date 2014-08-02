#include <stdlib.h>

#include "config.h"
#include "utils.h"
#include "data.h"
#include "selection.h"


struct config *config_new(void)
{
    struct config *c = malloc(sizeof(struct config));

    c->max_generations = -1;
    c->population_size = -1;

    c->population_generator = NULL;
    c->evaluate_population = NULL;

    c->stale_limit = -1;
    c->target_score = -1;

    c->data = NULL;
    c->data_struct = NULL;
    c->data_struct_free = NULL;

    c->selection = NULL;
    c->crossover = NULL;
    c->mutation = NULL;
    c->selection_free = selection_config_destroy;

    c->selection_free = NULL;
    c->crossover_free = NULL;
    c->mutation_free = NULL;

    c->free_func = NULL;
    c->copy_func = NULL;
    c->get_score = NULL;
    c->cmp = NULL;

    return c;
}

void config_destroy(void *config)
{
    struct config *c = (struct config *) config;

    if (config) {

        data_destroy(c->data);
        if (c->data_struct && c->data_struct_free) {
            c->data_struct_free(c->data_struct);
        } else if (c->data_struct && (c->data_struct_free == NULL)) {
            log_err("ERROR! You forgot to set config->data_struct_free!\n");
        }

        free_mem(c->selection, c->selection_free);
        free_mem(c->crossover, c->crossover_free);
        free_mem(c->mutation, c->mutation_free);

        free(config);
    }
}
