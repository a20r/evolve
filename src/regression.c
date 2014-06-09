#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "cmp.h"
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

    } else if (n->terminal_type == EVAL) {
        values = malloc(sizeof(float *) * (unsigned long) d->rows);
        for (i = 0; i < d->rows; i++) {
            values[i] = n->values[i];
        }
    }

    node_destroy(n);
    stack[index] = NULL;

    return values;
}

int regression_traverse(int index, int end, struct node **stack, struct data *d)
{
    int i;
    int val_1_type = -1;
    int val_2_type = -1;
    float res;
    float **result = NULL;
    float **val_1 = NULL;
    float **val_2 = NULL;
    struct node *n = stack[index];

    if (n->type == FUNC_NODE) {
        /* result array */
        result = malloc(sizeof(float *) * (unsigned long) d->rows);

        /* pop stack */
        if (n->arity == 2) {
            val_1_type = stack[index - 2]->terminal_type;
            val_2_type = stack[index - 1]->terminal_type;
            val_1 = regression_pop_stack(index - 2, stack, d);
            val_2 = regression_pop_stack(index - 1, stack, d);

        } else if (n->arity == 1) {
            val_1_type = stack[index - 1]->terminal_type;
            val_1 = regression_pop_stack(index - 1, stack, d);
        }

        /* evaluate function */
        switch (n->function) {
        case ADD:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] + *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case SUB:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] - *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case MUL:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] * *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case DIV:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] / *(float *) val_2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case POW:
            for (i = 0; i < d->rows; i++) {
                res = (float) pow(*(float *) val_1[i], *(float *) val_2[i]);
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case LOG:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) log(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case EXP:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) exp(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case RAD:
            for (i = 0; i < d->rows; i++) {
                res = *(float *) val_1[i] * (float) (PI / 180.0);
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case SIN:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) sin(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        case COS:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) cos(*(float *) val_1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result);
            break;

        default:
            /* UNRECOGNIZED FUNCTION */
            return -1;
        }

        /* clean up */
        if (val_1_type == EVAL) {
            free_mem_arr(val_1, d->rows, free);
        } else if (val_1_type == CONSTANT) {
            free(val_1);
        }

        if (val_2_type == EVAL) {
            free_mem_arr(val_2, d->rows, free);
        } else if (val_2_type == CONSTANT) {
            free(val_2);
        }

        node_destroy(n);
    }

    if (index == end) {
        return 0;
    } else {
        return regression_traverse(index + 1, end, stack, d);
    }
}

int regression_evaluate(struct tree *t, struct data *d, char *resp)
{
    int i;
    int res;
    int hits = 0;
    int col = data_field_index(d, resp);
    float err = 0.0;
    float sse = 0.0;
    struct node **stack = tree_copy_chromosome(t);
    struct node *n;

    /* evaluate stack */
    res = regression_traverse(0, t->size - 1, stack, d);
    silent_check(res != -1);
    n = stack[t->size - 1];

    /* check results */
    for (i = 0; i < d->rows; i++) {
        if (fltcmp(n->values[i], d->data[col][i]) == 0) {
            hits++;
        }

        err = (float) fabs(*(float *) n->values[i] - *(float *) d->data[col][i]);
        err = (float) pow(err, 2);
        sse += err;

        free(n->values[i]);
    }
    node_destroy(n);
    free(stack);
    t->score = malloc_float(sse + t->size);

    return 0;
error:
    return -1;
}
