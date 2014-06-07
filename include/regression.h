#ifndef __REGRESSION__
#define __REGRESSION__

#include "tree.h"
#include "data.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define POW 4

#define LOG 5
#define EXP 6
#define SIN 7
#define COS 8


/* FUNCTIONS */
float **regression_pop_stack(int index, struct node **stack, struct data *d);
int regression_traverse(int index, int end, struct node **stack, struct data *d);
float *regression_evaluate(struct tree *t, struct data *d);

#endif
