#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <al/sort.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"
#include "selection.h"
#include "ga/initialize.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#include "example_utils.h"

#define MAXN 100
#define NUM_LBOUND 49
#define NUM_UBOUND 52

/* GLOBAL VARS */
int k = 5;  /* number of elements */
int len = 20;  /* length of each element */
char *strings[5];
float min_hd;
float *mutate_prob;
volatile sig_atomic_t stop_signal = 0;


static void signal_handler(int signnum)
{
        printf("SIGNAL %d!!\n", signnum);
        stop_signal = 1;
}

static char *randnumstr(int length)
{
        int i = 0;
        int num_index = 0;
        char num[10];
        char *r_num = malloc(sizeof(char *) * length);

        check_mem(r_num);
        memset(r_num, '\0', sizeof(char *) * length);

        /* create num array */
        for (i = NUM_LBOUND; i <= NUM_UBOUND; i++) {
                num[num_index] = i;
                num_index++;
        }

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                r_num[i] = num[randnum_i(0, num_index - 1)];
        }
        r_num[length] = '\0';  /* add null terminator for safety */

        return r_num;
error:
        if (r_num) free(r_num);
        return NULL;
}

static void mutate_numstr(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        char num[4] = { '1', '2', '3', '4' };
        char new_num;

        /* if new_num is same as previous, mutate again */
        while((new_num = num[randnum_i(0, 3)]) == (*str)[index]) {}

        (*str)[index] = new_num;
}

static float hamming_dist(char *s1, char *s2)
{
        int i;
        int len = strlen(s1);
        float dist = 0.0;

        for (i = 0; i < len; i++) {
                if (s1[i] != s2[i]) {
                        dist += 1;
                }
        }

        return dist;
}

static void insertion_sort_scores(
        float *arr,
        char *combinations[],
        int elements
)
{
        int j = 0;
        float value;
        char *combination;

        for (j = 1; j <= elements - 1; j++) {
                int i = j - 1;
                value = arr[j];
                combination = combinations[j];

                while (i >= 0 && arr[i] > value) {
                        arr[i + 1] = arr[i];
                        combinations[i + 1] = combinations[i];
                        i--;
                }

                arr[i + 1] = value;
                combinations[i + 1] = combination;
        }
}

static int binomial_coefficient(int n, int m)
{
        int i;
        int j;
        long bc[MAXN][MAXN];    /* table of binomial coefficient values */

        for (i = 0; i <= n; i++) {
                bc[i][0] = 1;
        }

        for (j = 0; j <= n; j++) {
                bc[j][j] = 1;
        }

        for (i = 1; i <= n; i++) {
                for (j = 1; j < i; j++) {
                        bc[i][j] = bc[i - 1][j - 1] + bc[i - 1][j];
                }
        }

        return bc[n][m];
}

static void setup_strings(int k, int len)
{
        int i;
        int j;
        int scores_len = binomial_coefficient(k, 2);
        float scores[scores_len];
        char *combinations[scores_len];
        char *cache;

        /* initialize closest string strings */
        for (i = 0; i < k; i++) {
                strings[i] = randnumstr(len);
                printf("s[%d]: %s\n", i, strings[i]);
        }

        /* find min edit dist between all strings */
        int index = 0;
        for (i = 0; i < k; i++) {
                for (j = i + 1; j < k; j++) {
                        /* calculate min edit distance */
                        scores[index] = hamming_dist(strings[i], strings[j]);

                        /* make combination string */
                        cache = calloc(1, (sizeof(char) * len * 2) + 2);
                        strcpy(cache, strings[i]);
                        strcat(cache, " ");
                        strcat(cache, strings[j]);

                        /* record combination string */
                        combinations[index] = cache;
                        index++;
                }
        }

        /* sort by score */
        insertion_sort_scores(scores, combinations, scores_len);
        for (i = 0; i < scores_len; i++) {
                printf("scores:[%f] string:[%s]\n", scores[i], combinations[i]);
        }

        /* find theoretical minimal hamming diameter */
        min_hd = roundf(fabs((scores[scores_len - 1]) / 2));
        printf("Theoretical Minimum Hamming Distance: %.2f\n", min_hd);

        /* clean up */
        for (i = 0; i < scores_len; i++) {
                free(combinations[i]);
        }
}

static void teardown_strings()
{
        int i;

        for (i = 0; i < k; i++) {
                free(strings[i]);
        }
}

float fitness_function(char *individual)
{
        int i;
        float scores[k];
        float max_score = len * k;

        /* evaulate closest string */
        for (i = 0; i < k; i++) {
                scores[i] = hamming_dist(individual, strings[i]);
        }
        insertion_sort(scores, sizeof(float), 0, k - 1, float_cmp_desc);

        return max_score - fabs(min_hd - scores[0]);
}

int main(int argc, char *argv[])
{
        int max_pop = 10;
        int max_gen = 1000;
        float *p_c = calloc(1, sizeof(float));
        float *p_m = calloc(1, sizeof(float));

        /* setup */
        *p_c = (argv[1] == NULL) ? 0.8 : atof(argv[1]);
        *p_m = (argv[1] == NULL) ? 0.1 : atof(argv[2]);
        if (argc != 2) {
                printf("missing arguments, using default settings!\n");
        }

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* setup */
        setup_strings(k, len);
        printf("Running!\n");
        signal(SIGINT, signal_handler);

        /* initialize evolution */
        struct population *p = population_create(
                len,  /* individual size */
                len * k,  /* goal */
                max_pop,  /* max_pop */
                max_gen  /* max_gen */
        );
        struct evolve_monitor *m = init_evolve_monitor(
                p->individuals->element_size,  /* individual size */
                5,
                /* "csp.dat" */
                NULL
        );

        /* run evolution */
        printf("RUNNING GA!\n");
        printf("Crossover Probability [%.2f]!\n", *p_c);
        printf("Mutation Probability [%.2f]!\n", *p_m);
        initialize_population(p, randnumstr);

        run_evolution(
                &p,
                fitness_function,

                /* selection */
                tournament_selection,
                DEFAULT_SELECT,

                /* crossover */
                one_pt_crossover,
                p_c, /* crossover probability */
                DEFAULT_PIVOT,

                /* mutation */
                mutate_numstr,
                p_m, /* mutation probability */

                m,
                0,
                &stop_signal
        );

        /* print results */
        print_evolve_results(p, fitness_function);
        /* print_top_individuals(m, 5); */

        /* clean up */
        destroy_evolve_monitor(&m);
        population_destroy(&p, free);
        teardown_strings();
        free(p_c);
        free(p_m);

        return 0;
}
