#ifndef __CMP__
#define __CMP__

#ifndef FLOAT_EPSILON
    #define FLOAT_EPSILON 0.00001
#endif

/* FUNCTIONS */
int intcmp(const void *v1, const void *v2);
int intcmp_asc(const void *v1, const void *v2);
int intcmp_desc(const void *v1, const void *v2);

int floatcmp(const void *v1, const void *v2);
int floatcmp_asc(const void *v1, const void *v2);
int floatcmp_desc(const void *v1, const void *v2);

#endif
