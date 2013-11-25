#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"
#include "selection.h"
#include "gp/initialize.h"
#include "gp/data_loader.h"
#include "gp/crossover.h"
#include "gp/mutation.h"
#include "gp/tree_evaluator.h"
#include "gp/tree_parser.h"

#define TEST_INPUT_DATA "examples/sym_regression/sine_input.dat"
#define TEST_RESPONSE_DATA "examples/sym_regression/sine_response.dat"


static void print_program(struct gp_tree *tree)
{
        int i;
        struct ast *node;

        /* print program */
        printf("PROGRAM STACK [REVERSE POLISH NOTATION]\n");
        for (i = 0; i < tree->program->end; i++) {
                node = darray_get(tree->program, i);
                print_node(node);
                printf("\n");
        }
}

int main(int argc, char *argv[])
{
        struct evolve_config *config;
        struct evolve_monitor *m;
        struct population *p;
        char *config_fp;

        /* parse arguments */
        if (argc == 1) {
                config_fp = argv[1];
        } else {
                printf("missing config file!\n");
        }

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* setup */
        config = load_config(config_fp);
        load_data(TEST_INPUT_DATA, config, INPUT_DATA);
        load_data(TEST_RESPONSE_DATA, config, RESPONSE_DATA);
        p = gp_population_initialize(init_tree_full, config->general.gp_tree);
        m = init_evolve_monitor(p->individuals->element_size, 5, NULL);

        /* run evolution */
        printf("RUNNING GP!\n");
        evolve(
                p,
                evaluate_program,
                evaluate_programs,
                tournament_selection,
                one_point_crossover,
                gp_point_mutation,
                config,
                m,
                0,
                NULL
        );

        if (p->solution != NULL) {
                printf("SOLUTION!!!");
                print_program(p->solution);
        }

        /* clean up */
        population_destroy(&p, gp_tree_destroy);

        return 0;
}
