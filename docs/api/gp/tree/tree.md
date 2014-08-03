# tree.h

**Contents**:
- Structures
- Function Set
- Terminal Set
- Node
- Tree


## Structures

    struct tree_config
    {
        int build_method;
        int max_depth;

        struct function_set *fs;
        struct terminal_set *ts;
    };

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



## Function Set

    struct function_set *function_set_new(struct function **functions, int n);
    void function_set_destroy(void *set);

`function_set_new()` creates and returns pointer to a `struct function_set`,
while `function_set_destroy()` destroys it. `function_set_new()` expects
`functions` to contain an array of `struct function`, with `n` indicating the
number of functions in `functions`.


---

    struct function *function_new(int type, int function, int arity);
    struct function *function_new_func(int function, int arity);
    struct function *function_new_cfunc(int function, int arity);
    void function_destroy(void *f);

Use `function_new()`, `function_new_func()` and `function_cfunc()` to  create a
new `struct function`, where `type` is the function type:

- `DEFAULT`
- `CLASSIFICATION`

and `arity` is the arity of the function (e.g. an `ADD` function should have
arity of 2). The difference between `function_new()` and `function_new_func()`
is that the latter is simply a wrapper around the former, setting the function
type `type` as `DEFAULT`. Similarly `function_new_cfunc()` is sets the function
type `type` to `CLASSIFICATION`. Finally to free or destory the memory
allocated by the previously mentioned functions, just call
`function_destory()`.



## Terminal Set

    struct terminal_set *terminal_set_new(struct terminal **terminals, int n);
    void terminal_set_destroy(void *set);

`terminal_set_new()` creates and returns pointer to a `struct terminal_set`,
while `terminal_set_destroy()` destroys it.


---

    struct terminal *terminal_new(int type, int value_type, void *value);
    struct terminal *terminal_new_input(char *input_name);
    struct terminal *terminal_new_constant(int type, void *value);
    struct terminal *terminal_new_random_constant(
        int type,
        void *min,
        void *max,
        int precision
    );
    void terminal_destroy(void *f);

Use `terminal_new*()` functions to create a `struct terminal`, where `type` is
the type of terminal:

- `INPUT`
- `CONSTANT`
- `RANDOM_CONSTANT`
- `EVAL`

`value_type` is the type of value the terminal holds:

- `INTEGER`
- `FLOAT`
- `DOUBLE`
- `STRING`

`value` is the data you wish for `struct terminal` to hold, `input_name` is the
name of the input the terminal should hold.

`terminal_new_random_constant()` is a bit special in that it is used to create
a random constant of type `type` (limited to `INTEGER`, `FLOAT`, `DOUBLE`),
between `min` and `max`, with a precision of `precision` (i.e. decimal places).
Finally to free or destory the memory allocated by the previously mentioned
functions, just call `terminal_destory()`.


---

    void *terminal_resolve_random(struct terminal *t);

Resolve a random terminal `t` and returns pointer to it.



## Node

    struct node *node_new(int type);
    struct node *node_new_func(int function, int arity);
    struct node *node_new_cfunc(int function, int arity);
    struct node *node_new_input(char *input_name);
    struct node *node_new_constant(int value_type, void *value);
    struct node *node_new_eval(int value_type, void *values, int n_values);
    void node_destroy(void *n);
    void node_clear_destroy(void *n);

Use `node_new*()` functions to create a `struct node`, where `type` is the type of node:

- `TERM_NODE`
- `FUNC_NODE`

, `function` is your identifier for what the function does, `arity` is the number of arity of that function, `input_name` is the name of input, `value_type` is the type of value the node contains:

- `INTEGER`
- `FLOAT`
- `DOUBLE`
- `STRING`

`value` is the actual value the node contains, and `n_values` is the number of
values `values` has. Finally to free or destory the memory allocated by the
previously mentioned functions, just call `node_destory()` or
`node_clear_destroy()`, the difference between the two functions is that the
latter recursively clears a node of type `FUNC_NODE`'s children if they exists,
this is particularly useful if one wishes to completely free a subtree.


---

    void *node_copy(void *src);
    void *node_deepcopy(void *src);

Use `node_copy()` or `node_deepcopy()` to allocate and make a new copy of
`src`. The difference between the two is the former makes a shallow copy,
leaving the children of `src`, where as the latter copies the children of `src`
(i.e. copying the whole subtree from node `src`).


---

    int node_equals(struct node *n1, struct node *n2);
    int node_deep_equals(struct node *n1, struct node *n2);

Use `node_equals()` or `node_deep_equals()` to compare node `n1` and `n2`. The
difference between the two is the former compares both of them aside from their children, where as the latter compares the children as well.


---

    char *node_string(struct node *n);

Creates a string representation of node `n`.


---

    int node_print(struct node *n);

Prints node `n` as a string.


---

    struct node *node_random_func(struct function_set *fs);
    struct node *node_random_func_arity(struct function_set *fs, int arity);
    struct node *node_random_term(struct terminal_set *ts);

`node_random_*()` generates a random node `struct node` from function set `fs`
or terminal set `ts`. With `node_random_func_arity()` you can constrain the
arity of the random function with `arity`.



## Tree

    struct tree_config *tree_config_new(void);
    void tree_config_destroy(void *e);

`tree_config_new()` creates and returns pointer to a `struct tree_config`,
while `tree_config_destroy()` destroys it.


---

    struct tree *tree_new(struct function_set *fs);
    void tree_destroy(void *t);

`tree_new()` creates and returns pointer to a `struct tree`, while
`tree_destroy()` destroys it. The tree generated has default values:

    t->root = node_random_func(fs);
    t->size = 1;
    t->depth = 0;
    t->score = NULL;
    t->hits = 0;
    t->chromosome = NULL;


---

    void *tree_copy(void *t);
    struct node **tree_copy_chromosome(struct tree *t);

Use `tree_copy()` to make a complete copy of tree `t`. To only make a copy of
`tree->chromosome` use `tree_copy_chromosome()`.


---

    int tree_traverse(struct node *n, int (*callback)(struct node *));

`tree_traverse()` is a utility function where you can traverse a tree or a
subtree from node `n`. Each time the function traverses to a node, `callback`
is called. To terminate the tersal, the function expects `callback` to return
`-1` else the return value will be `0`. What ever `callback` returns,
`tree_traverse()` will return the same value.


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

`tree_build()` builds the tree `t` with method `method`:

- `FULL`
- `GROW`
- `RAMPED_HALF_AND_HALF`

starting from node `n`, using nodes from the function set `fs`, terminal set
`ts` and max depth `max_depth`. When using this function `curr_depth` should be
by default 0.

`tree_gerenate()` is essentially a wrapper around `tree_build()` requiring
fewer arguments.


---

    struct population *tree_population(struct config *c);

Generate a population of trees using configurations from `c`.


---

    float *tree_score(void *t);

Utility function to obtain a tree's score (i.e. `t->score`).


---

    int tree_equals(struct tree *t1, struct tree *t2);

Compares two trees `t1` and `t2`. Returns `1` if equals, else `0`.


---

    int tree_size(struct node *n);

Utility function that traverses down the tree from node `n` to obtain the
tree's or subtree's size.


---

    char *tree_string(struct tree *t);

Returns a string representation of tree `t`.


---

    void tree_print(struct tree *t);

Prints a string representation of tree `t`.


---

    void tree_update_traverse(struct tree *t, struct node *n, int depth);
    void tree_update(struct tree *t);

`tree_update_traverse()` traverses the tree `t` from node `n` and depth
`depth`, by default `depth` should be 0. The update updates the tree's depth,
size and chromosome.

`tree_update()` is a wrapper around `tree_update_traverse()` requiring less
arguments.


---

    struct node *tree_replace_node(struct node *old_node, struct node *new_node);

Replaces `old_node` with `new_node` and returns the `old_node`.


---

    int tree_asc_cmp(const void *t1, const void *t2);
    int tree_desc_cmp(const void *t1, const void *t2);
    int tree_cmp(const void *t1, const void *t2);

Compares two trees `t1` and `t2` in ascending order with `tree_asc_cmp()` and
`tree_cmp()` where it returns:

- `0` if `t1` and `t2` are equal or `NULL`.
- `-1` if `t1 > t2` or `t2` `NULL`.
- `1` if `t1 < t2` or `t1` `NULL`.

The reverse is true for `tree_desc_cmp` where:

- `0` if `t1` and `t2` are equal or `NULL`.
- `-1` if `t1 < t2` or `t2` `NULL`.
- `1` if `t1 > t2` or `t1` `NULL`.
