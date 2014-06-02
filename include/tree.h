#ifndef __TREE__
#define __TREE__

/* NODE TYPE */
#define TERM_NODE 0
#define FUNC_NODE 1

/* TERMINAL TYPE */
#define INPUT 0
#define CONSTANT 1
#define RANDOM_CONSTANT 2

/* TERMINAL VALUE TYPE */
#define INTEGER 0
#define FLOAT 1
#define DOUBLE 2
#define STRING 3

/* FUNCTION TYPE */
#define DEFAULT 0
#define CLASSIFICATION 1


/* GENERATE TYPE */
#define FULL 0
#define GROW 1
#define RAMPED_HALF_AND_HALF 2

/* ERRORS */
#define ERROR_GEN_METHOD "Error! Unrecognized tree generation method!\n"


/* STRUCTURES */
struct function_set
{
    int length;
    struct function **functions;
};

struct function
{
    int type;
    int function;
    int arity;
};

struct terminal_set
{
    int length;
    struct terminal **terminals;
};

struct terminal
{
    int type;
    int value_type;
    void *value;

    void *min;
    void *max;
    int precision;
};

struct node {
    int type;

    int terminal_type;
    int value_type;
    void *value;

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
};

/* FUNCTIONS */
/* function set */
struct function_set *function_set_new(struct function **functions, int n);
int function_set_destroy(struct function_set *fs);
struct function *function_new(int type, int function, int arity);
struct function *function_new_func(int function, int arity);
struct function *function_new_cfunc(int function, int arity);
int function_destroy(struct function *f);

/* terminal set */
struct terminal_set *terminal_set_new(struct terminal **terminals, int n);
int terminal_set_destroy(struct terminal_set *fs);
struct terminal *terminal_new(int type, int value_type, void *value);
struct terminal *terminal_new_input(char *input_name);
struct terminal *terminal_new_constant(int type, void *value);
struct terminal *terminal_new_random_constant(
    int type,
    void *min,
    void *max,
    int precision
);
int terminal_destroy(struct terminal *f);
void *terminal_resolve_random(struct terminal *t);

/* node */
struct node *node_new(int type);
struct node *node_new_func(int function, int arity);
struct node *node_new_cfunc(int function, int arity);
struct node *node_new_input(char *input_name);
struct node *node_new_constant(int value_type, void *value);
int node_destroy(struct node *n);
int node_clear_destroy(struct node *n);
void *node_copy(void *src);
void *node_deepcopy(void *src);
int node_equals(struct node *n1, struct node *n2);
int node_deep_equals(struct node *n1, struct node *n2);
struct node *node_random_func(struct function_set *fs);
struct node *node_random_term(struct terminal_set *ts);
int node_print(struct node *n);

/* tree */
struct tree *tree_new(struct function_set *fs);
int tree_destroy(void *t);
void *tree_copy(void *t);
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
int tree_equals(struct tree *t1, struct tree *t2);
int tree_asc_cmp(const void *t1, const void *t2);
int tree_desc_cmp(const void *t1, const void *t2);

/* utils */
void *copy_value(int value_type, void *value);

#endif
