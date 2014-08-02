#ifndef _CROSSOVER_H_
#define _CROSSOVER_H_

#include "gp/tree/tree.h"

#define POINT_CROSSOVER 0

/* STRUCTS */
struct crossover_config
{
    int method;
    float probability;

    int (*crossover_func)(void *, void *);
};


/* FUNCTIONS */
struct crossover_config *crossover_config_new(int method);
void crossover_config_destroy(void *config);

int *random_indicies_new(struct tree *t1, struct tree *t2);
void random_indicies_destory(int **indicies);
int point_crossover(void *t1, void *t2);

#endif
