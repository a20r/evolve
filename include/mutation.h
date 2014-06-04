#ifndef __MUTATION__
#define __MUTATION__

#include "tree.h"

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
int point_mutation(
    struct tree *t,
    struct function_set *fs,
    struct terminal_set *ts
);

#endif
