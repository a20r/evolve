# stack.h

**Contents**:
- Structures
- Functions


## Structures

    struct stack_node
    {
        void *value;
        struct stack_node *next;
        struct stack_node *prev;
    };

    struct stack
    {
        int size;
        struct stack_node *root;
        struct stack_node *end;
    };


## Functions

    struct stack *stack_new(void);
    int stack_destroy(struct stack *s, void (*free_func)(void  *));
    void stack_destroy_traverse(struct stack_node *n, void (*free_func)(void  *));

`stack_new()` and `stacks_destroy()` creates and destroys `struct stack`. Where
`s` is the stack to destroy, `free_func` is a function pointer to free a data
element in the stack.


---

    int stack_push(struct stack *s, void *value);
    void *stack_pop(struct stack *s);

`stack_push()` and `stack_pop()` pushes and pops elements to and from the stack
`s`, where `value` is the value to be pushed to the stack. During the push
operation, if the function `stack_push()` fails to push, it will return -1,
else 0.
