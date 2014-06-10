#ifndef __REGRESSION__
#define __REGRESSION__

#include "tree.h"
#include "data.h"
#include "stack.h"

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

#define PI 3.14159265358979323846

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
int regression_traverse(
    int index,
    int end,
    struct node **chromosome,
    struct stack *stack,
    float **func_input,
    struct data *d
);
int regression_evaluate(
    struct tree *t,
    float **func_input,
    struct data *d,
    char *resp
);
int regression_evaluate_population(struct population *p, struct data *d);

#endif
