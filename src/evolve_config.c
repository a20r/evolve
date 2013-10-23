#include <stdio.h>
#include <stdlib.h>

#include <jansson.h>
#include <dbg/dbg.h>

#include "evolve_config.h"


static struct evolve_config *config_create()
{
        struct evolve_config *config;

        config = calloc(1, sizeof(struct evolve_config));

        /* selection */
        config->selection = calloc(1, sizeof(struct selection_settings));

        /* crossover */
        config->crossover = calloc(1, sizeof(struct crossover_settings));
        config->crossover->probability = calloc(1, sizeof(float));

        /* mutation */
        config->mutation = calloc(1, sizeof(struct mutation_settings));
        config->mutation->probability = calloc(1, sizeof(float));

        return config;
}

void config_destroy(struct evolve_config *config)
{
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

static int parse_selection_config(struct evolve_config *config, json_t *root)
{
        json_t *value;

        /* select */
        value = json_object_get(root, "select");
        check(json_is_string(value), "Invalid selection select value!");
        config->selection->select= calloc(
                1,
                sizeof(char) * strlen(json_string_value(value)) + 1
        );
        strcpy(config->selection->select, json_string_value(value));

        return 0;
error:
        return -1;
}

static int parse_crossover_config(struct evolve_config *config, json_t *root)
{
        json_t *value;

        /* probability */
        value = json_object_get(root, "probability");
        check(json_is_real(value), "Failed to obtain crossover probability!");
        *(config->crossover->probability) = json_real_value(value);

        /* pivot_index */
        value = json_object_get(root, "pivot_index");
        check(json_is_string(value), "Invalid crossover pivot_index value!");
        config->crossover->pivot_index = calloc(
                1,
                sizeof(char) * strlen(json_string_value(value)) + 1
        );
        strcpy(config->crossover->pivot_index, json_string_value(value));

        return 0;
error:
        return -1;
}

static int parse_mutation_config(struct evolve_config *config, json_t *root)
{
        json_t *value;

        /* probability */
        value = json_object_get(root, "probability");
        check(json_is_real(value), "Failed to obtain mutation probability!");
        *(config->mutation->probability) = json_real_value(value);

        /* mutation_point */
        value = json_object_get(root, "mutation_point");
        check(json_is_string(value), "Invalid mutation mutation_point value!");
        config->mutation->mutation_point = calloc(
                1,
                sizeof(char) * strlen(json_string_value(value)) + 1
        );
        strcpy(config->mutation->mutation_point, json_string_value(value));

        return 0;
error:
        return -1;
}

struct evolve_config *parse_config(char *fp)
{
        json_t *root;
        json_error_t error;
        json_t *selection;
        json_t *crossover;
        json_t *mutation;

        struct evolve_config *config = config_create();

        /* read entire config file */
        root = json_load_file(fp, JSON_DECODE_ANY, &error);
        check(root != NULL, "Failed to read config file!");

        /* parse config file */
        selection = json_object_get(root, "selection");
        crossover = json_object_get(root, "crossover");
        mutation = json_object_get(root, "mutation");

        check(json_is_object(selection), "Missing selection settings!");
        check(json_is_object(crossover), "Missing crossover settings!");
        check(json_is_object(mutation), "Missing mutation settings!");

        check(
                parse_selection_config(config, selection) == 0,
                "Failed to parse selection config!"
        );
        check(
                parse_crossover_config(config, crossover) == 0,
                "Failed to parse crossover config!"
        );
        check(
                parse_mutation_config(config, mutation) == 0,
                "Failed to parse mutation config!"
        );

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
