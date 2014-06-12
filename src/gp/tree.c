#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "utils.h"
#include "gp/tree.h"
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

void function_set_destroy(void *set)
{
    int i;
    struct function_set *fs = (struct function_set *) set;

    if (set != NULL) {
        for (i = 0; i < fs->length; i++) {
            if (fs->functions[i]) {
                function_destroy(fs->functions[i]);
            }
        }

        free(fs->functions);
        free(fs);
    }
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

void function_destroy(void *f)
{
    free_mem(f, free);
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

void terminal_set_destroy(void *set)
{
    int i;
    struct terminal_set *ts = (struct terminal_set *) set;

    if (set) {
        for (i = 0; i < ts->length; i++) {
            terminal_destroy(ts->terminals[i]);
        }

        free_mem(ts->terminals, free);
        free_mem(ts, free);
    }
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

    t->min = copy_value(type, min);
    t->max = copy_value(type, max);
    t->precision = precision;

    return t;
}

void terminal_destroy(void *t)
{
    struct terminal *term = (struct terminal *) t;

    if (t) {
        if (term->value) {
            free(term->value);
        }

        if (term->type == RANDOM_CONSTANT) {
            free(term->min);
            free(term->max);
        }

        free(term);
    }
}

void *terminal_resolve_random(struct terminal *t)
{
    int i;
    float f;
    double d;
    float precision = (float) pow(10 , t->precision);
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
                f = roundf(f * precision) / precision;
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
                d = round(d * precision) / precision;
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

    /* general */
    n->type = type;
    n->parent = NULL;
    n->nth_child = -1;

    /* terminal node specific */
    n->terminal_type = -1;
    n->value_type = -1;
    n->value = NULL;
    n->n_values = 0;
    n->values = NULL;

    /* function node specific */
    n->function_type = -1;
    n->function = -1;
    n->arity = -1;
    n->children = NULL;

    return n;
}

struct node *node_new_func(int function, int arity)
{
    struct node *n = node_new(FUNC_NODE);

    n->function_type = DEFAULT;
    n->function = function;
    n->arity = arity;
    n->children = NULL;

    return n;
}

struct node *node_new_cfunc(int function, int arity)
{
    struct node *n = node_new(FUNC_NODE);

    n->function_type = CLASSIFICATION;
    n->function = function;
    n->arity = arity;
    n->children = NULL;

    return n;
}

struct node *node_new_input(char *input_name)
{
    struct node *n = node_new(TERM_NODE);

    n->terminal_type = INPUT;
    n->value_type = STRING;
    n->value = input_name;

    return n;
}

struct node *node_new_constant(int value_type, void *value)
{
    struct node *n = node_new(TERM_NODE);

    n->terminal_type = CONSTANT;
    n->value_type = value_type;
    n->value = value;

    return n;
}

struct node *node_new_eval(int value_type, void *values, int n_values)
{
    struct node *n = node_new(TERM_NODE);

    n->terminal_type = EVAL;
    n->value_type = value_type;
    n->n_values = n_values;
    n->values = values;

    return n;
}

void node_destroy(void *n)
{
    struct node *node = (struct node *) n;

    if (n == NULL) {
        return;

    } else if (node->type == TERM_NODE) {
        if (node->terminal_type == RANDOM_CONSTANT) {
            free(node->value);
        } else if (node->terminal_type == EVAL) {
            free(node->values);
        }

    } else if (node->type == FUNC_NODE) {
        free(node->children);

    }

    free(n);
}

void node_clear_destroy(void *n)
{
    int i;
    struct node *node = (struct node *) n;

    if (n == NULL) {
        return;

    } else if (node->type == TERM_NODE) {
        if (node->terminal_type == RANDOM_CONSTANT) {
            free(node->value);
        } else if (node->terminal_type == EVAL) {
            free(node->values);
        }

    } else if (node->type == FUNC_NODE) {
        if (node->children) {
            for (i = 0; i < node->arity; i++) {
                node_clear_destroy(node->children[i]);
            }
            free(node->children);
        }

    }

    free(n);
}

void *node_copy(void *s)
{
    struct node *src = (struct node *) s;
    struct node *cpy = NULL;

    /* pre-check */
    if (s == NULL) {
        return cpy;
    }

    /* create copy */
    cpy = node_new(src->type);

    if (src->type == TERM_NODE) {
        cpy->terminal_type = src->terminal_type;
        cpy->value_type = src->value_type;

        if (src->terminal_type == RANDOM_CONSTANT) {
            cpy->value = copy_value(src->value_type, src->value);
        } else if (src->terminal_type == EVAL) {
            cpy->value = copy_value(src->value_type, src->value);
        } else {
            cpy->value = src->value;
        }

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
    struct node *cpy = NULL;

    /* pre-check */
    if (s == NULL) {
        return cpy;
    }

    /* create copy */
    cpy = node_new(src->type);

    if (src->type == TERM_NODE) {
        cpy->terminal_type = src->terminal_type;
        cpy->value_type = src->value_type;

        if (src->terminal_type == RANDOM_CONSTANT) {
            cpy->value = copy_value(src->value_type, src->value);
        } else if (src->terminal_type == EVAL) {
            cpy->value = copy_value(src->value_type, src->value);
        } else {
            cpy->value = src->value;
        }

    } else if (src->type == FUNC_NODE) {
        cpy->function_type = src->function_type;
        cpy->function = src->function;
        cpy->arity = src->arity;

        children_size = sizeof(struct node *) * (unsigned long) src->arity;
        cpy->children = malloc(children_size);

        for (i = 0; i < src->arity; i++) {
            cpy->children[i] = node_deepcopy(src->children[i]);
        }

    } else {
        return NULL;
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
            silent_check(intcmp(n1->value, n2->value) == 0);
            return 1;
        case FLOAT:
            silent_check(fltcmp(n1->value, n2->value) == 0);
            return 1;
        case DOUBLE:
            silent_check(fltcmp(n1->value, n2->value) == 0);
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

    } else {
        return 0;
    }

error:
    return 0;
}

int node_deep_equals(struct node *n1, struct node *n2)
{
    int i;
    int res;

    if (n1->type == TERM_NODE && n2->type == TERM_NODE) {
        return node_equals(n1, n2);

    } else if (n1->type == FUNC_NODE && n2->type == FUNC_NODE) {
        silent_check(node_equals(n1, n2) == 1);

        for (i = 0; i < n1->arity; i++) {
            res = node_deep_equals(n1->children[i], n2->children[i]);
            silent_check(res == 1);
        }

        return 1;
    }

    return 0;
error:
    return 0;
}

char *node_string(struct node *n)
{
    char buffer[30];
    char *node_str;

    /* pre-check */
    if (n == NULL) {
        return NULL;
    }

    /* build node string */
    if (n->type == TERM_NODE) {
        if (n->value != NULL) {
            if (n->value_type == INTEGER) {
                sprintf(buffer, "T[%d]", *(int *) n->value);
            } else if (n->value_type == FLOAT) {
                sprintf(buffer, "T[%f]", *(float *) n->value);
            } else if (n->value_type == STRING) {
                sprintf(buffer, "T[%s]", (char *) n->value);
            }
        }
    } else if (n->type == FUNC_NODE) {
        sprintf(buffer, "F[%d]", n->function);
    }

    /* copy and return node string */
    node_str = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(node_str, buffer);
    return node_str;
}

int node_print(struct node *n)
{
    /* pre-check */
    if (n == NULL) {
        return -1;
    }

    /* print node */
    if (n->type == TERM_NODE) {
        if (n->value != NULL) {
            if (n->value_type == INTEGER) {
                printf("T[%d] ", *(int *) n->value);
            } else if (n->value_type == FLOAT) {
                printf("T[%f] ", *(float *) n->value);
            } else if (n->value_type == STRING) {
                printf("T[%s] ", (char *) n->value);
            }
        } else {
                printf("T[EVAL] ");
        }
    } else if (n->type == FUNC_NODE) {
        printf("F[%d] ", n->function);
    }

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

struct node *node_random_func_arity(struct function_set *fs, int arity)
{
    int retry = 0;
    int retry_limit = 100;
    struct node *n = NULL;

    while (retry < retry_limit) {
        n = node_random_func(fs);

        if (n->arity == arity) {
            return n;
        } else {
            node_destroy(n);
            retry++;
        }
    }

    node_destroy(n);
    return NULL;
}

struct node *node_random_term(struct terminal_set *ts)
{
    int i = randi(0, ts->length - 1);
    struct terminal *t = ts->terminals[i];
    struct node *n = node_new(TERM_NODE);

    n->terminal_type = t->type;
    if (t->type == RANDOM_CONSTANT) {
        n->value_type = t->value_type;
        n->value = terminal_resolve_random(t);
    } else {
        n->value_type = t->value_type;
        n->value = t->value;
    }

    return n;
}



/* TREE */
struct tree *tree_new(struct function_set *fs)
{
    struct tree *t = malloc(sizeof(struct tree));

    t->root = node_random_func(fs);
    t->size = 1;
    t->depth = 0;
    t->score = NULL;
    t->hits = 0;
    t->chromosome = NULL;

    return t;
}

void tree_destroy(void *t)
{
    /* pre-check */
    if (t == NULL) {
        return;
    }

    struct tree *target = (struct tree *) t;

    node_clear_destroy(target->root);
    free(target->score);
    free(target->chromosome);
    free(t);
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
    int curr_depth,
    int max_depth
)
{
    int i;
    struct node *child;
    float end = ts->length / (float) (ts->length + fs->length);

    if (curr_depth > t->depth) {
        t->depth++;
    }

    for (i = 0; i < n->arity; i++) {
        if (curr_depth == max_depth) {
            /* create terminal node */
            child = node_random_term(ts);
            child->parent = n;
            child->nth_child = i;
            n->children[i] = child;

        } else if (method == GROW && randf(0, 1.0) < end) {
            /* create terminal node */
            child = node_random_term(ts);
            child->parent = n;
            child->nth_child = i;
            n->children[i] = child;

        } else {
            /* create function node */
            child = node_random_func(fs);
            child->parent = n;
            child->nth_child = i;
            n->children[i] = child;

            tree_build(method, t, child, fs, ts, curr_depth + 1, max_depth);
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
        tree_build(method, t, t->root, fs, ts, 0, max_depth);
        break;
    case RAMPED_HALF_AND_HALF:
        if (randf(0.0, 1.0) > 0.5) {
            tree_build(FULL, t, t->root, fs, ts, 0, max_depth);
        } else {
            tree_build(GROW, t, t->root, fs, ts, 0, max_depth);
        }
        break;
    default:
        printf(ERROR_GEN_METHOD);
    }
    tree_update(t);

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
    struct tree *source = (struct tree *) src;
    struct tree *copy = malloc(sizeof(struct tree));

    copy->root = node_deepcopy(source->root);

    if (source->score) {
        copy->score = malloc(sizeof(float));
        *(copy->score) = *(source->score);
    } else {
        copy->score = NULL;
    }
    copy->hits = source->hits;
    copy->chromosome = NULL;
    tree_update(copy);

    return (void *) copy;
}

struct node **tree_copy_chromosome(struct tree *t)
{
    int i;
    struct node **copy = NULL;

    if (t->chromosome) {
        copy = malloc(sizeof(struct node) * (unsigned long) t->size);
        for (i = 0; i < t->size; i++) {
            copy[i] = node_copy(t->chromosome[i]);
        }
    }

    return copy;
}

float tree_score(void *t)
{
    return *(float *) ((struct tree *) t)->score;
}

int tree_equals(struct tree *t1, struct tree *t2)
{
    silent_check(intcmp(&t1->size, &t2->size) == 0);
    silent_check(intcmp(&t1->depth, &t2->depth) == 0);

    if (t1->score == NULL || t2->score == NULL) {
        silent_check(t1->score == t2->score);
    } else {
        silent_check(fltcmp(t1->score, t2->score) == 0);
    }

    return node_deep_equals(t1->root, t2->root);
error:
    return 0;
}

int tree_size(struct node *n)
{
    int i;
    int size = 1;

    if (n->type == TERM_NODE) {
        return 1;
    } else if (n->type == FUNC_NODE) {
        for (i = 0; i < n->arity; i++) {
            size += tree_size(n->children[i]);
        }
    }

    return size;
}

char *tree_string(struct tree *t)
{
    int i;
    char buffer[4096] = "";
    char *node_str = NULL;
    char *tree_str = NULL;

    /* build tree string */
    for (i = 0; i < t->size; i++) {
        node_str = node_string(t->chromosome[i]);
        strcat(buffer, node_str);
        strcat(buffer, " ");
        free(node_str);
    }

    /* copy and return */
    tree_str = malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(tree_str, buffer);
    return tree_str;
}

void tree_print(struct tree *t)
{
    int i;
    for (i = 0; i < t->size; i++) {
        node_print(t->chromosome[i]);
    }
    printf("\n");
}



void tree_update_traverse(struct tree *t, struct node *n, int depth)
{
    int i;

    if (depth > t->depth) {
        t->depth++;
    }

    if (n->type == FUNC_NODE) {
        for (i = 0; i < n->arity; i++) {
            tree_update_traverse(t, n->children[i], depth + 1);
        }
    }

    t->chromosome[t->size] = n;
    t->size++;
}

void tree_update(struct tree *t)
{
    int size = tree_size(t->root);

    if (t->chromosome) {
        free(t->chromosome);
    }
    t->chromosome = malloc(sizeof(struct node *) * (unsigned long) size);

    /* reset size and depth */
    t->depth = 0;
    t->size = 0;

    tree_update_traverse(t, t->root, 0);
}

struct node *tree_replace_node(struct node *old_node, struct node *new_node)
{
    struct node *new_parent = old_node->parent;
    struct node *old_parent = new_node->parent;
    int new_nth_child = old_node->nth_child;
    int old_nth_child = new_node->nth_child;

    if (new_parent) {
        new_parent->children[new_nth_child] = new_node;
    }

    if (old_parent) {
        old_parent->children[old_nth_child] = NULL;
    }

    new_node->parent = new_parent;
    new_node->nth_child = new_nth_child;

    return old_node;
}

int tree_asc_cmp(const void *t1, const void *t2)
{
    float *score_1 = (float *) ((struct tree *) t1)->score;
    float *score_2 = (float *) ((struct tree *) t2)->score;

    /* null-check */
    if (score_1 == NULL || score_2 == NULL) {
        if (score_1 == NULL && score_2 == NULL) {
            return 0;
        } else if (score_1 == NULL) {
            return -1;
        } else if (score_2 == NULL) {
            return 1;
        }
    }

    /* compare floats */
    if (*score_1 > *score_2) {
        return  1;
    } else if (*score_1 < *score_2) {
        return -1;
    } else {
        return 0;
    }
}

int tree_desc_cmp(const void *t1, const void *t2)
{
    return tree_asc_cmp(t1, t2) * -1;
}

int tree_cmp(const void *t1, const void *t2)
{
    return tree_asc_cmp(t1, t2);
}

