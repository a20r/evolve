#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <dstruct/darray.h>


/* STRUCTS */
struct ga_config
{
    int *max_pop;
    int *max_gen;

};

struct gp_tree_config
{
    int *max_pop;
    int *max_gen;
    int *max_depth;
    int *max_size;

    struct darray *function_set;
    struct darray *terminal_set;
    struct darray *input_set;

    char *input_fp;
    char *input_format;
    int cols;
    int rows;
    char **header;
    float **data;
};

union general_config
{
    struct ga_config *ga;
    struct gp_tree_config *gp_tree;
};

enum evolve_mode
{
    GA,
    GP_TREE
};

struct selection_config
{
    char *select;
};

struct crossover_config
{
    float *probability;
    char *pivot_index;
};

struct mutation_config
{
    float *probability;
    char *mutation_point;
};

struct evolve_config
{
    enum evolve_mode mode;
    union general_config general;
    struct selection_config *selection;
    struct crossover_config *crossover;
    struct mutation_config *mutation;
};


/* FUNCTIONS */
struct evolve_config *config_create();
void config_destroy(struct evolve_config *config);

struct ga_config *init_ga_config();
void destroy_ga_config(struct ga_config *config);

struct gp_tree_config *init_gp_tree_config();
void destroy_gp_tree_config(struct gp_tree_config *config);

struct selection_config *selection_config_create();
void selection_config_destroy(struct selection_config *config);

struct crossover_config *crossover_config_create();
void crossover_config_destroy(struct crossover_config *config);

struct mutation_config *mutation_config_create();
void mutation_config_destroy(struct mutation_config *config);

struct evolve_config *load_config(char *fp);
void config_destroy(struct evolve_config *c);

#endif
