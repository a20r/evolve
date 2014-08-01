#ifndef _CONFIG_H_
#define _CONFIG_H_

#define NONE -1

/* ERRORS */
#define EEVAL "config->evaluate_population is NULL!"
#define ECMP "config->cmp is NULL!"

#define EFREEFUNC "config->free_func is NULL!"
#define ECOPYFUNC "config->copy_fun is NULLc!"
#define EGETSCORE "config->get_score is NULL!"

#define ECFUNC "config->crossover->crossover_func is NULL!"
#define ECPROB "config->crossover->probability is NULL!"

#define EMFUNC "config->mutation->mutation_func is NULL!"
#define EMPROB "config->mutation->probability is NULL!"

#include "data.h"
#include "population.h"


/* STRUCTS */
struct config
{
    int max_generations;
    int population_size;

    struct population *(*population_generator)(
        struct config *
    );
    int (*evaluate_population)(
        struct population *,
        struct data *
    );

    int stale_limit;
    float target_score;

    struct data *data;
    void *data_struct;
    void (*data_struct_free)(void *);

    struct selection_config *selection;
    struct crossover_config *crossover;
    struct mutation_config *mutation;

    void (*print_func)(void *);
    void (*free_func)(void *);
    void *(*copy_func)(void *);
    float *(*get_score)(void *);
    int (*cmp)(const void *, const void *);
};

struct selection_config
{
    int method;
    struct population *(*select_func)(
        struct population *,
        struct config *
    );

    int tournament_size;
};

struct crossover_config
{
    int method;
    float probability;

    int (*crossover_func)(void *, void *);
};

struct mutation_config
{
    int method;
    float probability;

    int (*mutation_func)(void *, struct config *);
};


/* FUNCTIONS */
struct config *config_new(int s_method, int c_method, int m_method);
void config_destroy(void *e);

struct selection_config *selection_config_new(int method);
void selection_config_destroy(void *config);

struct crossover_config *crossover_config_new(int method);
void crossover_config_destroy(void *config);

struct mutation_config *mutation_config_new(int method);
void mutation_config_destroy(void *config);

#endif
