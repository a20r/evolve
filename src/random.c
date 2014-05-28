#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "dbg.h"
#include "random.h"


int randi(int min, int max)
{
    int divisor = RAND_MAX / (max + 1);
    int retval = rand() / divisor;

    check(max > min, "Lower bound is bigger than upper bound!");
    while (retval < min || retval > max) {
        retval = rand() / divisor;
        printf("-->%d\n", retval);
    }

    return retval;
error:
    return min + 1;
}

float randf(float min, float max)
{
    float divisor = (float) RAND_MAX / (float) (max + 1.0);
    float retval = rand() / divisor;

    check(max > min, "Lower bound is bigger than upper bound!");
    while (retval < min || retval > max) {
        retval = rand() / divisor;
    }

    return retval;
error:
    return min + 1;
}
