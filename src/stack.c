#include <stdlib.h>

#include "stack.h"


struct stack *stack_new(void)
{
    struct stack *s = malloc(sizeof(struct stack));

    s->size = 0;
    s->root = NULL;
    s->end = NULL;

    return s;
}

void stack_destroy_traverse(struct stack_node *n, void (*free_func)(void  *))
{
    if (n->next) {
        stack_destroy_traverse(n->next, free_func);
    }
    free_func(n->value);
    free(n);
}

int stack_destroy(struct stack *s, void (*free_func)(void  *))
{
    if (s->root) {
        stack_destroy_traverse(s->root, free_func);
    }
    free(s);

    return 0;
}

int stack_push(struct stack *s, void *value)
{
    struct stack_node *n = malloc(sizeof(struct stack_node));
    struct stack_node *prev_end = s->end;

    if (n == NULL) {
        return -1;
    }

    n->value = value;
    n->next = NULL;
    n->prev = prev_end;

    if (s->size == 0)  {
        s->root = n;
        s->end = n;
    } else {
        prev_end->next = n;
        s->end = n;
    }
    s->size++;

    return 0;
}

void *stack_pop(struct stack *s)
{
    void *value = s->end->value;
    struct stack_node *previous = s->end->prev;

    free(s->end);
    if (s->size > 1) {
        previous->next = NULL;
        s->end = previous;
    } else {
        s->root = NULL;
        s->end = NULL;
    }
    s->size--;

    return value;
}

