#ifndef __CMP__
#define __CMP__

#ifndef FLOAT_EPSILON
    #define FLOAT_EPSILON 0.00001
#endif

/* FUNCTIONS */
int int_cmp(const void *v1, const void *v2);
int int_cmp_asc(const void *v1, const void *v2);
int int_cmp_desc(const void *v1, const void *v2);

int float_cmp(const void *v1, const void *v2);
int float_cmp_asc(const void *v1, const void *v2);
int float_cmp_desc(const void *v1, const void *v2);

#endif
