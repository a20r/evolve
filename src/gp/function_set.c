#include <stdlib.h>
#include <string.h>

#include <dstruct/ast.h>

#include "gp/function_set.h"

struct ast *gp_default_function_factory(enum default_function_set func_type)
{
        struct ast *func;

        if (func_type == ADD) {
                func = ast_make_binary_exp("ADD", NULL, NULL);
        } else if (func_type == SUB) {
                func = ast_make_binary_exp("SUB", NULL, NULL);
        } else if (func_type == DIV) {
                func = ast_make_binary_exp("DIV", NULL, NULL);
        } else if (func_type == MUL) {
                func = ast_make_binary_exp("MUL", NULL, NULL);
        } else if (func_type == MOD) {
                func = ast_make_binary_exp("MOD", NULL, NULL);
        } else if (func_type == POW) {
                func = ast_make_binary_exp("POW", NULL, NULL);
        } else if (func_type == SQRT) {
                func = ast_make_binary_exp("SQRT", NULL, NULL);
        } else if (func_type == SIN) {
                func = ast_make_unary_exp("SIN", NULL);
        } else if (func_type == COS) {
                func = ast_make_unary_exp("COS", NULL);
        } else if (func_type == TAN) {
                func = ast_make_unary_exp("TAN", NULL);
        }

        return func;
}
