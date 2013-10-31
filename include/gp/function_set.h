#ifndef _FUNCTION_SET_H_
#define _FUNCTION_SET_H_

/* FUNCTIONS */
struct ast *gp_default_function_factory(char *func_type);
struct ast *gp_function_factory(char *tag, char *op_name);

#endif
