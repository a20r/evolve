#ifndef __CROSSOVER__
#define __CROSSOVER__

#include "gp/tree.h"

int **random_indicies_new(struct tree *t1, struct tree *t2);
void random_indicies_destory(int **indicies);
int point_crossover(struct tree *t1, struct tree *t2);

#endif
