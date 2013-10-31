#include <stdlib.h>
#include <string.h>

#include <dstruct/ast.h>
#include <dbg/dbg.h>

#include "gp/function_set.h"


struct ast *gp_function_factory(char *tag, char *op_name)
{
        struct ast *func;

        if (strcmp(tag, "UNARY_OP") == 0) {
                func = ast_make_unary_exp(op_name, NULL);
        } else if (strcmp(tag, "BINARY_OP") == 0) {
                func = ast_make_binary_exp(op_name, NULL, NULL);
        } else {
                log_err("Error! Unknown function type [%s]\n", tag);
                return NULL;
        }

        return func;
}
