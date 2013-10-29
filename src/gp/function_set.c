#include <stdlib.h>
#include <string.h>

#include <dstruct/ast.h>

#include "gp/function_set.h"

struct ast *gp_default_function_factory(char *func_type)
{
        struct ast *func;

        if (strcmp(func_type, "ADD") == 0) {
                func = ast_make_binary_exp("ADD", NULL, NULL);
        } else if (strcmp(func_type, "SUB") == 0) {
                func = ast_make_binary_exp("SUB", NULL, NULL);
        } else if (strcmp(func_type, "DIV") == 0) {
                func = ast_make_binary_exp("DIV", NULL, NULL);
        } else if (strcmp(func_type, "MUL") == 0) {
                func = ast_make_binary_exp("MUL", NULL, NULL);
        } else if (strcmp(func_type, "MOD") == 0) {
                func = ast_make_binary_exp("MOD", NULL, NULL);
        } else if (strcmp(func_type, "POW") == 0) {
                func = ast_make_binary_exp("POW", NULL, NULL);
        } else if (strcmp(func_type, "SQRT") == 0) {
                func = ast_make_binary_exp("SQRT", NULL, NULL);
        } else if (strcmp(func_type, "SIN") == 0) {
                func = ast_make_unary_exp("SIN", NULL);
        } else if (strcmp(func_type, "COS") == 0) {
                func = ast_make_unary_exp("COS", NULL);
        } else if (strcmp(func_type, "TAN") == 0) {
                func = ast_make_unary_exp("TAN", NULL);
        } else {
                return NULL;
        }

        return func;
}
