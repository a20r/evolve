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


# Functions
- struct config *config_new()
- void config_destroy(void *e)`

`config_new()` creates and returns pointer to a `struct config`, while
`config_destroy()` destroys it.
