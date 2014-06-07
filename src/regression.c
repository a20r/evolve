#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "tree.h"
#include "utils.h"
#include "regression.h"


float **regression_pop_stack(int index, struct node **stack, struct data *d)
{
    int i;
    struct node *n = stack[index];
    float **values = NULL;

    if (n->terminal_type == INPUT) {
        values = d->data[data_field_index(d, n->value)];
    } else if (n->terminal_type == CONSTANT) {
        values = malloc(sizeof(float *) * (unsigned long) d->rows);
        for (i = 0; i < d->rows; i++) {
            values[i] = n->value;
        }
    }

    node_destroy(n);
    stack[index] = NULL;

    return values;
}

int regression_traverse(int index, int end, struct node **stack, struct data *d)
{
    int i;
    float res;
    float **result = NULL;
    float **val_1 = NULL;
    float **val_2 = NULL;
    struct node *n = stack[index];

    if (n->type == FUNC_NODE) {
        result = malloc(sizeof(float *) * (unsigned long) d->rows);

        if (n->arity == 2) {
            val_1 = regression_pop_stack(index - 1, stack, d);
            val_2 = regression_pop_stack(index - 2, stack, d);
        } else if (n->arity == 1) {
            val_1 = regression_pop_stack(index - 1, stack, d);
        }

        switch (n->function) {
        /* ADD */
        case 0:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] + *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* SUB */
        case 1:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] - *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* MUL */
        case 2:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] * *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* DIV */
        case 3:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] / *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* POW */
        case 4:
            for (i = 0; i < d->rows; i++) {
                res = (float) pow(*(float *) val_1[i], *(float *) val_2[i]);
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* LOG */
        case 5:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) log(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* EXP */
        case 6:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) exp(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* SIN */
        case 7:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) sin(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        /* COS */
        case 8:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) cos(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        default:
            /* UNRECOGNIZED FUNCTION */
            return -1;
        }

        node_destroy(n);
        free_mem(val_1, free);
        free_mem(val_2, free);
    }

    if (index == end) {
        return 0;
    } else {
        return regression_traverse(index + 1, end, stack, d);
    }
}

float *regression_evaluate(struct tree *t, struct data *d)
{
    int res;
    float *score = NULL;
    struct node **stack = tree_copy_chromosome(t);

    res = regression_traverse(0, t->size, stack, d);
    silent_check(res != -1);

    return score;
error:
    return NULL;
}
