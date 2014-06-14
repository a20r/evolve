#ifndef __CROSSOVER__
#define __CROSSOVER__

#include "gp/tree.h"

#define POINT_CROSSOVER 0

int **random_indicies_new(struct tree *t1, struct tree *t2);
void random_indicies_destory(int **indicies);
int point_crossover(void *t1, void *t2);

#endif
