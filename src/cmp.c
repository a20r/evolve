#include "stdio.h"
#include "math.h"

#include "cmp.h"


/* INTEGER COMPARATOR */
int intcmp(const void *v1, const void *v2)
{
    /* null-check */
    if (v1 == NULL || v2 == NULL) {
        if (v1 == NULL) {
            return -1;
        } else if (v1 == NULL) {
            return 1;
        } else {
            return 0;
        }
    }

    if (*(int *) v1 > *(int *) v2) {
        return 1;
    } else if (*(int *) v1 < *(int *) v2) {
        return -1;
    } else {
        return 0;
    }
}

int intcmp_asc(const void *v1, const void *v2)
{
    return intcmp(v1, v2);
}

int intcmp_desc(const void *v1, const void *v2)
{
    return intcmp(v1, v2) * -1;
}


/* FLOAT COMPARATOR */
int floatcmp(const void *v1, const void *v2)
{
    /* null-check */
    if (v1 == NULL || v2 == NULL) {
        if (v1 == NULL) {
            return -1;
        } else if (v1 == NULL) {
            return 1;
        } else {
            return 0;
        }
    }

    /* compare floats */
    if (fabs(*(float *) v1 - *(float *) v2) <= FLOAT_EPSILON) {
        return 0;
    } else if (*(float *) v1 > *(float *) v2) {
        return 1;
    } else if (*(float *) v1 < *(float *) v2) {
        return -1;
    } else {
        printf("Error! Undefined runtime behaviour!\n");
        return -1;
    }
}

int floatcmp_asc(const void *v1, const void *v2)
{
    return floatcmp(v1, v2);
}

int floatcmp_desc(const void *v1, const void *v2)
{
    return floatcmp(v1, v2) * -1;
}
