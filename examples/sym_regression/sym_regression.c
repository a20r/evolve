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
#include "gp/crossover.h"
#include "gp/mutation.h"


static float fitness_function(char *individual)
{
        char *target = "";
        float max_score = 122 * strlen(individual);
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - individual[i]));
        }

        return max_score - total;
}

int main(int argc, char *argv[])
{
        struct evolve_config *config;
        struct gp_tree_config *gp_config;
        struct population *p;
        struct gp_tree *gp;
        char *config_fp;
        float *p_c = calloc(1, sizeof(float));
        float *p_m = calloc(1, sizeof(float));

        /* parse arguments */
        if (argc == 1) {
                config_fp = argv[1];
        } else {
                printf("missing config file!\n");
        }

        /* setup */
        config = load_config(config_fp);

        /* run evolution */
        printf("RUNNING GP!\n");
        p = population_initialize(init_tree_full, gp_config);
        /* run_evolution( */
        /*         &p, */
        /*         fitness_function, */

        /*         #<{(| selection |)}># */
        /*         tournament_selection, */
        /*         DEFAULT_SELECT, */

        /*         #<{(| crossover |)}># */
        /*         one_point_crossover, */
        /*         p_c, #<{(| crossover probability |)}># */
        /*         DEFAULT_PIVOT, */

        /*         #<{(| mutation |)}># */
        /*         point_mutation, */
        /*         p_m , #<{(| mutation probability |)}># */

        /*         m, */
        /*         0, */
        /*         &stop_signal */

        /* ); */

        population_destroy(&p, gp_tree_destroy);

        return 0;
}
