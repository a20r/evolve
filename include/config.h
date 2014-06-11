#ifndef __CONFIG__
#define __CONFIG__

#define NONE -1

/* STRUCTS */
struct evolve
{
    int max_generations;
    int stale_limit;
    float target_score;

    struct selection_config *selection;
    struct crossover_config *crossover;
    struct mutation_config *mutation;

    void (*free_func)(void *);
    void *(*copy_func)(void *);
    int (*cmp)(const void *, const void *);

    void *function_set;
    void *terminal_set;
};

struct selection_config
{
    int method;
    struct population *(*select_func)(
        struct population *,
        struct evolve *
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

    int (*mutation_func)(void *, struct evolve *);
};


/* FUNCTIONS */
struct evolve *evolve_new(int s_method, int c_method, int m_method);
void evolve_destroy(void *e);

struct selection_config *selection_config_new(int method);
void selection_config_destroy(void *config);

struct crossover_config *crossover_config_new(int method);
void crossover_config_destroy(void *config);

struct mutation_config *mutation_config_new(int method);
void mutation_config_destroy(void *config);

#endif
