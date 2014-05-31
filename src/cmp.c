#include "stdio.h"
#include "math.h"

#include "cmp.h"


/* INTEGER COMPARATOR */
int int_cmp(const void *v1, const void *v2)
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

int int_cmp_asc(const void *v1, const void *v2)
{
    return int_cmp(v1, v2);
}

int int_cmp_desc(const void *v1, const void *v2)
{
    return int_cmp(v1, v2) * -1;
}


/* FLOAT COMPARATOR */
int float_cmp(const void *v1, const void *v2)
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

int float_cmp_asc(const void *v1, const void *v2)
{
    return float_cmp(v1, v2);
}

int float_cmp_desc(const void *v1, const void *v2)
{
    return float_cmp(v1, v2) * -1;
}
