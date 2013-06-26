# Selection
Selection is the stage of a genetic algorithm in which individual genomes are
chosen from a population for later breeding (recombination or crossover).

A generic selection procedure may be implemented as follows:

1. The fitness function is evaluated for each individual, providing fitness
values, which are then normalized. Normalization means dividing the fitness
value of each individual by the sum of all fitness values, so that the sum of
all resulting fitness values equals 1.

2. The population is sorted by descending fitness values.

3. Accumulated normalized fitness values are computed (the accumulated fitness
value of an individual is the sum of its own fitness value plus the fitness
values of all the previous individuals). The accumulated fitness of the last
individual should be 1 (otherwise something went wrong in the normalization
step).  4. A random number R between 0 and 1 is chosen.

The selected individual is the first one whose accumulated normalized value is
greater than R.
