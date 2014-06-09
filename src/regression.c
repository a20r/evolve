#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "cmp.h"
#include "tree.h"
#include "utils.h"
#include "regression.h"
#include "population.h"


float **regression_stack_pop(int index, struct node **stack, struct data *d)
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

void regression_stack_destroy(int index, int end, struct node **stack)
{
    int i;

    for (i = index; i <= end; i++) {
        node_clear_destroy(stack[i]);
        stack[i] = NULL;
    }

    free(stack);
}

void regression_free_vals(
    int in1_type,
    float **in1,
    int in2_type,
    float **in2,
    struct data *d
)
{
    if (in1_type == EVAL) {
        free_mem_arr(in1, d->rows, free);
    } else if (in1_type == CONSTANT) {
        free(in1);
    }

    if (in2_type == EVAL) {
        free_mem_arr(in2, d->rows, free);
    } else if (in2_type == CONSTANT) {
        free(in2);
    }
}

int regression_traverse(int index, int end, struct node **stack, struct data *d)
{
    int i;
    int in1_type = -1;
    int in2_type = -1;
    float res;
    float zero = 0.0;
    float **result = NULL;
    float **in1 = NULL;
    float **in2 = NULL;
    struct node *n = stack[index];

    if (n->type == FUNC_NODE) {
        /* result array */
        result = malloc(sizeof(float *) * (unsigned long) d->rows);

        /* pop stack */
        if (n->arity == 2) {
            in1_type = stack[index - 2]->terminal_type;
            in2_type = stack[index - 1]->terminal_type;
            in1 = regression_stack_pop(index - 2, stack, d);
            in2 = regression_stack_pop(index - 1, stack, d);

        } else if (n->arity == 1) {
            in1_type = stack[index - 1]->terminal_type;
            in1 = regression_stack_pop(index - 1, stack, d);
        }

        /* evaluate function */
        switch (n->function) {
        case ADD:
            for (i = 0; i < d->rows; i++) {
                res = *in1[i] + *in2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case SUB:
            for (i = 0; i < d->rows; i++) {
                res = *in1[i] - *in2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case MUL:
            for (i = 0; i < d->rows; i++) {
                res = *in1[i] * *in2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case DIV:
            silent_check(fltcmp(in2, &zero) != 0);  /* check for zero */
            for (i = 0; i < d->rows; i++) {
                res = *in1[i] / *in2[i];
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case POW:
            for (i = 0; i < d->rows; i++) {
                res = (float) pow(*in1[i], *in2[i]);
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case LOG:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) log(*in1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case EXP:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) exp(*in1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case RAD:
            for (i = 0; i < d->rows; i++) {
                res = *in1[i] * (float) (PI / 180.0);
                result[i] = malloc_float(res);
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case SIN:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) sin(*in1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        case COS:
            for (i = 0; i < d->rows; i++) {
                result[i] = malloc_float((float) cos(*in1[i]));
            }
            stack[index] = node_new_eval(FLOAT, (void **) result, d->rows);
            break;

        default:
            /* UNRECOGNIZED FUNCTION */
            goto func_error;
        }

        /* clean up */
        regression_free_vals(in1_type, in1, in2_type, in2, d);
        node_destroy(n);
    }

    /* terminate check */
    if (index == end) {
        return 0;
    } else {
        return regression_traverse(index + 1, end, stack, d);
    }

error:
    regression_free_vals(in1_type, in1, in2_type, in2, d);
    regression_stack_destroy(index + 1, end, stack);
    return -1;

func_error:
    regression_free_vals(in1_type, in1, in2_type, in2, d);
    regression_stack_destroy(index + 1, end, stack);
    return -2;
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

        err = (float) fabs(*(float *) n->values[i] - *d->data[col][i]);
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

int regression_evaluate_population(struct population *p, struct data *d)
{
    int i;
    struct tree *t;

    for (i = 0; i < p->size; i++) {
        t = (struct tree *) p->individuals[i];
        regression_evaluate(t, d, (char *) "y");
    }


   return 0;
}
