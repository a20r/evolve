#ifndef _FUNCTION_H_
#define _FUNCTION_H_


/* FUNCTION TYPE */
#define DEFAULT 0
#define CLASSIFICATION 1


/* STRUCTS */
struct function_set
{
    int length;
    struct function **functions;
};

struct function
{
    int type;
    int function;
    int arity;
};

/* FUNCTIONS */
struct function_set *function_set_new(struct function **functions, int n);
void function_set_destroy(void *set);
struct function *function_new(int type, int function, int arity);
struct function *function_new_func(int function, int arity);
struct function *function_new_cfunc(int function, int arity);
void function_destroy(void *f);

#endif
