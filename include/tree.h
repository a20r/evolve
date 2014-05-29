#ifndef __TREE__
#define __TREE__

/* NODE TYPE */
#define TERM_NODE 0
#define FUNC_NODE 1
#define CFUNC_NODE 2

/* VALUE TYPE */
#define INT 0
#define FLOAT 1
#define STRING 2

/* GENERATE TYPE */
#define FULL 0
#define GROW 1
#define RAMPED_HALF_AND_HALF 2

/* ERRORS */
#define ERROR_GEN_METHOD "Error! Unrecognized tree generation method!\n"


/* STRUCTURES */
struct function
{
    int function;
    int arity;
};

struct function_set
{
    int length;
    struct function **functions;
};

struct terminal
{
    int value_type;
    void *value;
};

struct terminal_set
{
    int length;
    struct terminal **terminals;
};

struct node {
    int type;

    int value_type;
    void *value;

    int function;
    int arity;
    struct node **children;
};

struct tree {
    struct node *root;

    int size;
    int depth;

    float *score;
};

/* FUNCTIONS */
/* function set */
struct function_set *function_set_new(int *functions, int *arities, int num);
int function_set_destroy(struct function_set *fs);
struct function *function_new(int function_index, int arity);
int function_destroy(struct function *f);

/* terminal set */
struct terminal_set *terminal_set_new(int *value_types, void **values, int n);
int terminal_set_destroy(struct terminal_set *fs);
struct terminal *terminal_new(int value_type, void *value);
int terminal_destroy(struct terminal *f);

/* node */
struct node *node_new(int type);
int node_destroy(struct node *n);
int node_clear_destroy(struct node *n);
struct node *node_random_func(struct function_set *fs);
struct node *node_random_term(struct terminal_set *ts);
int node_print(struct node *n);

/* tree */
struct tree *tree_new(struct function_set *fs);
int tree_destroy(void *t);
int tree_traverse(struct node *n, int (*callback)(struct node *));
void tree_build(
    int method,
    struct tree *t,
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
);
struct tree *tree_generate(
    int method,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
);
struct population *tree_population(
    int size,
    int method,
    struct function_set *fs,
    struct terminal_set *ts,
    int max_depth
);
float tree_score(void *t);
int tree_asc_cmp(const void *t1, const void *t2);
int tree_desc_cmp(const void *t1, const void *t2);

#endif
