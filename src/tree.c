#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


/* FUNCTION SET */
struct function_set *function_set_new(int *functions, int *arities, int n)
{
    int i;
    struct function_set *fs;

    fs = malloc(sizeof(struct function_set));
    fs->num_functions = n;
    fs->functions = malloc(sizeof(struct function) * n);

    for (i = 0; i < n; i++) {
        fs->functions[i] = function_new(functions[i], arities[i]);
    }

    return fs;
}

void function_set_destroy(struct function_set *fs)
{
    int i;

    for (i = 0; i < fs->num_functions; i++) {
        function_destroy(fs->functions[i]);
    }

    free(fs->functions);
    free(fs);
}

struct function *function_new(int function, int arity)
{
    struct function *f;

    f = malloc(sizeof(struct function));
    f->function = function;
    f->arity = arity;

    return f;
}

void function_destroy(struct function *f)
{
    free(f);
}



/* TERMINAL SET */
/* struct terminal_set *terminal_set_new(int num_terminals) */
/* { */
/*     struct terminal_set *ts; */
/*  */
/*     ts = malloc(sizeof(struct terminal_set)); */
/*     ts->num_terminals = num_terminals; */
/*     ts->terminals = malloc(sizeof(struct terminal) * num_terminals); */
/*  */
/*     return ts; */
/* } */
/*  */
/* void terminal_set_destroy(struct terminal_set *ts) */
/* { */
/*     int i; */
/*  */
/*     for (i = 0; i < ts->num_terminals; i++) { */
/*         terminal_destroy(ts->terminals[i]); */
/*     } */
/*     free(ts); */
/* } */
/*  */
/* struct terminal *terminal_new(int terminal_index) */
/* { */
/*     struct terminal *t; */
/*  */
/*     t = malloc(sizeof(struct terminal)); */
/*     t->terminal_index = terminal_index; */
/*  */
/*     return t; */
/* } */
/*  */
/* void terminal_destroy(struct terminal *t) */
/* { */
/*     free(t); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* #<{(| TREE |)}># */
/* struct tree *tree_new() */
/* { */
/*     struct tree *t = malloc(sizeof(struct tree *)); */
/*     t->size = 0; */
/*     t->depth = 0; */
/*     return t; */
/* } */
/*  */
/* void tree_destroy(struct tree *t) */
/* { */
/*     free(t); */
/* } */
/*  */
/* void tree_preorder_traverse(struct node *n, int (*callback)(struct node *)) */
/* { */
/*     int i = 0; */
/*  */
/*     switch (n->type) { */
/*     case TERM_NODE: */
/*         callback(n); */
/*         break; */
/*  */
/*     case FUNC_NODE: */
/*         callback(n); */
/*  */
/*         for (i = 0; i < n->arity; i++) { */
/*             tree_preorder_traverse((struct node *) n->children + i, callback); */
/*         } */
/*         break; */
/*  */
/*     case CFUNC_NODE: */
/*         callback(n); */
/*  */
/*         for (i = 0; i < n->arity; i++) { */
/*             tree_preorder_traverse((struct node *) n->children + i, callback); */
/*         } */
/*         break; */
/*     } */
/* } */
/*  */
/* void tree_postorder_traverse(struct node *n, int (*callback)(struct node *)) */
/* { */
/*     int i = 0; */
/*  */
/*     switch (n->type) { */
/*     case TERM_NODE: */
/*         callback(n); */
/*         break; */
/*  */
/*     case FUNC_NODE: */
/*         for (i = 0; i < n->arity; i++) { */
/*             tree_preorder_traverse((struct node *) n->children + i, callback); */
/*         } */
/*         callback(n); */
/*         break; */
/*  */
/*     case CFUNC_NODE: */
/*         for (i = 0; i < n->arity; i++) { */
/*             tree_preorder_traverse((struct node *) n->children + i, callback); */
/*         } */
/*         callback(n); */
/*         break; */
/*     } */
/* } */

/* struct tree *tree_full_method(struct node *n, int max_depth, int *size) */
/* { */
/*     int i; */
/*     struct node *child; */
/*  */
/*     for (i = 0; i < n->arity; i++) { */
/*         if (max_depth ==  0) { */
/*             #<{(| create terminal node |)}># */
/*             child = generate_term_node(); */
/*             n->children[i] = child; */
/*  */
/*         } else { */
/*             #<{(| create function node |)}># */
/*             child = generate_func_node() */
/*             tree_full_method(child, max_depth - 1); */
/*  */
/*             n->children[i] = child; */
/*         } */
/*     } */
/*  */
/*     return n; */
/* } */
/*  */
/* void tree_generate(int method, void *func_set, void *term_set, int max_depth) */
/* { */
/*     struct tree *t = tree_new(); */
/*     struct node *root = node_random_func(func_set); */
/*     t->root = ; */
/*     t->depth = max_depth; */
/*  */
/*     switch (method) { */
/*     case FULL: */
/*         tree_full_method(max_depth); */
/*         break; */
/*     #<{(| case GROW: |)}># */
/*     #<{(|     tree_grow_method(max_depth); |)}># */
/*     #<{(|     break; |)}># */
/*     #<{(| case RAMPED_HALF_AND_HALF: |)}># */
/*     #<{(|     tree_ramped_half_and_half_method(max_depth); |)}># */
/*     #<{(|     break; |)}># */
/*     default: */
/*         printf(ERROR_GEN_METHOD); */
/*     } */
/* } */
/*  */
/*  */
/* #<{(| NODE |)}># */
/* struct node *node_new() */
/* { */
/*     struct node *n = malloc(sizeof(struct node *)); */
/*     return n; */
/* } */
/*  */
/* void node_destroy(struct node *n) */
/* { */
/*     if (n->value_type == STRING) { */
/*         free(n->value); */
/*     } */
/*  */
/*     free(n); */
/* } */
/*  */
/* struct node *node_random_func(struct function_set *fs) */
/* { */
/*     struct node *n = node_new(); */
/*     n->type = FUNC_NODE; */
/*  */
/*     return n; */
/* } */
