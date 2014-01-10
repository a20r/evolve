#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ga/individual.h"


struct ga_individual *ga_individual_create(char *chromosome)
{
        struct ga_individual *individual;

        individual = malloc(sizeof(struct ga_individual));
        individual->chromosome = malloc(sizeof(char) * strlen(chromosome));
        strcpy(individual->chromosome, chromosome);
        individual->score = 0.0;

        return individual;
}

void ga_individual_destroy(struct ga_individual *individual)
{
        free(individual->chromosome);
        free(individual);
}
