# `config.h`
This module contains code for creating a `struct config` to configure
the Evolutionary Algorithm.

# Structures

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


# Functions
- struct config *config_new(int s_method, int c_method, int m_method)
- void config_destroy(void *e)`

`config_new()` creates and returns pointer to a `struct config`, while
`config_destroy()` destroys it.

`config_new()` accepts the following arguments:

    - int s_method
        Selection method

    - int c_method
        Crossover method

    - int m_method
        Mutation method



- struct selection_config *selection_config_new(int method)
- void selection_config_destroy(void *config)

`selection_config_new()` creates and returns pointer to a `struct
selection_config`, while `selection_config_destroy()` destroys it.

`selection_config_new()` accepts an integer argument detailing which selection
method to use.



- struct crossover_config *crossover_config_new(int method)
- void crossover_config_destroy(void *config)

`crossover_config_new()` creates and returns pointer to a `struct
crossover_config`, while `crossover_config_destroy()` destroys it.

`crossover_config_new()` accepts an integer argument detailing which crossover
method to use.



- struct mutation_config *mutation_config_new(int method)
- void mutation_config_destroy(void *config)

`crossover_config_new()` creates and returns pointer to a `struct
crossover_config`, while `crossover_config_destroy()` destroys it.

`crossover_config_new()` accepts an integer argument detailing which crossover
method to use.
