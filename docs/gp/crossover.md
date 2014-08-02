# `crossover.h`

# Structures

    struct crossover_config
    {
        int method;
        float probability;

        int (*crossover_func)(void *, void *);
    };


# Functions
- struct crossover_config *crossover_config_new(int method)
- void crossover_config_destroy(void *config)

`crossover_config_new()` creates and returns pointer to a `struct
crossover_config`, while `crossover_config_destroy()` destroys it.

`crossover_config_new()` accepts an integer argument detailing which crossover
method to use.


- int *random_indicies_new(struct tree *t1, struct tree *t2)
- void random_indicies_destory(int **indicies)

`random_indicies_new()` and `random_indicies_destroy()` creates an integer
array and destroys them. The random indices created are co-ordinates for
crossover.


- int point_crossover(void *t1, void *t2)

`point_crossover()` performs point crossover with `t1` and `t2`, always
returns 0.
