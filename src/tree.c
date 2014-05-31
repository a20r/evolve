#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dbg.h"
#include "cmp.h"
#include "tree.h"
#include "random.h"
#include "population.h"


/* FUNCTION SET */
struct function_set *function_set_new(struct function **functions, int n)
{
    int i;
    struct function_set *fs = NULL;

    fs = malloc(sizeof(struct function_set));
    fs->length = n;
    fs->functions = malloc(sizeof(struct function *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        if (functions[i]) {
            fs->functions[i] = functions[i];
        } else {
            fs->functions[i] = NULL;
        }
    }

    return fs;
}

int function_set_destroy(struct function_set *fs)
{
    int i;

    for (i = 0; i < fs->length; i++) {
        if (fs->functions[i]) {
            function_destroy(fs->functions[i]);
        }
    }

    free(fs->functions);
    free(fs);
    return 0;
}

struct function *function_new(int type, int function, int arity)
{
    struct function *f;

    f = malloc(sizeof(struct function));

    f->type = type;
    f->function = function;
    f->arity = arity;

    return f;
}

struct function *function_new_func(int function, int arity)
{
    struct function *f = function_new(DEFAULT, function, arity);
    return f;
}

struct function *function_new_cfunc(int function, int arity)
{
    struct function *f = function_new(CLASSIFICATION, function, arity);
    return f;
}

int function_destroy(struct function *f)
{
    free(f);
    return 0;
}



/* TERMINAL SET */
struct terminal_set *terminal_set_new(struct terminal **terminals, int n)
{
    int i;
    struct terminal_set *ts;

    ts = malloc(sizeof(struct terminal_set));
    ts->length = n;
    ts->terminals = malloc(sizeof(struct terminal *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        ts->terminals[i] = terminals[i];
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

struct terminal *terminal_new(int type, int value_type, void *value)
{
    struct terminal *t = malloc(sizeof(struct terminal));
    t->type = type;
    t->value_type = value_type;
    t->value = NULL;

    t->min = NULL;
    t->max = NULL;
    t->precision = -1;

    if (value == NULL) {
        return t;
    }

    switch (value_type) {
    case INTEGER:
        t->value = malloc(sizeof(int));
        *(int *) t->value = *(int *) value;
        break;
    case FLOAT:
        t->value = malloc(sizeof(float));
        *(float *) t->value = *(float *) value;
        break;
    case DOUBLE:
        t->value = malloc(sizeof(double));
        *(double *) t->value = *(double *) value;
        break;
    case STRING:
        t->value = malloc(sizeof(char) * strlen(value) + 1);
        strcpy(t->value, value);
        break;
    }

    return t;
}

struct terminal *terminal_new_input(char *input_name)
{
    struct terminal *t = terminal_new(INPUT, STRING, input_name);
    return t;
}

struct terminal *terminal_new_constant(int type, void *value)
{
    struct terminal *t = terminal_new(CONSTANT, type, value);
    return t;
}

struct terminal *terminal_new_random_constant(
    int type,
    void *min,
    void *max,
    int precision
)
{
    struct terminal *t = terminal_new(RANDOM_CONSTANT, type, NULL);

    switch (type) {
    case INTEGER:
        t->min = malloc(sizeof(int));
        t->max = malloc(sizeof(int));
        *(int *) t->min = *(int *) min;
        *(int *) t->max = *(int *) max;
        break;
    case FLOAT:
        t->min = malloc(sizeof(float));
        t->max = malloc(sizeof(float));
        *(float *) t->min = *(float *) min;
        *(float *) t->max = *(float *) max;
        break;
    case DOUBLE:
        t->min = malloc(sizeof(double));
        t->max = malloc(sizeof(double));
        *(double *) t->min = *(double *) min;
        *(double *) t->max = *(double *) max;
        break;
    }
    t->precision = precision;

    return t;
}

int terminal_destroy(struct terminal *t)
{
    if (t->value != NULL) {
        free(t->value);
    }

    if (t->type == RANDOM_CONSTANT) {
        free(t->min);
        free(t->max);
    }

    free(t);
    return 0;
}

void *terminal_resolve_random(struct terminal *t)
{
    int i;
    float f;
    double d;
    void *retval;

    switch (t->value_type) {
    case INTEGER:
        i = randi(*(int *) t->min, *(int *) t->max);
        retval = malloc(sizeof(int));
        *(int *) retval = i;
        break;
    case FLOAT:
        f = randf(*(float *) t->min, *(float *) t->max);
        if (t->precision != -1) {
            if (t->precision == 0) {
                f = (int) f;
            } else {
                f = (float) (f * pow(10, t->precision));
                f = (int) f;
                f = (float) (f / pow(10, t->precision));
            }
        }

        retval = malloc(sizeof(f));
        *(float *) retval = f;
        break;
    case DOUBLE:
        d = randf((float) *(double *) t->min, (float) *(double *) t->max);
        if (t->precision != -1) {
            if (t->precision == 0) {
                d = (int) d;
            } else {
                d = d * pow(10, t->precision);
                d = (int) d;
                d = d / pow(10, t->precision);
            }
        }

        retval = malloc(sizeof(double));
        *(double *) retval = d;
        break;
    default:
        retval = NULL;
        break;
    }

    return retval;
}


/* NODE */
struct node *node_new(int type)
{
    struct node *n = malloc(sizeof(struct node));

    n->type = type;

    /* terminal node specific */
    n->terminal_type = -1;
    n->value_type = -1;

    /* function node specific */
    n->function_type = -1;
    n->function = -1;
    n->arity = -1;
    n->children = NULL;

    return n;
}

int node_destroy(struct node *n)
{
    if (n == NULL) {
        return -1;
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
        /* DO NOTHING! because n->value references terminals */

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

void *node_copy(void *s)
{
    struct node *src = (struct node *) s;
    struct node *cpy = NULL;

    /* pre-check */
    if (s == NULL) {
        return cpy;
    } else {
        cpy = malloc(sizeof(struct node));
        cpy->type = src->type;
        cpy->children = NULL;
    }

    /* create copy */
    if (src->type == TERM_NODE) {
        cpy->terminal_type = src->terminal_type;
        cpy->value_type = src->value_type;
        cpy->value = src->value;

    } else if (src->type == FUNC_NODE) {
        cpy->function_type = src->function_type;
        cpy->function = src->function;
        cpy->arity = src->arity;

        /* this function does not copy the children, if you want that
         * use node_deepcopy() instead */

    } else {
        return NULL;
    }

    return (void *) cpy;
}

void *node_deepcopy(void *s)
{
    int i;
    size_t children_size;
    struct node *src = (struct node *) s;
    struct node *cpy = node_copy(s);
    struct node *child = NULL;

    if (src->type == FUNC_NODE) {
        children_size = sizeof(struct node) * (unsigned long) src->arity;
        cpy->children = malloc(children_size);

        for (i = 0; i < src->arity; i++) {
            child = node_deepcopy(src->children[i]);

            if (child == NULL) {
                return NULL;
            }

            cpy->children[i] = child;
        }
    }

    return (void *) cpy;
}

int node_equals(struct node *n1, struct node *n2)
{
    if (n1->type == TERM_NODE && n2->type == TERM_NODE) {
        /* terminal specific */
        silent_check(n1->terminal_type == n2->terminal_type);
        silent_check(n1->value_type == n2->value_type);

        switch (n1->value_type) {
        case INTEGER:
            silent_check(int_cmp(n1->value, n2->value) == 0);
            return 1;
        case FLOAT:
            silent_check(float_cmp(n1->value, n2->value) == 0);
            return 1;
        case DOUBLE:
            silent_check(float_cmp(n1->value, n2->value) == 0);
            return 1;
        case STRING:
            silent_check(strcmp(n1->value, n2->value) == 0);
            return 1;
        default:
            return 0;
        }

    } else if (n1->type == FUNC_NODE && n2->type == FUNC_NODE) {
        /* function specific */
        silent_check(n1->function_type == n2->function_type);
        silent_check(n1->function == n2->function);
        silent_check(n1->arity == n2->arity);
        return 1;
    }

    return 0;
error:
    return 0;
}

struct node *node_random_func(struct function_set *fs)
{
    int i = randi(0, fs->length - 1);
    struct function *f = fs->functions[i];
    struct node *n = node_new(FUNC_NODE);

    n->function_type = f->type;
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

    n->terminal_type = t->type;
    n->value_type = t->value_type;
    n->value = t->value;

    return n;
}

int node_print(struct node *n)
{
    if (n == NULL) {
        return -1;
    } else if (n->type == TERM_NODE) {
        if (n->value != NULL) {
            if (n->value_type == INTEGER) {
                printf("T[%d] ", *(int *) n->value);
            } else if (n->value_type == FLOAT) {
                printf("T[%f] ", *(float *) n->value);
            } else if (n->value_type == STRING) {
                printf("T[%s] ", (char *) n->value);
            }
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

void *tree_copy(void *src)
{
    struct tree *copy = malloc(sizeof(struct tree));

    copy->root = ((struct tree *) src)->root;
    copy->size = ((struct tree *) src)->size;
    copy->depth = ((struct tree *) src)->depth;

    if (((struct tree *) src)->score) {
        copy->score = malloc(sizeof(float));
        *(copy->score) = *(((struct tree *) src)->score);
    }

    return (void *) copy;
}

float tree_score(void *t)
{
    return *(float *) ((struct tree *) t)->score;
}

/* int tree_equals(struct tree *t1, struct tree *t2) */
/* { */
/*  */
/*     return 0; */
/* } */

int tree_asc_cmp(const void *tree1, const void *tree2)
{
    float *t1 = (float *) ((struct tree *) tree1)->score;
    float *t2 = (float *) ((struct tree *) tree2)->score;

    /* null-check */
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

    /* null-check */
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
