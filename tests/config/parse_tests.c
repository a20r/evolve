#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>
#include <dstruct/ast.h>
#include <dbg/dbg.h>

#include "config/parse.h"

#define TEST_CONFIG_FILE "tests/test_files/parse_test.json"
#define TEST_GET_STR "Hello World!"
#define TEST_GET_INT 123
#define TEST_GET_REAL 9.999
#define TEST_GET_JSON "{ \"print\": \"Hello World\" }"

#define TEST_SET_STR "testing, testing, 1-2-3, 1-2-3"
#define TEST_SET_INT 1
#define TEST_SET_REAL 9.9999

/* GLOBAL VARS */
json_t *root;
json_error_t error;


static void setup()
{
        root = json_load_file(TEST_CONFIG_FILE, 0, &error);
        check(root != NULL, "Failed to read config file!");

        return;
error:
        exit(-1);
        return;
}

static void teardown()
{
        json_decref(root);
}

int test_get_str()
{
        int res = 0;
        char *value;
        char *solution;

        /* get value */
        value = get_str(root, "test_get_str");
        solution = TEST_GET_STR;
        printf("value: [%s] solution: [%s]\n", value, solution);

        /* assert */
        res = strcmp(value, solution);
        mu_assert(res == 0, "Failed to get_str()!");

        /* clean up */
        free(value);
        return 0;
}

int test_get_int()
{
        int res = 0;
        int value = 0;
        int *solution;

        /* get value */
        value = get_int(root, "test_get_int");
        solution = calloc(1, sizeof(int));
        *solution = TEST_GET_INT;
        printf("value: [%d] solution: [%d]\n", value, *solution);

        /* assert */
        res = int_cmp(&value, solution);
        mu_assert(res == 0, "Failed to get_int()!");

        /* clean up */
        free(solution);
        return 0;
}

int test_get_real()
{
        int res = 0;
        float value = 0.0;
        float *solution;

        /* get value */
        value = get_real(root, "test_get_real");
        solution = calloc(1, sizeof(float));
        *solution = TEST_GET_REAL;
        printf("value: [%f] solution: [%f]\n", value, *solution);

        /* assert */
        res = float_cmp(&value, solution);
        mu_assert(res == 0, "Failed to get_real()!");

        /* clean up */
        free(solution);
        return 0;
}

int test_get_json()
{
        int res = 0;
        json_t *value;
        json_error_t err;
        json_t *solution;
        char *value_print;
        char *solution_print;

        /* get value */
        value = get_json(root, "test_get_json");
        solution = json_loads(TEST_GET_JSON, JSON_REJECT_DUPLICATES, &err);

        value_print = get_str(value, "print");
        solution_print = get_str(value, "print");
        printf("value: { \"print\": \"%s\" }\n", value_print);
        printf("solution: { \"print\": \"%s\" }\n", solution_print);

        /* assert */
        res = json_equal(value, solution);
        mu_assert(res == 1, "Failed to get_json()!");

        /* clean up */
        json_decref(solution);
        free(value_print);
        free(solution_print);
        return 0;
}

int test_set_str()
{
        int res = 0;
        char *target;
        char *solution;

        /* set target and solution */
        res = set_str(root, "test_set_str", &target);
        solution = calloc(1, sizeof(char) * strlen(TEST_SET_STR) + 1);
        strcpy(solution, TEST_SET_STR);
        printf("target: [%s] solution: [%s]\n", target, solution);

        /* assert */
        mu_assert(res == 0, "Failed to set_str()!");

        res = strcmp(target, solution);
        mu_assert(res == 0, "Failed to set_str()!");

        /* clean up */
        free(target);
        free(solution);
        return 0;
}

int test_set_int()
{
        int res = 0;
        int target;
        int solution;

        /* set target and solution */
        res = set_int(root, "test_set_int", &target);
        solution = TEST_SET_INT;
        printf("target: [%d] solution: [%d]\n", target, solution);

        /* assert */
        mu_assert(res == 0, "Failed to set_int()!");

        res = int_cmp(&target, &solution);
        mu_assert(res == 0, "Failed to set_int()!");

        return 0;
}

int test_set_real()
{
        int res = 0;
        float target;
        float solution;

        /* set target and solution */
        res = set_real(root, "test_set_real", &target);
        solution = TEST_SET_REAL;
        printf("target: [%f] solution: [%f]\n", target, solution);

        /* assert */
        mu_assert(res == 0, "Failed to set_real()!");

        res = float_cmp(&target, &solution);
        mu_assert(res == 0, "Failed to set_real()!");

        return 0;
}

int test_set_string_array()
{
        int res = 0;
        int i = 0;
        struct darray *target;
        char *solution = NULL;

        /* set target and solution */
        target = darray_create(sizeof(char *), 100);
        res = set_string_array(root, "test_set_string_array", target);

        /* assert */
        mu_assert(target->end != 0, "target should not have length 0!");
        mu_assert(res == 0, "Failed to set_string_array()!");

        for (i = 0; i <= target->end; i++) {
                solution = darray_get(target, i);
                printf("solution: [%s]\n", solution);
                mu_assert(solution != NULL, "Failed to set_string_array()!");
                solution = NULL;
        }

        /* clean up */
        darray_clear_destroy(target);
        return 0;
}

int test_set_ast_array()
{
        int res = 0;
        int i = 0;
        struct darray *target;
        struct ast *solution = NULL;

        /* set target and solution */
        target = darray_create(sizeof(struct ast *), 100);
        res = set_ast_array(
                root,
                "test_set_ast_array_function_mode",
                target,
                FUNCTION_MODE
        );

        /* FUNCTION MODE */
        mu_assert(target->end != 0, "target should not have length 0!");
        mu_assert(res == 0, "Failed to set_ast_array()!");

        for (i = 0; i <= target->end; i++) {
                solution = darray_get(target, i);

                if (solution->tag == UNARY_OP) {
                        printf("node: %s\n", solution->type.unary->op_name);
                } else if (solution->tag == BINARY_OP) {
                        printf("node: %s\n", solution->type.binary->op_name);
                }

                mu_assert(solution != NULL, "Failed to set_ast_array()!");
                solution = NULL;
        }

        /* clean up */
        for (i = 0; i <= target->end; i++) {
                solution = darray_get(target, i);
                ast_destroy(solution);
        }
        darray_destroy(target);

        /* TERMINAL MODE */
        target = darray_create(sizeof(struct ast *), 100);
        res = set_ast_array(
                root,
                "test_set_ast_array_terminal_mode",
                target,
                TERMINAL_MODE
        );

        /* assert */
        mu_assert(target->end != 0, "target should not have length 0!");
        mu_assert(res == 0, "Failed to set_ast_array()!");

        for (i = 0; i <= target->end; i++) {
                solution = darray_get(target, i);

                if (solution->tag == STRING) {
                        printf("node: %s\n", solution->type.string);
                } else if (solution->tag == INTEGER) {
                        printf("node: %d\n", solution->type.integer);
                } else if (solution->tag == REAL) {
                        printf("node: %f\n", solution->type.real);
                }

                mu_assert(solution != NULL, "Failed to set_ast_array()!");
                solution = NULL;
        }


        /* clean up */
        for (i = 0; i <= target->end; i++) {
                solution = darray_get(target, i);
                ast_destroy(solution);
        }
        darray_destroy(target);

        return 0;
}

int test_parse_ga_config()
{
        return 0;
}

int test_parse_gp_tree_config()
{
        return 0;
}

int test_parse_general_config()
{
        return 0;
}

int test_parse_selection_config()
{
        return 0;
}

int test_parse_crossover_config()
{
        return 0;
}

int test_parse_mutation_config()
{
        return 0;
}

void test_suite()
{
        setup();

        mu_run_test(test_get_str);
        mu_run_test(test_get_int);
        mu_run_test(test_get_real);
        mu_run_test(test_get_json);
        mu_run_test(test_set_str);
        mu_run_test(test_set_int);
        mu_run_test(test_set_real);
        mu_run_test(test_set_string_array);
        mu_run_test(test_set_ast_array);

        mu_run_test(test_parse_ga_config);
        mu_run_test(test_parse_gp_tree_config);
        mu_run_test(test_parse_general_config);
        mu_run_test(test_parse_selection_config);
        mu_run_test(test_parse_crossover_config);
        mu_run_test(test_parse_mutation_config);

        teardown();
}

mu_run_test_suite(test_suite);
