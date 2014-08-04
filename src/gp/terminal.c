#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "gp/terminal.h"


struct terminal_set *terminal_set_new(struct terminal **terminals, int n)
{
    int i;
    struct terminal_set *ts;

    ts = malloc(sizeof(struct terminal_set));
    ts->length = n;
    ts->terminals = malloc(sizeof(struct terminal *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        ts->terminals[i] = terminals[i];
    }

    return ts;
}

void terminal_set_destroy(void *set)
{
    int i;
    struct terminal_set *ts = (struct terminal_set *) set;

    if (set) {
        for (i = 0; i < ts->length; i++) {
            terminal_destroy(ts->terminals[i]);
        }

        free_mem(ts->terminals, free);
        free_mem(ts, free);
    }
}

struct terminal *terminal_new(int type, int value_type, void *value)
{
    struct terminal *t = malloc(sizeof(struct terminal));
    t->type = type;
    t->value_type = value_type;
    t->value = NULL;

    t->min = NULL;
    t->max = NULL;
    t->precision = -1;

    if (value == NULL) {
        return t;
    }

    switch (value_type) {
    case INTEGER:
        t->value = malloc(sizeof(int));
        *(int *) t->value = *(int *) value;
        break;
    case FLOAT:
        t->value = malloc(sizeof(float));
        *(float *) t->value = *(float *) value;
        break;
    case DOUBLE:
        t->value = malloc(sizeof(double));
        *(double *) t->value = *(double *) value;
        break;
    case STRING:
        t->value = malloc(sizeof(char) * strlen(value) + 1);
        strcpy(t->value, value);
        break;
    }

    return t;
}

struct terminal *terminal_new_input(char *input_name)
{
    struct terminal *t = terminal_new(INPUT, STRING, input_name);
    return t;
}

struct terminal *terminal_new_constant(int type, void *value)
{
    struct terminal *t = terminal_new(CONSTANT, type, value);
    return t;
}

struct terminal *terminal_new_random_constant(
    int type,
    void *min,
    void *max,
    int precision
)
{
    struct terminal *t = terminal_new(RANDOM_CONSTANT, type, NULL);

    t->min = copy_value(type, min);
    t->max = copy_value(type, max);
    t->precision = precision;

    return t;
}

void terminal_destroy(void *t)
{
    struct terminal *term = (struct terminal *) t;

    if (t) {
        if (term->value) {
            free(term->value);
        }

        if (term->type == RANDOM_CONSTANT) {
            free(term->min);
            free(term->max);
        }

        free(term);
    }
}

void *terminal_resolve_random(struct terminal *t)
{
    int i;
    float f;
    double d;
    float precision = (float) pow(10 , t->precision);
    void *retval = NULL;

    switch (t->value_type) {
    case INTEGER:
        i = randi(*(int *) t->min, *(int *) t->max);
        retval = malloc(sizeof(int));
        *(int *) retval = i;
        break;
    case FLOAT:
        f = randf(*(float *) t->min, *(float *) t->max);
        if (t->precision != -1) {
            if (t->precision == 0) {
                f = (int) f;
            } else {
                f = roundf(f * precision) / precision;
            }
        }

        retval = malloc(sizeof(f));
        *(float *) retval = f;
        break;
    case DOUBLE:
        d = randf((float) *(double *) t->min, (float) *(double *) t->max);
        if (t->precision != -1) {
            if (t->precision == 0) {
                d = (int) d;
            } else {
                d = round(d * precision) / precision;
            }
        }

        retval = malloc(sizeof(double));
        *(double *) retval = d;
        break;
    default:
        retval = NULL;
        break;
    }

    return retval;
}



