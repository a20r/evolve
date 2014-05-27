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
    int function_index;
    int arity;
};

struct function_set
{
    int num_functions;
    struct function **functions;
};

struct terminal
{
    int terminal_index;
};

struct terminal_set
{
    int num_terminals;
    struct terminal **terminals;
};

struct node {
    int type;

    int value_type;
    void *value;

    int function_index;
    int arity;
    void *children;
};

struct tree {
    int size;
    int depth;

    struct node *root;
};

/* FUNCTIONS */
/* function set */
struct function_set *function_set_new(int *functions, int *arities, int num);
void function_set_destroy(struct function_set *fs);
struct function *function_new(int function_index, int arity);
void function_destroy(struct function *f);

/* terminal set */
struct terminal_set *terminal_set_new(int num_terminals);
void terminal_set_destroy(struct terminal_set *fs);
struct terminal *terminal_new(int terminal_index);
void terminal_destroy(struct terminal *f);

/* tree */
struct tree *tree_new();
void tree_destroy(struct tree *t);
void tree_generate(int method, void *func_set, void *term_set, int max_depth);

/* node */
struct node *node_new();
void node_destroy(struct node *n);
struct node *node_random_func(struct function_set *fs);

#endif
