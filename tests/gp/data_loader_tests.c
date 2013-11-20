#include <stdlib.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "config/config.h"
#include "gp/data_loader.h"

#define TEST_INPUT_FILE "tests/test_files/sine_input.dat"
#define TEST_RESPONSE_FILE "tests/test_files/sine_response.dat"
#define TEST_CONFIG "tests/test_files/gp_data_loader.json"


int test_despace()
{
        char *line = NULL;
        int len = 0;
        int i = 0;

        /* setup */
        line = malloc(sizeof(char) * strlen("string with spaces \n") + 1);
        strcpy(line, "string with spaces \n");

        despace(line);

        /* assert */
        len = strlen(line);
        for (i = 0; i < len; i++) {
                mu_assert(line[i] != ' ', "Failed to remove space chars!");
        }
        free(line);

        return 0;
}

int test_lines_in_file()
{
        int lines = 0;

        lines = lines_in_file(TEST_INPUT_FILE);
        mu_assert(lines == 38, "Wrong line number count!");

        return 0;
}

int test_trim_newline()
{
        char *line = NULL;
        int len = 0;

        /* setup */
        line = malloc(sizeof(char) * strlen("some_string\n") + 1);
        strcpy(line, "some_string\n");

        trim_newline(line);

        /* assert */
        len = strlen(line);
        mu_assert(line[len - 1] != '\n', "Failed to remove newline char!");
        free(line);

        return 0;
}

int test_number_of_cols()
{
        char *line = NULL;
        char *delimiters = ",";
        int res = 0;

        /* setup */
        line = malloc(sizeof(char) * strlen("x, y, z") + 1);
        strcpy(line, "x, y, z");

        res = number_of_cols(line, delimiters);

        /* assert */
        mu_assert(res == 3, "Invalid number of fields!");
        free(line);

        return 0;
}

int test_parse_header_line()
{
        char *line = NULL;
        char *delimiters = ",";
        char **header = NULL;
        int fields = 3;
        int i = 0;
        int res = 0;

        /* PASS TEST */
        /* setup */
        line = malloc(sizeof(char) * strlen("x, y, z\n") + 1);
        strcpy(line, "x, y, z\n");
        header = calloc(fields, sizeof(char *));

        trim_newline(line);
        despace(line);
        res = parse_header_line(line, fields, delimiters, header);

        /* assert */
        mu_assert(res == 0, "Invalid return value!");
        for (i = 0; i < fields; i++) {
                printf("header[%i]: %s\n", i, header[i]);
                mu_assert(header[i] != NULL, "Failed to parse header!");
                free(header[i]);
        }

        free(line);
        free(header);

        /* FAIL TEST */
        /* setup */
        fields = 2;
        line = malloc(sizeof(char) * strlen("x, y, z") + 1);
        strcpy(line, "x, y, z");
        header = calloc(fields, sizeof(char *));

        trim_newline(line);
        despace(line);
        res = parse_header_line(line, fields, delimiters, header);

        /* #<{(| assert |)}># */
        mu_assert(res == -1, "Invalid return value!");
        for (i = 0; i < fields; i++) {
                mu_assert(header[i] != NULL, "Failed to parse header!");
                free(header[i]);
        }

        free(line);
        free(header);

        return 0;
}

int test_parse_data_line()
{
        char *line = NULL;
        char *delimiters = ",";
        float *data;
        float solution[3] = { 1.0, 1.2, 99.99 };
        int i = 0;
        int res = 0;
        int fields = 3;

        /* PASS TEST */
        /* setup */
        line = malloc(sizeof(char) * strlen("1.0, 1.2, 99.99") + 1);
        strcpy(line, "1.0, 1.2, 99.99");
        data = calloc(fields, sizeof(float));

        res = parse_data_line(line, fields, delimiters, data);

        /* assert */
        mu_assert(res == 0, "Invalid return value!");
        for (i = 0; i < fields; i++) {
                printf("data[%i]: %f\n", i, data[i]);
                res = float_epsilon_cmp(&data[i], &solution[i], 0.01);
                mu_assert(res == 0, "Failed to parse data!");
        }
        free(line);
        free(data);

        /* FAIL TEST */
        /* setup */
        fields = 2;
        line = malloc(sizeof(char) * strlen("1.0, 1.2, 99.99") + 1);
        strcpy(line, "1.0, 1.2, 99.99");
        data = calloc(fields, sizeof(float *));

        res = parse_data_line(line, fields, delimiters, data);

        /* assert */
        mu_assert(res == -1, "Invalid return value!");
        for (i = 0; i < fields; i++) {
                printf("data[%i]: %f\n", i, data[i]);
                res = float_epsilon_cmp(&data[i], &solution[i], 0.01);
                mu_assert(res == 0, "Failed to parse data!");
        }

        free(line);
        free(data);

        return 0;
}

int test_load_data()
{
        int res = 0;
        struct evolve_config *config;
        float **input_data;
        float **response_data;
        char **input_header;
        char **response_header;
        int i = 0;
        int j = 0;

        /* INPUT DATA */
        config = load_config(TEST_CONFIG);
        res = load_data(TEST_INPUT_FILE, config, INPUT_DATA);
        res = load_data(TEST_RESPONSE_FILE, config, RESPONSE_DATA);
        input_header = config->general.gp_tree->input_header;
        input_data = config->general.gp_tree->input_data;
        response_header = config->general.gp_tree->response_header;
        response_data = config->general.gp_tree->response_data;

        printf("--- INPUT DATA ---\n");
        printf("--- HEADER ---\n");
        for (i = 0; i < config->general.gp_tree->input_cols; i++) {
                printf("header[%d]: %s\n", i, input_header[i]);
        }

        printf("--- DATA ---\n");
        for (j = 0; j < config->general.gp_tree->input_rows; j++) {
                printf("%f\n", *(input_data[j]));
        }

        printf("\n\n--- RESPONSE DATA ---\n");
        printf("--- HEADER ---\n");
        for (i = 0; i < config->general.gp_tree->response_cols; i++) {
                printf("header[%d]: %s\n", i, response_header[i]);
        }

        printf("--- DATA ---\n");
        for (j = 0; j < config->general.gp_tree->response_rows; j++) {
                printf("%f\n", *(response_data[j]));
        }

        config_destroy(config);

        return 0;
}

void test_suite()
{
        mu_run_test(test_despace);
        mu_run_test(test_trim_newline);
        mu_run_test(test_lines_in_file);
        mu_run_test(test_number_of_cols);
        mu_run_test(test_parse_header_line);
        mu_run_test(test_parse_data_line);
        mu_run_test(test_load_data);
}

mu_run_test_suite(test_suite);
