#ifndef _TREE_H_
#define _TREE_H_

#include "config.h"


/* NODE TYPE */
#define TERM_NODE 0
#define FUNC_NODE 1

/* GENERATE TYPE */
#define FULL 0
#define GROW 1
#define RAMPED_HALF_AND_HALF 2

/* ERRORS */
#define E_FS "Function set is not set! Check config->data_struct->fs!"
#define E_TS "Terminal set is not set! Check config->data_struct->ts!"
#define E_GEN_METHOD "Unrecognized tree generation method!"


/* STRUCTURES */
struct tree_config
{
    int build_method;
    int max_depth;

    struct function_set *fs;
    struct terminal_set *ts;
};

struct node {
    int type;

    struct node *parent;
    int nth_child;

    int terminal_type;
    int value_type;
    void *value;
    int n_values;
    void *values;

    int function_type;
    int function;
    int arity;
    struct node **children;
};

struct tree {
    struct node *root;

    int size;
    int depth;

    float *score;
    int hits;
    struct node **chromosome;
};

/* FUNCTIONS */
/* node */
struct node *node_new(int type);
struct node *node_new_func(int function, int arity);
struct node *node_new_cfunc(int function, int arity);
struct node *node_new_input(char *input_name);
struct node *node_new_constant(int value_type, void *value);
struct node *node_new_eval(int value_type, void *values, int n_values);
void node_destroy(void *n);
void node_clear_destroy(void *n);
void *node_copy(void *src);
void *node_deepcopy(void *src);
int node_equals(struct node *n1, struct node *n2);
int node_deep_equals(struct node *n1, struct node *n2);
char *node_string(struct node *n);
int node_print(struct node *n);
struct node *node_random_func(struct function_set *fs);
struct node *node_random_func_arity(struct function_set *fs, int arity);
struct node *node_random_term(struct terminal_set *ts);

/* tree */
struct tree_config *tree_config_new(void);
void tree_config_destroy(void *e);
struct tree *tree_new(struct function_set *fs);
void tree_destroy(void *t);
void *tree_copy(void *t);
struct node **tree_copy_chromosome(struct tree *t);
int tree_traverse(struct node *n, int (*callback)(struct node *));
void tree_build(
    int method,
    struct tree *t,
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts,
    int curr_depth,
    int max_depth
);
struct tree *tree_generate(
    int method,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
);
struct population *tree_population(struct config *c);
float *tree_score(void *t);
int tree_equals(struct tree *t1, struct tree *t2);
int tree_size(struct node *n);
char *tree_string(struct tree *t);
void tree_print(struct tree *t);
void tree_update_traverse(struct tree *t, struct node *n, int depth);
void tree_update(struct tree *t);
struct node *tree_replace_node(struct node *old_node, struct node *new_node);
int tree_asc_cmp(const void *t1, const void *t2);
int tree_desc_cmp(const void *t1, const void *t2);
int tree_cmp(const void *t1, const void *t2);

#endif
