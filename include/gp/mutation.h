#ifndef _MUTATION_H_
#define _MUTATION_H_

#include "config.h"
#include "gp/tree.h"

#define POINT_MUTATION 0
#define SUBTREE_MUTATION 1


/* STRUCTS */
struct mutation_config
{
    int method;
    float probability;

    int (*mutation_func)(void *, struct config *);
};


/* FUNCTIONS */
struct mutation_config *mutation_config_new(int method);
void mutation_config_destroy(void *config);

int mutate_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
);
int mutate_new_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
);
int point_mutation(void *t, struct config *c);
int subtree_mutation(void *t, struct config *c);

#endif
