#ifndef _TERMINAL_H_
#define _TERMINAL_H_


/* TERMINAL TYPE */
#define INPUT 0
#define CONSTANT 1
#define RANDOM_CONSTANT 2
#define EVAL 3

/* TERMINAL VALUE TYPE */
#define INTEGER 0
#define FLOAT 1
#define DOUBLE 2
#define STRING 3


/* STRUCTS */
struct terminal_set
{
    int length;
    struct terminal **terminals;
};

struct terminal
{
    int type;
    int value_type;
    void *value;

    void *min;
    void *max;
    int precision;
};

/* FUNCTIONS */
struct terminal_set *terminal_set_new(struct terminal **terminals, int n);
void terminal_set_destroy(void *set);
struct terminal *terminal_new(int type, int value_type, void *value);
struct terminal *terminal_new_input(char *input_name);
struct terminal *terminal_new_constant(int type, void *value);
struct terminal *terminal_new_random_constant(
    int type,
    void *min,
    void *max,
    int precision
);
void terminal_destroy(void *f);
void *terminal_resolve_random(struct terminal *t);

#endif
