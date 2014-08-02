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

    void *selection;
    void *crossover;
    void *mutation;

    void (*selection_free)(void *);
    void (*crossover_free)(void *);
    void (*mutation_free)(void *);

    void (*print_func)(void *);
    void (*free_func)(void *);
    void *(*copy_func)(void *);
    float *(*get_score)(void *);
    int (*cmp)(const void *, const void *);
};


/* FUNCTIONS */
struct config *config_new(void);
void config_destroy(void *e);

#endif
