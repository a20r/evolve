#ifndef _GP_TREE_EVALUATOR_H_
#define _GP_TREE_EVALUATOR_H_

#include <dstruct/queue.h>
#include <dstruct/stack.h>


/* FUNCTIONS */
int evaluate_node(struct ast *node, struct stack *s);
struct ast *evaluate_program(struct queue *program);

#endif
