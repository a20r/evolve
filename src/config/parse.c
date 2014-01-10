#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include <dbg/dbg.h>
#include <dstruct/darray.h>
#include <dstruct/ast.h>

#include "config/config.h"
#include "config/parse.h"
/* #include "gp/function_set.h" */
/* #include "gp/terminal_set.h" */


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

json_t *get_json(json_t *obj, char *path)
{
        json_t *value;

        /* get json value */
        value = json_object_get(obj, path);

        return value;
}


int set_str(json_t *obj, char *path, char **target)
{
        json_t *value;
        const char *str;

        /* get json value */
        value = json_object_get(obj, path);
        if (value == NULL) {
                *target = NULL;
                log_warn("Warning! %s not set!", path);
                return 0;
        }
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
        if (value == NULL) {
                target = NULL;
                log_warn("Warning! %s not set!", path);
                return 0;
        }
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
        if (value == NULL) {
                target = NULL;
                log_warn("Warning! %s not set!", path);
                return 0;
        }
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

        /* get string array */
        value = json_object_get(obj, path);
        if (value == NULL) {
                target = NULL;
                log_warn("Warning! %s not set!", path);
                return 0;
        }
        silent_check(json_is_array(value) == 1);

        /* loop through array */
        array_size = json_array_size(value);
        check(array_size, "Error! array is empty!");
        for (i = 0; i < array_size; i++) {
                /* get array element */
                data = json_array_get(value, i);
                res = json_is_string(data);
                check(res, "Error! element is not a string!");

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

int set_ast_array(json_t *obj, char *path, struct darray *target, int mode)
{
        json_t *array;
        json_t *element;

        int i = 0;
        int array_size = 0;

        struct ast *node;
        char *tag;
        char *type;
        char *str;
        int integer;
        float real;

        /* get json array */
        array = json_object_get(obj, path);
        if (array == NULL) {
                target = NULL;
                log_warn("Warning! %s not set!", path);
                return 0;
        }
        silent_check(json_is_array(array) == 1);

        /* loop through array */
        array_size = json_array_size(array);
        if (array_size == 0) {
                target = NULL;
                return 0;
        }

        for (i = 0; i < array_size; i++) {
                /* get array element */
                element = json_array_get(array, i);

                /* parse element */
                if (mode == FUNCTION_SET) {
                        tag = get_str(element, "tag");
                        type = get_str(element, "type");

                        if (strcmp(tag, "UNARY_OP") == 0) {
                                node = ast_make_unary_exp(type, NULL);
                        } else if (strcmp(tag, "BINARY_OP") == 0) {
                                node = ast_make_binary_exp(type, NULL, NULL);
                        } else {
                                log_err("Error! Unknown type [%s]\n", tag);
                                goto error;
                        }

                        check(node, "Error! Failed to create function node!");
                        free(tag);
                        free(type);

                } else if (mode == TERMINAL_SET) {
                        tag = get_str(element, "tag");

                        if (strcmp(tag, "INTEGER") == 0) {
                                integer = get_int(element, "value");
                                node = ast_make_exp(INTEGER, &integer);
                        } else if (strcmp(tag, "REAL") == 0) {
                                real = get_real(element, "value");
                                node = ast_make_exp(REAL, &real);
                        } else if (strcmp(tag, "STRING") == 0) {
                                str = get_str(element, "value");
                                node = ast_make_exp(STRING, str);
                        } else {
                                log_err("Error! Unknown type [%s]\n", tag);
                                goto error;
                        }

                        check(node, "Error! Failed to create terminal node!");
                        free(tag);

                } else if (mode == INPUT_SET) {
                        str = get_str(element, "var_name");
                        node = ast_make_exp(STRING, str);
                        check(node, "Error! Failed to create input node!");
                }

                /* set array element */
                silent_check(node);
                darray_set(target, i, node);
        }

        return 0;
error:
        return -1;
}


int parse_ga_config(json_t *obj, struct ga_config *config)
{
        int res = 0;

        /* max_pop */
        res = set_int(obj, "max_pop", config->max_pop);
        check(res == 0, "Failed to parse max_pop!");

        /* max_gen */
        res = set_int(obj, "max_gen", config->max_gen);
        check(res == 0, "Failed to parse max_gen!");

        return 0;
error:
        return -1;
}

int parse_gp_tree_config(json_t *obj, struct gp_tree_config *config)
{
        int res = 0;
        struct darray *function_set = config->function_set;
        struct darray *terminal_set = config->terminal_set;
        struct darray *input_set = config->input_set;
        struct darray *response_set = config->response_set;

        /* general */
        res = set_int(obj, "max_pop", config->max_pop);
        check(res == 0, "Failed to parse max_pop!");
        res = set_int(obj, "max_gen", config->max_gen);
        check(res == 0, "Failed to parse max_gen!");

        /* tree */
        res = set_int(obj, "max_depth", config->max_depth);
        check(res == 0, "Failed to parse max_depth!");
        res = set_int(obj, "max_size", config->max_size);
        check(res == 0, "Failed to parse max_size!");

        /* primitives */
        res = set_ast_array(obj, "function_set", function_set, FUNCTION_SET);
        check(res == 0, "Failed to parse function_set!");
        res = set_ast_array(obj, "terminal_set", terminal_set, TERMINAL_SET);
        check(res == 0, "Failed to parse terminal_set!");
        res = set_ast_array(obj, "input_set", input_set, INPUT_SET);
        check(res == 0, "Failed to parse input_set!");
        res = set_ast_array(obj, "response_set", response_set, RESPONSE_SET);
        check(res == 0, "Failed to parse response_set!");

        /* input */
        res = set_str(obj, "input_file", &config->input_file);
        check(res == 0, "Failed to parse input_file!");
        res = set_str(obj, "input_format", &config->input_format);
        check(res == 0, "Failed to parse input_format!");

        /* response */
        res = set_str(obj, "response_file", &config->response_file);
        check(res == 0, "Failed to parse response_file!");
        res = set_str(obj, "response_format", &config->response_format);
        check(res == 0, "Failed to parse response_format!");

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
                res = parse_ga_config(j_config, config->general.ga);
                check(res == 0, "Failed to parse ga config!");

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
