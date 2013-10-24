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
        config->selection = calloc(1, sizeof(struct selection_config));

        /* crossover */
        config->crossover = calloc(1, sizeof(struct crossover_config));
        config->crossover->probability = calloc(1, sizeof(float));

        /* mutation */
        config->mutation = calloc(1, sizeof(struct mutation_config));
        config->mutation->probability = calloc(1, sizeof(float));

        return config;
}

void config_destroy(struct evolve_config *config)
{
        /* general */
        free(config->mode);

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

static int parse_str(json_t *obj, char *path, char **target)
{
        json_t *value;
        const char *str;

        /* get json value */
        value = json_object_get(obj, path);
        silent_check(json_is_string(value));

        /* stringify json value and set target */
        str = json_string_value(value);
        *target = calloc(1, sizeof(char) * strlen(str) + 1);
        strcpy(*target, str);

        return 0;
error:
        return -1;
}

static int parse_real(json_t *obj, char *path, float *target)
{
        json_t *value;

        value = json_object_get(obj, path);
        silent_check(json_is_real(value));
        *(target) = json_real_value(value);

        return 0;
error:
        return -1;
}

static int parse_general_config(struct evolve_config *config, json_t *root)
{
        int res = 0;

        /* mode */
        res = parse_str(root, "mode", &(config->mode));
        check(res == 0, "Failed to parse mode!");

        return 0;
error:
        return -1;
}

static int parse_selection_config(struct selection_config *config, json_t *root)
{
        int res = 0;

        /* select */
        res = parse_str(root, "select", &(config->select));
        check(res == 0, "Failed to obtain number of individuals to select!");

        return 0;
error:
        return -1;
}

static int parse_crossover_config(struct crossover_config *config, json_t *root)
{
        int res = 0;

        /* probability */
        res = parse_real(root, "probability", config->probability);
        check(res == 0, "Failed to obtain crossover probability!");

        /* pivot_index */
        res = parse_str(root, "pivot_index", &(config->pivot_index));
        check(res == 0, "Failed to obtain pivot_index!");

        return 0;
error:
        return -1;
}

static int parse_mutation_config(struct mutation_config *config, json_t *root)
{
        int res = 0;

        /* probability */
        res = parse_real(root, "probability", config->probability);
        check(res == 0, "Failed to obtain mutation probability!");

        /* mutation_point */
        res = parse_str(root, "mutation_point", &(config->mutation_point));
        check(res == 0, "Failed to obtain mutation point!");

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

        struct evolve_config *config;
        int res = 0;

        /* read entire config file */
        config = config_create();
        root = json_load_file(fp, 0, &error);
        check(root != NULL, "Failed to read config file!");

        /* parse config file */
        check(
                parse_general_config(config, root) == 0,
                "Failed to parse general config!"
        );

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
