#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"
#include "random.h"
#include "mutation.h"

int mutate_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
)
{
    struct node *new = NULL;

    if (n->type == TERM_NODE) {
        new = node_random_term(ts);

        /* free random constant in old node */
        if (n->terminal_type == RANDOM_CONSTANT) {
            free(n->value);
        }

        /* copy new->value if type is RANDOM_CONSTANT */
        if (new->terminal_type == RANDOM_CONSTANT) {
            n->value = copy_value(new->value_type, new->value);
        } else {
            n->value = new->value;
        }

        n->terminal_type = new->terminal_type;
        n->value_type = new->value_type;

    } else if (n->type == FUNC_NODE) {
        new = node_random_func_arity(fs, n->arity);

        n->function_type = new->function_type;
        n->function = new->function;
        n->arity = n->arity;

    } else {
        return -1;
    }

    node_destroy(new);
    return 0;
}

int mutate_new_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
)
{
    int retry = 0;
    int retry_limit = 100;
    struct node *copy = node_copy(n);

    while (retry <= retry_limit) {
        mutate_node(n, fs, ts);

        if (node_equals(n, copy) == 0) {
            node_destroy(copy);
            return 0;
        } else {
            retry++;
        }
    }

    node_destroy(copy);
    return -1;
}

int point_mutation(
    struct tree *t,
    struct function_set *fs,
    struct terminal_set *ts
)
{
    int index = randi(0, t->size - 1);
    struct node *n = t->chromosome[index];
    return mutate_new_node(n, fs, ts);
}
