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

#define CONFIG_FILE "examples/sym_regression/config.json"
#define EXAMPLE_INPUT_DATA "examples/sym_regression/data/sine_input.dat"
#define EXAMPLE_RESPONSE_DATA "examples/sym_regression/data/sine_response.dat"


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
        int i = 0;
        struct evolve_config *config = NULL;
        struct evolve_monitor *m = NULL;
        struct population *p = NULL;
        struct gp_tree *tree = NULL;

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* setup */
        config = load_config(CONFIG_FILE);
        load_data(EXAMPLE_INPUT_DATA, config, INPUT_DATA);
        load_data(EXAMPLE_RESPONSE_DATA, config, RESPONSE_DATA);
        p = gp_population_initialize(init_tree_full, config->general.gp_tree);
        /* m = init_evolve_monitor(p->individuals->element_size, 5, NULL); */


        /* #<{(| run evolution |)}># */
        /* printf("RUNNING GP!\n"); */
        /* evolve( */
        /*         p, */
        /*         evaluate_program, */
        /*         evaluate_programs, */
        /*         tournament_selection, */
        /*         one_point_crossover, */
        /*         gp_point_mutation, */
        /*         config, */
        /*         m, */
        /*         0, */
        /*         NULL */
        /* ); */

        /* if (p->solution != NULL) { */
        /*         printf("SOLUTION!!!"); */
        /*         print_program(p->solution); */
        /* } */

        /* clean up */
        for (i = 0; i < p->max_population; i++) {
                tree = darray_get(p->individuals, i);
                printf("- TREE[%d]", i);
                printf(" [size: %d]", tree->size);
                printf("[depth: %d]\n", tree->depth);
                print_gp_tree(tree->root);
                printf("\n\n");
        }
        population_destroy(&p, gp_tree_destroy);
        /* destroy_evolve_monitor(&m); */
        config_destroy(config);

        return 0;
}
