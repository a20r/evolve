#ifndef __MUTATION__
#define __MUTATION__

#include "config.h"
#include "gp/tree.h"

#define POINT_MUTATION 0
#define SUBTREE_MUTATION 1

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
