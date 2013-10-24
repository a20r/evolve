#ifndef _EVOLVE_CONFIG_H_
#define _EVOLVE_CONFIG_H_


/* STRUCTS */
struct gp_tree_config
{
    int *max_pop;
    int *max_gen;

    int *max_depth;
    int *max_size;

};

struct ga_config
{
    int *max_pop;
    int *max_gen;

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
    char *mode;
    struct selection_config *selection;
    struct crossover_config *crossover;
    struct mutation_config *mutation;
};


/* FUNCTIONS */
struct evolve_config *parse_config(char *fp);
void config_destroy(struct evolve_config *c);


#endif
