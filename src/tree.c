#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "tree.h"
#include "random.h"


/* FUNCTION SET */
struct function_set *function_set_new(int *functions, int *arities, int n)
{
    int i;
    struct function_set *fs;

    fs = malloc(sizeof(struct function_set));
    fs->num_functions = n;
    fs->functions = malloc(sizeof(struct function *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        fs->functions[i] = function_new(functions[i], arities[i]);
    }

    return fs;
}

int function_set_destroy(struct function_set *fs)
{
    int i;

    for (i = 0; i < fs->num_functions; i++) {
        function_destroy(fs->functions[i]);
    }

    free(fs->functions);
    free(fs);
    return 0;
}

struct function *function_new(int function, int arity)
{
    struct function *f;

    f = malloc(sizeof(struct function *));
    f->function = function;
    f->arity = arity;

    return f;
}

int function_destroy(struct function *f)
{
    free(f);
    return 0;
}



/* TERMINAL SET */
struct terminal_set *terminal_set_new(int *value_types, void **values, int n)
{
    int i;
    struct terminal_set *ts;

    ts = malloc(sizeof(struct terminal_set));
    ts->num_terminals = n;
    ts->terminals = malloc(sizeof(struct terminal *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        ts->terminals[i] = terminal_new(value_types[i], values[i]);
    }

    return ts;
}

int terminal_set_destroy(struct terminal_set *ts)
{
    int i;

    for (i = 0; i < ts->num_terminals; i++) {
        terminal_destroy(ts->terminals[i]);
    }

    free(ts->terminals);
    free(ts);
    return 0;
}

struct terminal *terminal_new(int value_type, void *value)
{
    struct terminal *t;

    t = malloc(sizeof(struct terminal));
    t->value_type = value_type;
    if (value_type == STRING) {
        t->value = malloc(sizeof(char) * strlen(value) + 1);
        strcpy(t->value, value);
    } else {
        t->value = value;
    }

    return t;
}

int terminal_destroy(struct terminal *t)
{
    if (t->value_type == STRING) {
        free(t->value);
    }

    free(t);
    return 0;
}

/* NODE */
struct node *node_new(int type)
{
    struct node *n = malloc(sizeof(struct node));
    n->type = type;
    n->value_type = -1;
    return n;
}

int node_destroy(struct node *n)
{
    if (n->type == TERM_NODE) {
        if (n->value_type != -1 && n->value_type == STRING) {
            free(n->value);
        }
    } else if (n->type == FUNC_NODE) {
        free(n->children);
    }

    free(n);
    return 0;
}

int node_clear_destroy(struct node *n)
{
    int i;

    if (n->type == TERM_NODE) {
        silent_check(n->value_type != -1 && n->value_type == STRING);
        free(n->value);

    } else if (n->type == FUNC_NODE) {
        for (i = 0; i < n->arity; i++) {
            node_destroy(n->children[i]);
        }
        free(n->children);
    }

    free(n);
    return 0;
error:
    free(n);
    return 0;
}

struct node *node_random_func(struct function_set *fs)
{
    int i = randi(0, fs->num_functions - 1);
    struct function *f = fs->functions[i];
    struct node *n = node_new(FUNC_NODE);

    n->function = f->function;
    n->arity = f->arity;
    n->children = malloc(sizeof(struct node) * (unsigned long) n->arity);

    return n;
}

struct node *node_random_term(struct terminal_set *ts)
{
    int i = randi(0, ts->num_terminals - 1);
    struct terminal *t = ts->terminals[i];
    struct node *n = node_new(TERM_NODE);

    n->value_type = t->value_type;
    if (n->value_type == STRING) {
        n->value = malloc((sizeof(char) * strlen(t->value)) + 1);
        strcpy(n->value, t->value);
    } else {
        n->value = t->value;
    }

    return n;
}

int node_print(struct node *n)
{
    if (n->type == TERM_NODE) {
        if (n->value_type == INT) {
            printf("T[%d] ", *(int *) n->value);
        } else if (n->value_type == FLOAT) {
            printf("T[%f] ", *(float *) n->value);
        } else if (n->value_type == STRING) {
            printf("T[%s] ", (char *) n->value);
        }
    } else if (n->type == FUNC_NODE) {
        printf("F[%d] ", n->function);
    }

    return 0;
}


/* TREE */
struct tree *tree_new(void)
{
    struct tree *t = malloc(sizeof(struct tree));
    t->size = 0;
    t->depth = 0;
    return t;
}

int tree_destroy(struct tree *t)
{
    free(t);
    return 0;
}

int tree_clear_destroy(struct tree *t)
{
    tree_traverse(t->root, node_destroy);
    free(t);
    return 0;
}

void tree_traverse(struct node *n, int (*callback)(struct node *))
{
    int i = 0;

    switch (n->type) {
    case TERM_NODE:
        silent_check(callback(n));
        break;

    case FUNC_NODE:
    case CFUNC_NODE:
        for (i = 0; i < n->arity; i++) {
            tree_traverse((struct node *) n->children[i], callback);
        }
        silent_check(callback(n));
        break;
    }

error:
    return;
}

struct node *tree_full_method(
    struct tree *t,
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
)
{
    int i;
    struct node *child;

    for (i = 0; i < n->arity; i++) {
        if (max_depth ==  0) {
            /* create terminal node */
            n->children[i] = node_random_term(ts);

        } else {
            /* create function node */
            child = node_random_func(fs);
            n->children[i] = child;
            tree_full_method(t, child, fs, ts, max_depth - 1);
        }

        t->size++;
    }

    return n;
}

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
