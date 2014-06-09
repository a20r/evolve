#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "cmp.h"
#include "tree.h"
#include "utils.h"
#include "stack.h"
#include "regression.h"
#include "population.h"

int regression_stack_pop(
    struct node *n,
    struct data *d,
    float **func_input,
    int nth_arity
)
{
    int i;
    int col_index;

    if (n->terminal_type == INPUT) {
        col_index = data_field_index(d, n->value);
        for (i = 0; i < d->rows; i++) {
            func_input[nth_arity][i] = *d->data[col_index][i];
        }

    } else if (n->terminal_type == CONSTANT) {
        for (i = 0; i < d->rows; i++) {
            func_input[nth_arity][i] = *(float *) n->value;
        }

    } else if (n->terminal_type == EVAL) {
        for (i = 0; i < d->rows; i++) {
            func_input[nth_arity][i] = ((float *) n->values)[i];
        }
    }
    node_destroy(n);

    return 0;
}

void regression_stack_destroy(int index, int end, struct node **stack)
{
    int i;

    for (i = index; i <= end; i++) {
        node_destroy(stack[i]);
        stack[i] = NULL;
    }

    free(stack);
}

int regression_traverse(
    int index,
    int end,
    struct node **stack,
    float **func_input,
    struct data *d
)
{
    int i;
    float res;
    float zero = 0.0;
    float *result = NULL;
    struct node *n = stack[index];

    if (n->type == FUNC_NODE) {
        /* result array */
        result = malloc(sizeof(float) * (unsigned long) d->rows);

        /* prep function input data */
        if (n->arity == 2) {
            regression_stack_pop(stack[index - 2], d, func_input, 0);
            regression_stack_pop(stack[index - 1], d, func_input, 1);
        } else if (n->arity == 1) {
            regression_stack_pop(stack[index - 1], d, func_input, 0);
        }

        /* evaluate function */
        switch (n->function) {
        case ADD:
            for (i = 0; i < d->rows; i++) {
                res = func_input[0][i] + func_input[1][i];
                result[i] = res;
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case SUB:
            for (i = 0; i < d->rows; i++) {
                res = func_input[0][i] - func_input[1][i];
                result[i] = res;
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case MUL:
            for (i = 0; i < d->rows; i++) {
                res = func_input[0][i] * func_input[1][i];
                result[i] = res;
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case DIV:
            for (i = 0; i < d->rows; i++) {
                /* check for zero */
                silent_check(fltcmp(&func_input[1][i], &zero) != 0);

                res = func_input[0][i] / func_input[1][i];
                result[i] = res;
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case POW:
            for (i = 0; i < d->rows; i++) {
                res = (float) pow(func_input[0][i], func_input[1][i]);
                result[i] = res;
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case LOG:
            for (i = 0; i < d->rows; i++) {
                result[i] = (float) log(func_input[0][i]);
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case EXP:
            for (i = 0; i < d->rows; i++) {
                result[i] = (float) exp(func_input[0][i]);
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case RAD:
            for (i = 0; i < d->rows; i++) {
                result[i] = func_input[0][i] * (float) (PI / 180.0);
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case SIN:
            for (i = 0; i < d->rows; i++) {
                result[i] = (float) sin(func_input[0][i]);
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        case COS:
            for (i = 0; i < d->rows; i++) {
                result[i] = (float) cos(func_input[0][i]);
            }
            stack[index] = node_new_eval(FLOAT, result, d->rows);
            break;

        default:
            /* UNRECOGNIZED FUNCTION */
            goto func_error;
        }

        /* clean up */
        node_destroy(n);
    }

    /* terminate check */
    if (index == end) {
        return 0;
    } else {
        return regression_traverse(index + 1, end, stack, func_input, d);
    }

error:
    regression_stack_destroy(index, end, stack);
    free(result);
    return -1;

func_error:
    regression_stack_destroy(index, end, stack);
    free(result);
    return -2;
}

int regression_evaluate(
    struct tree *t,
    float **func_input,
    struct data *d,
    char *resp
)
{
    int i;
    int res;
    int hits = 0;
    int col = data_field_index(d, resp);
    float zero = 0.0;
    float err = 0.0;
    float sse = 0.0;
    struct node **stack = tree_copy_chromosome(t);
    struct node *n;

    /* evaluate stack */
    res = regression_traverse(0, t->size - 1, stack, func_input, d);
    silent_check(res != -1);
    n = stack[t->size - 1];

    /* check results */
    for (i = 0; i < d->rows; i++) {
        err = (float) fabs(((float *) n->values)[i] - *d->data[col][i]);
        err = (float) pow(err, 2);
        sse += err;

        if (fltcmp(&err, &zero) == 0) {
            hits++;
        }
    }
    node_destroy(n);
    free(stack);

    /* record evaluation */
    t->score = malloc_float(sse + t->size);
    t->hits = hits;

    return 0;
error:
    return -1;
}

int regression_evaluate_population(struct population *p, struct data *d)
{
    int i;
    float **func_input;

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* evaluate population */
    for (i = 0; i < p->size; i++) {
        tree_print(p->individuals[i]);
        regression_evaluate(p->individuals[i], func_input, d, (char *) "y");
    }

    /* struct tree *t = p->individuals[i]; */
    /* printf("tree->depth: %d\n", t->depth); */
    /* for (i = 0;  i < t->size; i++) { */
    /*     node_print(t->chromosome[i]); */
    /* } */
    /* printf("\n"); */

    /* clean up */
    free_mem_arr(func_input, 2, free);
    return 0;
}
