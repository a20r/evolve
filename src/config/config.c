#include <stdio.h>
#include <stdlib.h>

#include <jansson.h>
#include <dbg/dbg.h>
#include <dstruct/ast.h>

#include "config/config.h"
#include "config/parse.h"


struct evolve_config *config_create()
{
        struct evolve_config *config;

        config = calloc(1, sizeof(struct evolve_config));

        config->selection = selection_config_create();
        config->crossover = crossover_config_create();
        config->mutation = mutation_config_create();

        return config;
}

void config_destroy(struct evolve_config *config)
{
        /* general */
        if (config->mode == GA) {
                destroy_ga_config(config->general.ga);
        } else if (config->mode == GP_TREE) {
                destroy_gp_tree_config(config->general.gp_tree);
        }

        selection_config_destroy(config->selection);
        crossover_config_destroy(config->crossover);
        mutation_config_destroy(config->mutation);

        free(config);
}

struct ga_config *init_ga_config()
{
        struct ga_config *ga;

        ga = calloc(1, sizeof(struct ga_config));
        ga->max_pop = calloc(1, sizeof(int));
        ga->max_gen = calloc(1, sizeof(int));

        return ga;
}

void destroy_ga_config(struct ga_config *config)
{
        free(config->max_pop);
        free(config->max_gen);
        free(config);
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
        gp_tree->input_set = darray_create(sizeof(struct ast), 100);
        gp_tree->response_set = darray_create(sizeof(struct ast), 100);

        /* input data */
        gp_tree->input_file = NULL;
        gp_tree->input_format = NULL;
        gp_tree->input_cols = 0;
        gp_tree->input_rows = 0;
        gp_tree->input_header = NULL;
        gp_tree->input_data = NULL;

        /* response data */
        gp_tree->response_file = NULL;
        gp_tree->response_format = NULL;
        gp_tree->response_cols = 0;
        gp_tree->response_rows = 0;
        gp_tree->response_header = NULL;
        gp_tree->response_data = NULL;

        return gp_tree;
}

void destroy_gp_tree_config(struct gp_tree_config *config)
{
        struct ast *node;
        int i = 0;

        /* general */
        free(config->max_pop);
        free(config->max_gen);

        /* tree */
        free(config->max_depth);
        free(config->max_size);

        /* primitives */
        for (i = 0; i <= config->function_set->end; i++) {
                node = darray_get(config->function_set, i);
                release_mem(node, ast_destroy);
        }

        for (i = 0; i <= config->terminal_set->end; i++) {
                node = darray_get(config->terminal_set, i);
                release_mem(node, ast_destroy);
        }

        for (i = 0; i <= config->input_set->end; i++) {
                node = darray_get(config->input_set, i);
                release_mem(node, ast_destroy);
        }

        for (i = 0; i <= config->response_set->end; i++) {
                node = darray_get(config->response_set, i);
                release_mem(node, ast_destroy);
        }
        darray_destroy(config->function_set);
        darray_destroy(config->terminal_set);
        darray_destroy(config->input_set);
        darray_destroy(config->response_set);

        /* input data */
        release_mem(config->input_file, free);
        release_mem(config->input_format, free);
        for (i = 0; i < config->input_cols; i++) {
                free(config->input_header[i]);
        }
        release_mem(config->input_header, free);

        for (i = 0; i < config->input_rows; i++) {
                free(config->input_data[i]);
        }
        release_mem(config->input_data, free);

        /* response data */
        release_mem(config->response_file, free);
        release_mem(config->response_format, free);
        for (i = 0; i < config->response_cols; i++) {
                free(config->response_header[i]);
        }
        release_mem(config->response_header, free);

        for (i = 0; i < config->response_rows; i++) {
                free(config->response_data[i]);
        }
        release_mem(config->response_data, free);

        free(config);
}

struct selection_config *selection_config_create()
{
        struct selection_config *selection;

        selection = calloc(1, sizeof(struct selection_config));

        return selection;
}

void selection_config_destroy(struct selection_config *config)
{
        free(config->select);
        free(config);
}

struct crossover_config *crossover_config_create()
{
        struct crossover_config *crossover;

        crossover = calloc(1, sizeof(struct crossover_config));
        crossover->probability = calloc(1, sizeof(float));

        return crossover;
}

void crossover_config_destroy(struct crossover_config *config)
{
        free(config->probability);
        free(config->pivot_index);
        free(config);
}

struct mutation_config *mutation_config_create()
{
        struct mutation_config *mutation;

        mutation = calloc(1, sizeof(struct mutation_config));
        mutation->probability = calloc(1, sizeof(float));

        return mutation;
}

void mutation_config_destroy(struct mutation_config *config)
{
        free(config->probability);
        free(config->mutation_point);
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
