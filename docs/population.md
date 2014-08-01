# `population.h`

# Structures

    struct population
    {
        int generation;
        int size;

        void **individuals;
        size_t individual_size;
    };


# Functions
- struct population *population_new(int size, size_t individual_size)
- void population_destroy(struct population *p, void (*free_func)(void *))

`population_new()` and `populations_destroy()` creates and destroys `struct
population`. Where `size` is the number of individuals in the population,
`individual_size` is the size of the individual, `p` is the population to be
destroyed and `free_func` is the function pointer to free individuals in the
population.


- void *population_best(
        struct population *p,
        float *(*get_score)(void *t),
        int (*cmp)(const void *, const void *)
  )

`population_best()` finds the best individual, where `p` is the population,
`get_score` is  a function pointer to obtain score from the individual, and
`cmp` is the comparator to determine how each individuals are compared.
