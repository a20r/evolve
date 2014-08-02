# `mutation.h`

# Structures

    struct mutation_config
    {
        int method;
        float probability;

        int (*mutation_func)(void *, struct config *);
    };


# Functions
- struct mutation_config *mutation_config_new(int method)
- void mutation_config_destroy(void *config)

`mutation_config_new()` creates and returns pointer to a `struct
mutation_config`, while `mutation_config_destroy()` destroys it.

`mutation_config_new()` accepts an integer argument detailing which mutation
method to use.


- int mutate_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
  )
- int mutate_new_node(
    struct node *n,
    struct function_set *fs,
    struct terminal_set *ts
  )

Mutates node `n` mutating from function set `fs` terminal `ts`. Returns `0` on
success, `-1` on failure.


- int point_mutation(void *t, struct config *c)

Performs point mutation on tree `t` using configuration `c`. Returns `0` on
success, `-1` on failure.


- int subtree_mutation(void *t, struct config *c)

Performs subtree mutation on tree `t` using configuration `c`, always returns
`0`.
