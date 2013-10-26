#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include <dbg/dbg.h>
#include <dstruct/darray.h>

#include "config/config.h"
#include "config/parse.h"


int set_str(json_t *obj, char *path, char **target)
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

int set_int(json_t *obj, char *path, int *target)
{
        json_t *value;

        value = json_object_get(obj, path);
        silent_check(json_is_integer(value));
        *(target) = json_integer_value(value);

        return 0;
error:
        return -1;
}

int set_real(json_t *obj, char *path, float *target)
{
        json_t *value;

        value = json_object_get(obj, path);
        silent_check(json_is_real(value));
        *(target) = json_real_value(value);

        return 0;
error:
        return -1;
}

int set_string_array(json_t *obj, char *path, struct darray *target)
{
        int i = 0;
        int res = 0;
        json_t *value;
        json_t *data;
        const char *tmp;
        char *str;
        int array_size = 0;

        value = json_object_get(obj, path);
        silent_check(json_is_array(value) == 1);

        array_size = json_array_size(value);
        check(array_size, "Error! array is empty!");

        for (i = 0; i < array_size; i++) {
                /* get array element */
                data = json_array_get(value, i);
                res = json_is_string(data);
                check(res, "Error! elememnt is not a string!");

                /* set array element */
                tmp = json_string_value(data);
                str = calloc(strlen(tmp) + 1, sizeof(char));
                strcpy(str, tmp);
                darray_set(target, i, str);
        }

        return 0;
error:
        return -1;
}

char *get_str(json_t *obj, char *path)
{
        json_t *value;
        const char *str;
        char *res;

        /* get json value */
        value = json_object_get(obj, path);
        silent_check(json_is_string(value));

        /* stringify json value and set enum*/
        str = json_string_value(value);
        res = calloc(1, sizeof(char) * strlen(str) + 1);
        strcpy(res, str);

        return res;
error:
        return NULL;
}

int get_int(json_t *obj, char *path)
{
        json_t *value;
        int integer;

        /* get integer value */
        value = json_object_get(obj, path);
        integer = json_integer_value(value);

        return integer;
}

float get_real(json_t *obj, char *path)
{
        json_t *value;
        float real;

        /* get real value */
        value = json_object_get(obj, path);
        real = json_real_value(value);

        return real;
}

int parse_ga_config(json_t *obj, struct ga_config *config)
{
        int res = 0;

        /* max_pop */
        res = set_int(obj, "max_pop", config->max_pop);
        check(res == 0, "Failed to parse max_pop!");

        /* max_gen */
        res = set_int(obj, "max_gen", config->max_pop);
        check(res == 0, "Failed to parse max_gen!");

        return 0;
error:
        return -1;
}

int parse_gp_tree_config(json_t *obj, struct gp_tree_config *config)
{
        int res = 0;

        /* max_pop */
        res = set_int(obj, "max_pop", config->max_pop);
        check(res == 0, "Failed to parse max_pop!");

        /* max_gen */
        res = set_int(obj, "max_gen", config->max_gen);
        check(res == 0, "Failed to parse max_gen!");

        /* max_depth */
        res = set_int(obj, "max_depth", config->max_depth);
        check(res == 0, "Failed to parse max_depth!");

        /* max_size */
        res = set_int(obj, "max_size", config->max_size);
        check(res == 0, "Failed to parse max_size!");

        /* function set */
        res = set_string_array(obj, "function_set", config->function_set);
        check(res == 0, "Failed to parse function_set!");

        /* terminal set */
        res = set_string_array(obj, "terminal_set", config->terminal_set);
        check(res == 0, "Failed to parse terminal_set!");

        return 0;
error:
        return -1;
}

int parse_general_config(struct evolve_config *config, json_t *root)
{
        int res = 0;
        char *mode;
        json_t *j_config;

        /* mode */
        mode = get_str(root, "mode");
        if (strcmp(mode, "GA") == 0) {
                /* get genetic_algorithm json object */
                j_config = json_object_get(root, "ga");
                check(j_config, "Missing genetic_algorithm config!");

                /* init evolve_config */
                config->mode = GA;
                config->general.ga = init_ga_config();

                /* set evolve_onfig */
                res = parse_gp_tree_config(j_config, config->general.gp_tree);
                check(res == 0, "Failed to parse gp config!");

        } else if (strcmp(mode, "GP_TREE") == 0) {
                /* get genetic_programming json object */
                j_config = json_object_get(root, "gp_tree");
                check(j_config, "Missing genetic_programming config!");

                /* init evolve_config */
                config->mode = GP_TREE;
                config->general.gp_tree = init_gp_tree_config();

                /* set evolve_onfig */
                res = parse_gp_tree_config(j_config, config->general.gp_tree);
                check(res == 0, "Failed to parse gp config!");

        } else {
                log_err("Invalid mode type!");
                res = -1;
        }

        release_mem(mode, free);

        return res;
error:
        release_mem(mode, free);

        return res;
}

int parse_selection_config(struct selection_config *config, json_t *root)
{
        int res = 0;

        /* select */
        res = set_str(root, "select", &(config->select));
        check(res == 0, "Failed to obtain number of individuals to select!");

        return 0;
error:
        return -1;
}

int parse_crossover_config(struct crossover_config *config, json_t *root)
{
        int res = 0;

        /* probability */
        res = set_real(root, "probability", config->probability);
        check(res == 0, "Failed to obtain crossover probability!");

        /* pivot_index */
        res = set_str(root, "pivot_index", &(config->pivot_index));
        check(res == 0, "Failed to obtain pivot_index!");

        return 0;
error:
        return -1;
}

int parse_mutation_config(struct mutation_config *config, json_t *root)
{
        int res = 0;

        /* probability */
        res = set_real(root, "probability", config->probability);
        check(res == 0, "Failed to obtain mutation probability!");

        /* mutation_point */
        res = set_str(root, "mutation_point", &(config->mutation_point));
        check(res == 0, "Failed to obtain mutation point!");

        return 0;
error:
        return -1;
}
