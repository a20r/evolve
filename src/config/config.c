#include <stdio.h>
#include <stdlib.h>

#include <jansson.h>
#include <dbg/dbg.h>
#include <dstruct/ast.h>

#include "config/config.h"
#include "config/parse.h"


static struct evolve_config *config_create()
{
        struct evolve_config *config;

        config = calloc(1, sizeof(struct evolve_config));

        /* selection */
        config->selection = calloc(1, sizeof(struct selection_config));

        /* crossover */
        config->crossover = calloc(1, sizeof(struct crossover_config));
        config->crossover->probability = calloc(1, sizeof(float));

        /* mutation */
        config->mutation = calloc(1, sizeof(struct mutation_config));
        config->mutation->probability = calloc(1, sizeof(float));

        return config;
}

struct ga_config *init_ga_config()
{
        struct ga_config *ga;

        ga = calloc(1, sizeof(struct ga_config));
        ga->max_pop = calloc(1, sizeof(int));
        ga->max_gen = calloc(1, sizeof(int));

        return ga;
}

struct gp_tree_config *init_gp_tree_config()
{
        struct gp_tree_config *gp_tree;

        gp_tree = calloc(1, sizeof(struct gp_tree_config));

        /* general  */
        gp_tree->max_pop = calloc(1, sizeof(int));
        gp_tree->max_gen = calloc(1, sizeof(int));

        /* tree */
        gp_tree->max_depth = calloc(1, sizeof(int));
        gp_tree->max_size = calloc(1, sizeof(int));

        /* primitives */
        gp_tree->function_set = darray_create(sizeof(struct ast), 100);
        gp_tree->terminal_set  = darray_create(sizeof(struct ast), 100);

        return gp_tree;
}

void destroy_ga_config(struct ga_config *config)
{
        free(config->max_pop);
        free(config->max_gen);
        free(config);
}

void destroy_gp_tree_config(struct gp_tree_config *config)
{
        struct ast *node;
        int i = 0;

        free(config->max_pop);
        free(config->max_gen);
        free(config->max_depth);
        free(config->max_size);

        for (i = 0; i <= config->function_set->end; i++) {
                node = darray_get(config->function_set, i);
                ast_destroy(node);
        }

        /* for (i = 0; i <= config->terminal_set->end; i++) { */
        /*         node = darray_get(config->terminal_set, i); */
        /*         ast_destroy(node); */
        /* } */
        /* darray_destroy(config->function_set); */
        /* darray_destroy(config->terminal_set); */

        free(config);
}

void config_destroy(struct evolve_config *config)
{
        /* general */
        if (config->mode == GA) {
                destroy_ga_config(config->general.ga);
        } else if (config->mode == GP_TREE) {
                destroy_gp_tree_config(config->general.gp_tree);
        }

        /* selection */
        free(config->selection->select);
        free(config->selection);

        /* crossover */
        free(config->crossover->probability);
        free(config->crossover->pivot_index);
        free(config->crossover);

        /* mutation */
        free(config->mutation->probability);
        free(config->mutation->mutation_point);
        free(config->mutation);

        free(config);
}

struct evolve_config *load_config(char *fp)
{
        json_t *root;
        json_error_t error;
        json_t *selection;
        json_t *crossover;
        json_t *mutation;

        struct evolve_config *config;
        int res = 0;

        /* read entire config file */
        config = config_create();
        root = json_load_file(fp, 0, &error);
        check(root != NULL, "Failed to read config file!");

        /* parse config file */
        res = parse_general_config(config, root);
        check(res == 0, "Failed to parse general config!");

        selection = json_object_get(root, "selection");
        crossover = json_object_get(root, "crossover");
        mutation = json_object_get(root, "mutation");

        check(json_is_object(selection), "Missing selection settings!");
        check(json_is_object(crossover), "Missing crossover settings!");
        check(json_is_object(mutation), "Missing mutation settings!");

        /* parse selection config */
        res = parse_selection_config(config->selection, selection);
        check(res == 0, "Failed to parse selection config!");

        /* parse crossover config */
        res = parse_crossover_config(config->crossover, crossover);
        check(res == 0, "Failed to parse crossover config!");

        /* parse mutation config */
        res = parse_mutation_config(config->mutation, mutation);
        check(res == 0, "Failed to parse mutation config!");

        /* clean up */
        json_decref(root);

        return config;
error:
        if (strlen(error.text)) {
                log_err("\nParse Error: %s\n", error.text);
                log_err(" [%d:%d]", error.line, error.column);
                log_err(" %s\n", error.source);
        }
        release_mem(root, json_decref);
        config_destroy(config);
        return NULL;
}
