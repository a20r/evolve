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
    int **indicies = random_crossover_index(t1, t2);
    struct node *n1 = t1->chromosome[*(int *) indicies[0]];
    struct node *n2 = t2->chromosome[*(int *) indicies[1]];

    struct node *n1_old_parent = n1->parent;
    struct node *n2_old_parent = n2->parent;

    int n1_old_nth_child = n1->nth_child;
    int n2_old_nth_child = n2->nth_child;

    /* update parents */
    n1->parent->children[n1->nth_child] = n2;
    n2->parent->children[n2->nth_child] = n1;

    /* update node parent and nth_child */
    n1->parent = n2_old_parent;
    n2->parent = n1_old_parent;
    n1->nth_child = n2_old_nth_child;
    n2->nth_child = n1_old_nth_child;

    return 0;
}
