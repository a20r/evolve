#ifndef _FUNCTION_SET_H_
#define _FUNCTION_SET_H_

/* FUNCTIONS */
struct ast *execute_unary_function(struct ast *node, struct ast *value);
struct ast *execute_binary_function(
        struct ast *node,
        struct ast *x,
        struct ast *y
);
int node_is_function(struct ast *node);

#endif
