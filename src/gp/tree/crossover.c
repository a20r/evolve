#include <stdlib.h>

#include "utils.h"
#include "gp/tree/tree.h"
#include "gp/tree/crossover.h"


struct crossover_config *crossover_config_new(int method)
{
    struct crossover_config *cc = malloc(sizeof(struct crossover_config));

    cc->method = method;
    cc->probability = -1.0f;

    return cc;
}

void crossover_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

int *random_indicies_new(struct tree *t1, struct tree *t2)
{
    int *indicies = malloc(sizeof(int) * 2);

    if (t1->size > 2) {
        indicies[0] = randi(0, t1->size - 2);
    } else {
        indicies[0] = 0;
    }

    if (t2->size > 2) {
        indicies[1] = randi(0, t2->size - 2);
    } else {
        indicies[1] = 0;
    }

    return indicies;
}

int point_crossover(void *t1, void *t2)
{
    int *indicies = random_indicies_new(t1, t2);
    struct node *n1 = ((struct tree *) t1)->chromosome[indicies[0]];
    struct node *n2 = ((struct tree *) t2)->chromosome[indicies[1]];
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
    free(indicies);
    return 0;
}
