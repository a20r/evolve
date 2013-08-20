# Population
Population contains functions that handle, modify, perform actions on the GA population.

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

## Function Summary
    struct population *init_population(
            int param,
            float goal,
            int max_pop,
            int max_gen
    );

    void destroy_population(struct population **p);

    void gen_init_chromosomes(struct population **p, char *(*mutator)(int));

    int evaluate_chromosomes(float (eval_func)(char *), struct population **p);

    void normalize_fitness_values(struct population **p);

    void print_chromosome(struct population *p, int index);

    void print_chromosomes(struct population *p);

    void print_population(struct population *p);

    void insertion_sort_population(
            struct population *p,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );

    int partition_population(
            struct population *p,
            int pivot_index,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );

    void quick_sort_population(
            struct population *m,
            int left,
            int right,
            int(*cmp)(const void *, const void *)
    );

    void sort_population(
            struct population *p,
            int (*cmp)(const void *, const void *)
    );

    void populate(
            struct population **p,
            float crossover_prob,
            float mutation_prob
    );


## init_population
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


## destroy_population

 void destroy_population(struct population **p);

## gen_init_chromosomes

    void gen_init_chromosomes(struct population **p, char *(*mutator)(int));

## evaluate_chromosomes
    int evaluate_chromosomes(float (eval_func)(char *), struct population **p);

## normalize_fitness_values

    void normalize_fitness_values(struct population **p);

## print_chromosome
    void print_chromosome(struct population *p, int index);

## print_chromosomes
    void print_chromosomes(struct population *p);

## print_population
    void print_population(struct population *p);

## insertion_sort_population

    void insertion_sort_population(
            struct population *p,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );

## partition_population

    int partition_population(
            struct population *p,
            int pivot_index,
            int left,
            int right,
            int (*cmp)(const void *, const void *)
    );

## quick_sort_population

    void quick_sort_population(
            struct population *m,
            int left,
            int right,
            int(*cmp)(const void *, const void *)
    );

## sort_population
    void sort_population(
            struct population *p,
            int (*cmp)(const void *, const void *)
    );

## populate

    void populate(
            struct population **p,
            float crossover_prob,
            float mutation_prob
    );
