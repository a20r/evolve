# Population
Population contains functions that handle, modify, perform actions on the GA
population.



## Structures

    struct population {
            /* chromosomes */
            struct darray *chromosomes;
            struct darray *chromosome_scores;
            float total_score;

            /* fitness details */
            int parameters;
            float goal;

            /* evolution details */
            int curr_population;
            int curr_generation;
            int max_population;
            int max_generation;
            char *solution;
    };

The `population` structure is used to encapsulate all individuals in the
evolution process, it also keeps a record of the fitness scores, total fitness
score, current popualtion, generation, maximum population, generation as well
as the solution (if known, as a GA termination criteria).

Parameters:

    struct darray *chromosomes

    struct darray *chromosome_scores

    float total_score

    int parameters

    float goal

    int curr_population

    int curr_generation

    int max_population

    int max_generation



## Functions

    struct population *init_population(
            int param,
            float goal,
            int max_pop,
            int max_gen
    );

Initialize the structure `population`.

Parameters:

    int param
        Number of parameters

    float goal
        Goal score to achieve

    int max_pop
        Maximum popuation

    int max_gen
        Maximum generations

Returns:

    The initialized `population` structure

------------------------------------------------------------------------------

    void destroy_population(struct population **p);

Destory the `popuation` structure.

Parameters:

    structure population **p Population

------------------------------------------------------------------------------

    void gen_init_chromosomes(struct population **p, char *(*mutator)(int));

Generate initial random chromosomes.

------------------------------------------------------------------------------

    int evaluate_chromosomes(float (eval_func)(char *), struct population **p);

------------------------------------------------------------------------------

    void normalize_fitness_values(struct population **p);

------------------------------------------------------------------------------

    void print_chromosome(struct population *p, int index);

------------------------------------------------------------------------------

    void print_chromosomes(struct population *p);

------------------------------------------------------------------------------

    void print_population(struct population *p);
------------------------------------------------------------------------------

    void insertion_sort_population(
            struct population *p,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );
------------------------------------------------------------------------------

    int partition_population(
            struct population *p,
            int pivot_index,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );

------------------------------------------------------------------------------

    void quick_sort_population(
            struct population *m,
            int left,
            int right,
            int(*cmp)(const void *, const void *)
    );

------------------------------------------------------------------------------

    void sort_population(
            struct population *p,
            int (*cmp)(const void *, const void *)
    );

------------------------------------------------------------------------------

    void populate(
            struct population **p,
            float crossover_prob,
            float mutation_prob
    );
------------------------------------------------------------------------------
