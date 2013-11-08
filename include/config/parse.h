#ifndef _PARSE_H_
#define _PARSE_H_

#include <jansson.h>

#include <dstruct/darray.h>

#include "config/config.h"


/* DEFINES */
#define FUNCTION_MODE 1
#define TERMINAL_MODE 2
#define INPUT_MODE 3


/* FUNCTIONS */
char *get_str(json_t *obj, char *path);
int get_int(json_t *obj, char *path);
float get_real(json_t *obj, char *path);
json_t *get_json(json_t *obj, char *path);
int set_str(json_t *obj, char *path, char **target);
int set_int(json_t *obj, char *path, int *target);
int set_real(json_t *obj, char *path, float *target);
int set_string_array(json_t *obj, char *path, struct darray *target);
int set_ast_array(json_t *obj, char *path, struct darray *target, int mode);

int parse_ga_config(json_t *obj, struct ga_config *config);
int parse_gp_tree_config(json_t *obj, struct gp_tree_config *config);
int parse_general_config(struct evolve_config *config, json_t *root);
int parse_selection_config(struct selection_config *config, json_t *root);
int parse_crossover_config(struct crossover_config *config, json_t *root);
int parse_mutation_config(struct mutation_config *config, json_t *root);

#endif
