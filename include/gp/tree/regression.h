#ifndef _TREE_REGRESSION_H_
#define _TREE_REGRESSION_H_

#include "data.h"
#include "stack.h"
#include "gp/tree/tree.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define POW 4

#define LOG 5
#define EXP 6
#define RAD 7
#define SIN 8
#define COS 9

#ifndef PI
  #define PI 3.14159265358979323846
#endif

struct regression_stack
{
    struct node **stack;
    int end;
};


/* FUNCTIONS */
int regression_func_input(
    struct node *n,
    struct data *d,
    float **func_input,
    int nth_arity
);
void regression_free_inputs(
    int in1_type,
    struct node *in1,
    int in2_type,
    struct node *in2
);
int regression_check(void);
int regression_traverse(
    int index,
    int end,
    struct node **chromosome,
    struct stack *stack,
    float **func_input,
    struct data *d
);
void regression_clear_stack(struct stack *s);
int regression_evaluate(
    struct tree *t,
    float **func_input,
    struct data *d,
    char *resp
);
int regression_evaluate_population(struct population *p, struct data *d);
void regression_print_traverse(struct node *n);
void regression_print(void *target);

#endif
