#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <munit/munit.h>
#include <dstruct/darray.h>

#include "test_utils.h"
#include "population.h"


int assert_sorted_array(
        void *array,
        int len,
        size_t el_sz,
	int(*cmp)(const void *,const void *)
)
{
        int i = 0;
        int res = 0;

        for (i = 1; i < len; i++) {
                res = cmp(
                        (array + (el_sz * (i - 1))),
                        (array + (el_sz * i))
                );

                if (res > 0) {
                        return -1;
                }
        }

        return 0;
}

int assert_sorted_population(
        struct population *p,
	int(*cmp)(const void *,const void *)
)
{
        int i = 0;
        int end = p->chromosomes->end;
        int res = 0;

        for (i = 1; i <= end; i++) {
                res = cmp(
                        darray_get(p->chromosome_scores, i - 1),
                        darray_get(p->chromosome_scores, i)
                );

                if (res > 0) {
                        return -1;
                }
        }

        return 0;
}

int assert_sorted_gstats(
        struct evolve_monitor *m,
	int(*cmp)(const void *,const void *)
)
{
        int i = 0;
        int end = m->best_scores->end;
        int res = 0;

        for (i = 1; i <= end; i++) {
                res = cmp(
                        darray_get(m->best_scores, i - 1),
                        darray_get(m->best_scores, i)
                );

                if (res > 0) {
                        return -1;
                }
        }

        return 0;
}
