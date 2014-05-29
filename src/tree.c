#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "tree.h"
#include "random.h"
#include "population.h"


/* FUNCTION SET */
struct function_set *function_set_new(int *functions, int *arities, int n)
{
    int i;
    struct function_set *fs;

    fs = malloc(sizeof(struct function_set));
    fs->length = n;
    fs->functions = malloc(sizeof(struct function *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        fs->functions[i] = function_new(functions[i], arities[i]);
    }

    return fs;
}

int function_set_destroy(struct function_set *fs)
{
    int i;

    for (i = 0; i < fs->length; i++) {
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
    ts->length = n;
    ts->terminals = malloc(sizeof(struct terminal *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        ts->terminals[i] = terminal_new(value_types[i], values[i]);
    }

    return ts;
}

int terminal_set_destroy(struct terminal_set *ts)
{
    int i;

    for (i = 0; i < ts->length; i++) {
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
    if (n == NULL) {
        return -1;
    } if (n->type == TERM_NODE) {
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
    int res = 0;

    if (n == NULL) {
        return -1;

    } else if (n->type == TERM_NODE) {
        if (n->value_type != -1 && n->value_type == STRING) {
            free(n->value);
        }

    } else if (n->type == FUNC_NODE) {
        for (i = 0; i < n->arity; i++) {
            res = node_clear_destroy(n->children[i]);

            if (res == -1) {
                free(n->children);
                free(n);
                return -1;
            }
        }
        free(n->children);

    }

    free(n);
    return 0;
}

struct node *node_random_func(struct function_set *fs)
{
    int i = randi(0, fs->length - 1);
    struct function *f = fs->functions[i];
    struct node *n = node_new(FUNC_NODE);

    n->function = f->function;
    n->arity = f->arity;
    n->children = malloc(sizeof(struct node) * (unsigned long) n->arity);

    /* initialize children to be NULL */
    for (i = 0; i < n->arity; i++) {
        n->children[i] = NULL;
    }

    return n;
}

struct node *node_random_term(struct terminal_set *ts)
{
    int i = randi(0, ts->length - 1);
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
struct tree *tree_new(struct function_set *fs)
{
    struct tree *t = malloc(sizeof(struct tree));

    t->root = node_random_func(fs);
    t->size = 1;
    t->depth = 0;
    t->score = NULL;

    return t;
}

int tree_destroy(void *t)
{
    /* pre-check */
    if (t == NULL) {
        return -1;
    }

    node_clear_destroy(((struct tree *) t)->root);
    free(((struct tree *) t)->score);
    free(t);
    return 0;
}

int tree_traverse(struct node *n, int (*callback)(struct node *))
{
    int i;
    int res;

    /* pre-check */
    if (n == NULL) {
        return -1;
    }

    /* traverse */
    switch (n->type) {
    case TERM_NODE:
        callback(n);
        break;

    case FUNC_NODE:
    case CFUNC_NODE:
        for (i = 0; i < n->arity; i++) {
            res = tree_traverse((struct node *) n->children[i], callback);
            if (res == -1) {
                return -1;
            }
        }
        callback(n);
        break;
    }

    return 0;
}

void tree_build(
    int method,
    struct tree *t,
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
)
{
    int i;
    struct node *child;
    float end = ts->length / (float) (ts->length + fs->length);

    t->depth++;
    for (i = 0; i < n->arity; i++) {
        if (t->depth == max_depth) {
            /* create terminal node */
            /* n->children[i] = node_random_term(ts); */
            child = node_random_term(ts);
            n->children[i] = child;
            /* node_print(child); */

        } else if (method == GROW && randf(0, 1.0) < end) {
            /* create terminal node */
            n->children[i] = node_random_term(ts);

        } else {
            /* create function node */
            child = node_random_func(fs);
            n->children[i] = child;
            /* node_print(child); */

            tree_build(method, t, child, fs, ts, max_depth);
        }

        t->size++;
    }
}

struct tree *tree_generate(
    int method,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
)
{
    struct tree *t = tree_new(fs);

    switch (method) {
    case FULL:
    case GROW:
        tree_build(method, t, t->root, fs, ts, max_depth);
        break;
    case RAMPED_HALF_AND_HALF:
        if (randf(0.0, 1.0) > 0.5) {
            tree_build(FULL, t, t->root, fs, ts, max_depth);
        } else {
            tree_build(GROW, t, t->root, fs, ts, max_depth);
        }
        break;
    default:
        printf(ERROR_GEN_METHOD);
    }

    return t;
}

struct population *tree_population(
    int size,
    int method,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
)
{
    int i;
    struct population *p = population_new(size, sizeof(struct tree));

    for (i = 0; i < size; i++) {
        p->individuals[i] = tree_generate(method, fs, ts, max_depth);
    }

    return p;
}

float tree_score(void *t)
{
    return *(float *) ((struct tree *) t)->score;
}

int tree_asc_cmp(const void *tree1, const void *tree2)
{

    float *t1 = (float *) ((struct tree *) tree1)->score;
    float *t2 = (float *) ((struct tree *) tree2)->score;

    /* pre-check */
    if (t1 == NULL || t2 == NULL) {
        if (t1 == NULL && t2 == NULL) {
            return 0;
        } else if (t1 == NULL) {
            return -1;
        } else if (t2 == NULL) {
            return 1;
        }
    }

    /* compare floats */
    if (*t1 > *t2) {
        return  1;
    } else if (*t1 < *t2) {
        return -1;
    } else {
        return 0;
    }
}

int tree_desc_cmp(const void *tree1, const void *tree2)
{
    float *t1 = (float *) ((struct tree *) tree1)->score;
    float *t2 = (float *) ((struct tree *) tree2)->score;

    /* pre-check */
    if (t1 == NULL || t2 == NULL) {
        if (t1 == NULL && t2 == NULL) {
            return 0;
        } else if (t1 == NULL) {
            return 1;
        } else if (t2 == NULL) {
            return -1;
        }
    }

    /* compare floats */
    if (*t1 < *t2) {
        return  1;
    } else if (*t1 > *t2) {
        return -1;
    } else {
        return 0;
    }
}
