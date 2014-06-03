#include "tree.h"
#include "random.c"
#include "crossover.h"


int **random_crossover_index(struct tree *t1, struct tree *t2)
{
    int **indicies = malloc(sizeof(int) * 2);

    *(int *) indicies[0] = randi(0, t1->size - 1);
    *(int *) indicies[1] = randi(0, t2->size - 1);

    return indicies;
}

int point_crossover(struct tree *t1, struct tree *t2)
{
    /* int **indicies = random_crossover_index(t1, t2); */
    /* struct node *n1 =  */



    return 0;
}
