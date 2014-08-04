#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "gp/terminal.h"
#include "gp/tree/tree.h"
#include "gp/tree/mutation.h"


struct mutation_config *mutation_config_new(int method)
{
    struct mutation_config *mc = malloc(sizeof(struct mutation_config));

    mc->method = method;
    mc->probability = -1.0f;

    return mc;
}

void mutation_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

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

int point_mutation(void *t, struct config *c)
{
    struct tree_config *tc = (struct tree_config *) c->data_struct;
    struct function_set *fs = tc->fs;
    struct terminal_set *ts = tc->ts;
    int index = randi(0, ((struct tree *) t)->size - 1);
    struct node *n = ((struct tree *) t)->chromosome[index];
    return mutate_new_node(n, fs, ts);
}

int subtree_mutation(void *t, struct config *c)
{
    struct tree_config *tc = (struct tree_config *) c->data_struct;
    struct function_set *fs = tc->fs;
    struct terminal_set *ts = tc->ts;
    struct tree *target = (struct tree *) t;
    struct tree *subtree  = NULL;
    struct node *selected = NULL;
    int end = (target->size > 2) ? target->size - 2: target->size - 1;

    /* generate subtree */
    subtree = tree_generate(RAMPED_HALF_AND_HALF, fs, ts, 3);

    /* mutate tree */
    selected = target->chromosome[randi(0, end)];

    if (selected != target->root) {
        selected->parent->children[selected->nth_child] = subtree->root;
        subtree->root->parent = selected->parent;
        node_clear_destroy(selected);
    } else {
        node_clear_destroy(target->root);
        target->root = subtree->root;
    }
    subtree->root = NULL;
    tree_destroy(subtree);
    tree_update(t);

    return 0;
}
