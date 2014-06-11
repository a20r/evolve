#include <stdlib.h>

#include "utils.h"
#include "gp/tree.h"
#include "gp/crossover.h"


int **random_indicies_new(struct tree *t1, struct tree *t2)
{
    int **indicies = malloc(sizeof(int *) * 2);
    indicies[0] = malloc(sizeof(int));
    indicies[1] = malloc(sizeof(int));

    int max_1 = (t1->size > 2) ? t1->size - 2: t1->size - 1;
    int max_2 = (t2->size > 2) ? t2->size - 2: t2->size - 1;

    *indicies[0] = randi(0, max_1);
    *indicies[1] = randi(0, max_2);

    return indicies;
}

void random_indicies_destory(int **indicies)
{
    free(indicies[0]);
    free(indicies[1]);
    free(indicies);
}

int point_crossover(void *t1, void *t2)
{
    int **indicies = random_indicies_new(t1, t2);
    struct node *n1 = ((struct tree *) t1)->chromosome[*indicies[0]];
    struct node *n2 = ((struct tree *) t2)->chromosome[*indicies[1]];
    struct node *n1_old_parent = n1->parent;
    struct node *n2_old_parent = n2->parent;
    int n1_old_nth_child = n1->nth_child;
    int n2_old_nth_child = n2->nth_child;

    /* update parents */
    if (n1->parent != NULL) {
        n1->parent->children[n1->nth_child] = n2;
    }

    if (n2->parent != NULL) {
        n2->parent->children[n2->nth_child] = n1;
    }

    /* crossover */
    n1->parent = n2_old_parent;
    n2->parent = n1_old_parent;
    n1->nth_child = n2_old_nth_child;
    n2->nth_child = n1_old_nth_child;

    /* update tree */
    tree_update(t1);
    tree_update(t2);

    /* clean up */
    random_indicies_destory(indicies);
    return 0;
}
