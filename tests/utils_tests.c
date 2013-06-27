#include "time.h"
#include "stdlib.h"
#include "string.h"

#include <munit/munit.h>
#include <dbg/dbg.h>

#include "utils.h"


int test_randnum_i()
{
        int i = 0;
        int curr_num = 0;  /* current number */
        int last_num = 0;  /* last number */

        for (i = 0; i < 10; i++) {
                curr_num = randnum_i(0, 100);

                debug("Run: %d", i);
                debug("last_num: [%d]", last_num);
                debug("curr_num: [%d]", curr_num);
                debug("");

                mu_assert(curr_num <= 100, "curr_num should not be over 100!");
                mu_assert(curr_num >= 0, "curr_num should be over 0!");
                mu_assert(curr_num != last_num, "curr_num == last_num!");
                last_num = curr_num;
        }

        return 0;
}

int test_randstr()
{
        int i = 0;
        int len = 10;
        char *r_str = '\0';
        char curr_str[len];
        char last_str[len];

        memset(curr_str, 0, sizeof(curr_str));
        memset(last_str, 0, sizeof(last_str));

        for (i = 0; i < 10; i++) {
                r_str = randstr(len);
                strcpy(curr_str, r_str);

                debug("Run: %d", i);
                debug("random str: [%s]", curr_str);
                debug("");

                mu_assert(
                        strlen(curr_str) == 10,
                        "Random string should be length 10!"
                );
                mu_assert(
                        curr_str[len] == '\0',
                        "Random string should be length 10!"
                );
                mu_assert(
                        strcmp(last_str, curr_str) != 0,
                        "curr_str != last_str failed"
                );

                /* clean up */
                strncpy(last_str, curr_str, sizeof(curr_str));
                memset(curr_str, 0, sizeof(char) * len);
                free(r_str);
        }

        return 0;
}

void test_suite()
{
        mu_run_test(test_randnum_i);
        mu_run_test(test_randstr);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
