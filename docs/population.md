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
            int population;
            int generation;
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
        Population of chromosomes

    struct darray *chromosome_scores
        Stores the scores of each chromosomes

    float total_score
        Sum total fitness/evaluation score of population

    int parameters
        Length of chromosomes (assuming chromosome is a bit string)

    float goal
        Fitness/ evaluation goal score that causes GA to stop. Optionally
        `goal` can be NULL to denote no goal.

    int population
        Current number of chromosomes in population

    int generation
        Current generation number

    int max_population
        Maximum number of chromosomes in population

    int max_generation
        Maximum number of GA generations



## Functions

    struct population *init_population(
            int param,
            float goal,
            int max_pop,
            int max_gen
    )

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


    void destroy_population(struct population **p)

Destory the `popuation` structure.

Parameters:

    structure population **p Population


------------------------------------------------------------------------------


    void gen_init_chromosomes(struct population **p, char *(*mutator)(int))

Generate initial random chromosomes. How the chromosomes are create is user
defined.

Parameters:

    struct population **p
        Population

    char *(*mutator)(int)
        Mutation function


------------------------------------------------------------------------------


    int evaluate_chromosomes(float (eval_func)(char *), struct population **p)

Evaluates the chromosomes in the population using user defined evaluation
or fitness function.

Parameters:

    float (eval_func)(char *)
        Evaluation/ fitness function

    struct population **p
        Population


Returns:

    0   Goal is not reached
    1   If goal is reached


------------------------------------------------------------------------------


    void normalize_fitness_values(struct population **p)

Normalizes fitness values for all chromosomes in population.

Parameters:

    struct population **p
        Population


------------------------------------------------------------------------------


    void print_chromosome(struct population *p, int index)

Prints the details of a specific chromosome in the population.

Parameters:

    struct population **p
        Population

    int index
        Chromosome in the population to print


------------------------------------------------------------------------------


    void print_chromosomes(struct population *p)

Prints the details of chromosomes in the population.

Parameters:

    struct population **p
        Population


------------------------------------------------------------------------------


    void print_population(struct population *p)

Prints the details of the population.

Parameters:

    struct population **p
        Population


------------------------------------------------------------------------------


    void insertion_sort_population(
            struct population *p,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    )

Sorts the population by their fitness scores using insertion sort.

Parameters:

    struct population *p
        Population

    int left
        Left most or first element index to start sorting from

    int right
        Right most or last element index to start sorting to

    int (*cmp)(const void *, const void *)
        Comparator to be used to sort chromosomes


------------------------------------------------------------------------------


    int partition_population(
            struct population *p,
            int pivot_index,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    )

`partition_population` is part of the quick sort procedure, it sorts the
element under the `pivot_index` in-place to find its ultimate index if all
elements were sorted.

Parameters:

    struct population *p
        Population

    int pivot_index
        Partition pivot index

    int left
        Left most or first element index to start sorting from

    int right
        Right most or last element index to start sorting to

    int (*cmp)(const void *, const void *)
        Comparator to be used to sort chromosomes


Returns:

    Index of the element in `pivot_index`


------------------------------------------------------------------------------


    void quick_sort_population(
            struct population *m,
            int left,
            int right,
            int(*cmp)(const void *, const void *)
    )

Sorts the population by their fitness scores using quick sort.

Parameters:

    struct population *p
        Population

    int left
        Left most or first element index to start sorting from

    int right
        Right most or last element index to start sorting to

    int (*cmp)(const void *, const void *)
        Comparator to be used to sort chromosomes


------------------------------------------------------------------------------


    void sort_population(
            struct population *p,
            int (*cmp)(const void *, const void *)
    )

General population sort wrapper, it uses quick sort for large number of
elements and insertion sort for small number of elements.

Parameters:

    struct population *p
        Population

    int (*cmp)(const void *, const void *)
        Comparator to be used to sort chromosomes


------------------------------------------------------------------------------


    void populate(
            struct population **p,
            int (*crossover_func)(void **, void **, int),
            float *crossover_prob,
            int pivot_index,
            void (*mutation_func)(char **),
            float *mutate_prob
    )

Populates the population to its max after the selection process. It also

------------------------------------------------------------------------------
