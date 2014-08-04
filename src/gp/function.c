#include <stdlib.h>

#include "utils.h"
#include "gp/function.h"


/* FUNCTION SET */
struct function_set *function_set_new(struct function **functions, int n)
{
    int i;
    struct function_set *fs = NULL;

    fs = malloc(sizeof(struct function_set));
    fs->length = n;
    fs->functions = malloc(sizeof(struct function *) * (unsigned long) n);

    for (i = 0; i < n; i++) {
        if (functions[i]) {
            fs->functions[i] = functions[i];
        } else {
            fs->functions[i] = NULL;
        }
    }

    return fs;
}

void function_set_destroy(void *set)
{
    int i;
    struct function_set *fs = (struct function_set *) set;

    if (set != NULL) {
        for (i = 0; i < fs->length; i++) {
            if (fs->functions[i]) {
                function_destroy(fs->functions[i]);
            }
        }

        free(fs->functions);
        free(fs);
    }
}

struct function *function_new(int type, int function, int arity)
{
    struct function *f;

    f = malloc(sizeof(struct function));

    f->type = type;
    f->function = function;
    f->arity = arity;

    return f;
}

struct function *function_new_func(int function, int arity)
{
    struct function *f = function_new(DEFAULT, function, arity);
    return f;
}

struct function *function_new_cfunc(int function, int arity)
{
    struct function *f = function_new(CLASSIFICATION, function, arity);
    return f;
}

void function_destroy(void *f)
{
    free_mem(f, free);
}
