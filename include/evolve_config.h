#ifndef _EVOLVE_CONFIG_H_
#define _EVOLVE_CONFIG_H_


/* STRUCTS */
struct selection_settings
{
    char *select;
};

struct crossover_settings
{
    float *probability;
    char *pivot_index;
};

struct mutation_settings
{
    float *probability;
    char *mutation_point;
};

struct evolve_config
{
    struct selection_settings *selection;
    struct crossover_settings *crossover;
    struct mutation_settings *mutation;
};


/* FUNCTIONS */
struct evolve_config *parse_config(char *fp);
void config_destroy(struct evolve_config *c);


#endif
