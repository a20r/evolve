#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include "evolve_monitor.h"

int assert_sorted_array(
        void *array,
        int len,
        size_t el_sz,
        int(*cmp)(const void *,const void *)
);

int assert_sorted_gstats(
        struct evolve_monitor *m,
        int(*cmp)(const void *,const void *)
);
#endif
