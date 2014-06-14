#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <errno.h>

#include "stack.h"
#include "utils.h"
#include "population.h"
#include "gp/tree.h"
#include "gp/regression.h"


int regression_func_input(
    struct node *n,
    struct data *d,
    float **func_input,
    int nth_arity
)
{
    int i;
    int type;
    int col_index;
    float f;

    type = n->terminal_type;
    if (type == INPUT) {
        col_index = data_field_index(d, n->value);
        for (i = 0; i < d->rows; i++) {
            func_input[nth_arity][i] = *d->data[col_index][i];
        }

    } else if (type == CONSTANT || type == RANDOM_CONSTANT) {
        for (i = 0; i < d->rows; i++) {
            func_input[nth_arity][i] = *(float *) n->value;
        }

    } else if (type == EVAL) {
        for (i = 0; i < d->rows; i++) {
            f = ((float *) n->values)[i];
            func_input[nth_arity][i] = f;

            if (isnan(f) != 0 || isinf(f) == -1 || isinf(f) == 1) {
                return -1;
            }
        }
    }

    return 0;
}

void regression_free_inputs(
    int in1_type,
    struct node *in1,
    int in2_type,
    struct node *in2
)
{
    if (in1_type == EVAL) {
        node_destroy(in1);
    }
    if (in2_type == EVAL) {
        node_destroy(in2);
    }
}

int regression_check(void)
{
    if (errno == EDOM) {
        return -1;
    } else if (errno == ERANGE) {
        return -1;
    }

    return 0;
}

int regression_traverse(
    int index,
    int end,
    struct node **chromosome,
    struct stack *stack,
    float **func_input,
    struct data *d
)
{
    int i;
    float zero = 0.0;
    float *result = NULL;
    struct node *n = chromosome[index];
    struct node *in1 = NULL;
    struct node *in2 = NULL;
    int in1_type = -1;
    int in2_type = -1;

    errno = 0;
    if (n->type == TERM_NODE) {
        stack_push(stack, n);

    } else if (n->type == FUNC_NODE) {
        /* prep function input data */
        if (n->arity == 2) {
            in2 = stack_pop(stack);
            in1 = stack_pop(stack);
            in1_type = in1->terminal_type;
            in2_type = in2->terminal_type;

            silent_check(regression_func_input(in1, d, func_input, 0) == 0);
            silent_check(regression_func_input(in2, d, func_input, 1) == 0);

        } else {
            in1 = stack_pop(stack);
            in1_type = in1->terminal_type;

            silent_check(regression_func_input(in1, d, func_input, 0) == 0);

        }

        /* evaluate function */
        result = malloc(sizeof(float) * (unsigned long) d->rows);
        switch (n->function) {
        case ADD:
            for (i = 0; i < d->rows; i++) {
                result[i] = func_input[0][i] + func_input[1][i];
                silent_check(regression_check() == 0);
            }
            break;

        case SUB:
            for (i = 0; i < d->rows; i++) {
                result[i] = func_input[0][i] - func_input[1][i];
                silent_check(regression_check() == 0);
            }
            break;

        case MUL:
            for (i = 0; i < d->rows; i++) {
                result[i] = func_input[0][i] * func_input[1][i];
                silent_check(regression_check() == 0);
            }
            break;

        case DIV:
            for (i = 0; i < d->rows; i++) {
                /* check for zero */
                silent_check(fltcmp(&func_input[1][i], &zero) != 0);
                result[i] = func_input[0][i] / func_input[1][i];
                silent_check(regression_check() == 0);
            }
            break;

        case POW:
            for (i = 0; i < d->rows; i++) {
                func_input[0][i] = fabsf(func_input[0][i]);
                result[i] = powf(func_input[0][i], func_input[1][i]);
                silent_check(regression_check() == 0);
            }
            break;

        case LOG:
            for (i = 0; i < d->rows; i++) {
                /* check for zero and negative */
                silent_check(fltcmp(&func_input[0][i], &zero) == 1);
                result[i] = logf(func_input[0][i]);
                silent_check(regression_check() == 0);
            }
            break;

        case EXP:
            for (i = 0; i < d->rows; i++) {
                result[i] = expf(func_input[0][i]);
                silent_check(regression_check() == 0);
            }
            break;

        case RAD:
            for (i = 0; i < d->rows; i++) {
                result[i] = func_input[0][i] * (float) (PI / 180.0);
                silent_check(regression_check() == 0);
            }
            break;

        case SIN:
            for (i = 0; i < d->rows; i++) {
                result[i] = sinf(func_input[0][i]);
                silent_check(regression_check() == 0);
            }
            break;

        case COS:
            for (i = 0; i < d->rows; i++) {
                result[i] = cosf(func_input[0][i]);
                silent_check(regression_check() == 0);
            }
            break;

        default:
            /* UNRECOGNIZED FUNCTION */
            goto func_error;
        }

        stack_push(stack, node_new_eval(FLOAT, result, d->rows));
        regression_free_inputs(in1_type, in1, in2_type, in2);
    }

    /* terminate check */
    if (index == end) {
        return 0;
    } else {
        return regression_traverse(
            index + 1,
            end,
            chromosome,
            stack,
            func_input,
            d
        );
    }

error:
    regression_free_inputs(in1_type, in1, in2_type, in2);
    free(result);
    return -1;

func_error:
    regression_free_inputs(in1_type, in1, in2_type, in2);
    free(result);
    return -2;
}

void regression_clear_stack(struct stack *s)
{
    int i;
    struct node *n;

    for (i = 0; s->size; i++) {
        n = stack_pop(s);
        if (n->terminal_type == EVAL) {
            node_destroy(n);
        }
    }
    free(s);
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
    float value;
    float zero = 0.0;
    float err = 0.0;
    float sse = 0.0;
    struct node *result;
    struct stack *s = stack_new();

    /* evaluate chromosome */
    res = regression_traverse(0, t->size - 1, t->chromosome, s, func_input, d);
    silent_check(res != -1);

    /* check results */
    result = stack_pop(s);
    for (i = 0; i < d->rows; i++) {
        value = ((float *) result->values)[i];
        err = (float) fabs(value - *d->data[col][i]);
        err = (float) pow(err, 2);
        sse += err;

        if (fltcmp(&err, &zero) == 0) {
            hits++;
        }
    }

    /* record evaluation */
    if (t->score == NULL) {
        t->score = malloc_float(sse + t->size);
    } else {
        *t->score = (sse + t->size);
    }
    t->hits = hits;

    /* clean up */
    node_destroy(result);
    regression_clear_stack(s);
    return 0;
error:
    free(t->score);
    t->score = NULL;
    regression_clear_stack(s);
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
        regression_evaluate(p->individuals[i], func_input, d, (char *) "y");
    }

    /* clean up */
    free_mem_arr(func_input, 2, free);
    return 0;
}

void regression_print_traverse(struct node *n)
{
    if (n->type == TERM_NODE) {
        if (n->value_type == INTEGER) {
            printf("%d", *(int *) n->value);
        } else if (n->value_type == FLOAT) {
            printf("%f", *(float *) n->value);
        } else if (n->value_type == STRING) {
            printf("%s", (char *) n->value);
        }

    } else if (n->type == FUNC_NODE) {
        printf("(");

        if (n->arity == 2) {
            regression_print_traverse(n->children[0]);
        }

        switch (n->function) {
        case ADD:
            printf(" + ");
            break;
        case SUB:
            printf(" - ");
            break;
        case MUL:
            printf(" * ");
            break;
        case DIV:
            printf(" / ");
            break;
        case POW:
            printf("^");
            break;
        case LOG:
            printf("log");
            printf("(");
            break;
        case EXP:
            printf("exp");
            printf("(");
            break;
        case RAD:
            printf("rad");
            printf("(");
            break;
        case SIN:
            printf("sin");
            printf("(");
            break;
        case COS:
            printf("cos");
            printf("(");
            break;
        default:
            log_err("Opps! Don't recognize that function -> %d", n->function);
            break;
        }

        if (n->arity == 2) {
            regression_print_traverse(n->children[1]);
        } else if (n->arity == 1) {
            regression_print_traverse(n->children[0]);
            printf(")");
        }

        printf(")");
    }

}

void regression_print(void *target)
{
    struct tree *t = (struct tree *) target;
    regression_print_traverse(t->root);
    printf("\n");
}

