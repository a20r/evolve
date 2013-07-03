#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve.h"
#include "evolve_utils.h"
#include "selection.h"
#include "utils.h"

#define TEST_OUTPUT "evolve_utils_tests.out"

/* GLOBAL VAR */
struct population *p;
int output;
int save_stdout;


static float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}

static void setup()
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                1 /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);

        /* open output file */
        output = open(TEST_OUTPUT, O_RDWR|O_CREAT, 0755);
        if (output == -1) {
                printf("Error opening [%s]!\n", TEST_OUTPUT);
                exit(-1);
        }

        /* save stdout */
        save_stdout = dup(1);

        /* redirect stdout to file */
        if (dup2(output, 1) == -1) {
                printf("Failed to redirect stdout!\n");
                exit(-1);
        }
}

static void teardown()
{
        /* flush output */
        fflush(stdout);
        close(output);

        destroy_population(&p);

        /* restore stdout */
        dup2(save_stdout, 1);
        close(save_stdout);
}

static void cleanup()
{
        int result = remove(TEST_OUTPUT);
        if (result == 0) {
                printf("Removed file [%s]!\n", TEST_OUTPUT);
        } else {
                printf("ERROR Failed to removed file [%s]!\n", TEST_OUTPUT);
        }

}

static int read_file(char* fp)
{
        int line_counter = 0;
        char line[128];

        FILE *file = fopen(fp, "r");

        while (fgets(line, sizeof(line), file) != NULL) {
                fputs(line, stdout);
                line_counter++;
        }

        fclose(file);

        return line_counter;
}

int test_print_chromosome()
{
        setup();
        print_chromosome(p, 0);
        teardown();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_print_chromosomes()
{
        setup();
        print_chromosomes(p);
        teardown();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_print_population()
{
        setup();
        print_population(p);
        teardown();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

void test_suite()
{
        mu_run_test(test_print_chromosome);
        mu_run_test(test_print_chromosomes);
        mu_run_test(test_print_population);

        cleanup();
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
