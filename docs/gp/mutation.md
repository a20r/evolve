# `mutation.h`

# Functions
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
