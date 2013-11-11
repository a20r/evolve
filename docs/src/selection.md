# Selection
Selection is the stage of a genetic algorithm in which individual genomes are
chosen from a population for later breeding (recombination or crossover).


## Function Summary

    void roulette_wheel_selection(struct population **p, int *select)
    void tournament_selection(struct population **p, int *select)


## Functions


    void roulette_wheel_selection(struct population **p, int *select)

**Roulette Wheel Selection** is a generic GA selection procedure, it works as
follows:

1. The fitness function is evaluated for each individual, providing fitness
   values, which are then normalized. Normalization means dividing the fitness
   value of each individual by the sum of all fitness values, so that the sum
   of all resulting fitness values equals 1

2. The population is sorted by descending fitness values

3. Accumulated normalized fitness values are computed (the accumulated fitness
   value of an individual is the sum of its own fitness value plus the fitness
   values of all the previous individuals). The accumulated fitness of the last
   individual should be 1 (otherwise something went wrong in the normalization
   step)

4. A random number `R` between 0 and 1 is chosen

5. The selected individual is the first one whose accumulated normalized value
   is greater than R

6. Steps 4 and 5 are repeated until desired amount of individuals or
   chromosomes are selected from the population

Note: The function modifies the input population **in-place**.

Parameters:

    struct population **p
        Population 

    int *select
        Pointer to number of individual or chromosome to be selected


-------------------------------------------------------------------------------


    void tournament_selection(struct population **p, int *select)

**Tournament Selection** involves running tournaments among a few individuals
chosen at random from the population. The winner of each tournament is selected.

Currently `tournament_selection` implements a tournament size of 2 (aka Binary
tournament selection).

Parameters:

    struct population **p
        Population

    int *select
        Pointer to number of individual or chromosome to be selected
