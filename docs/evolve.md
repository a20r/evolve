# `evolve.h`


# Structures

    struct stats
    {
        int generation;
        int population_size;

        int stale_counter;
        void *best;
        float best_score;

        int individuals_evaluated;
        float diversity;
    };



# Functions
- struct stats *stats_new(void)
- void stats_destroy(void *stats, struct config *c)

`stats_new()` and `stats_destroy()` creates and destroys `struct stats`. Where
`stats` is the generation statistics and `c` is the configuration repsectively.


- void stats_update(struct population *p, struct config *c, struct stats *s)

`stats_update()` update dates `struct stats s` with new information about the
current generation, where `p` is the population, `c` is the configuration used.


- struct population *evolve_reproduce(struct population *p, struct config *c)

`evolve_reproduce()` reproduces the population `p` using configurations found
in configuraiton `c`. The function then returns the newly reproduced
population.


- int evolve_terminate(struct stats *s, struct config *c)

`evolve_terminate()` determines whether the Evolutionary Algorithm loop should
be terminated if certain criterias are met, such as:

    - Max generation
    - Stale limit
    - Target score reached

If any of the above is met the function will return 1 else 0.


- void evolve_gp(struct config *c, struct data *d)

`evolve_gp()` contains a general Genetic Programming loop, where the population
initialization, evaluation, genetic operators and reproduction occurs. Every
step of the Genetic Programming loop can be configured via configuration `c`.
