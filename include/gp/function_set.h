#ifndef _FUNCTION_SET_H_
#define _FUNCTION_SET_H_

enum default_function_set
{
    ADD,
    SUB,

    MUL,
    DIV,
    MOD,

    POW,
    SQRT,

    SIN,
    COS,
    TAN,
};

/* FUNCTIONS */
struct ast *gp_default_function_factory(enum default_function_set func_type);

#endif
