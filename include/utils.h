#ifndef __UTILS__
#define __UTILS__

#define INTEGER 0
#define FLOAT 1
#define DOUBLE 2
#define STRING 3

#define free_mem(TARGET, FREE_FUNC) \
    if (TARGET) { \
        FREE_FUNC((void *) TARGET); \
    }

#define free_mem_arr(TARGET, NELEM, FREE_FUNC) \
    int z; \
    if (TARGET) { \
        for (z = 0; z < NELEM; z++) { \
            FREE_FUNC((void *) TARGET[z]); \
        } \
        FREE_FUNC((void *) TARGET); \
    }

#define free_mem_2darr(TARGET, ROWS, COLS, FREE_FUNC) \
    int a; \
    int b; \
    if (TARGET) { \
        for (a = 0; a < COLS; a++) { \
            for (b = 0; b < ROWS; b++) { \
                FREE_FUNC((void *) TARGET[a][b]); \
            } \
            FREE_FUNC((void *) TARGET[a]); \
        } \
        FREE_FUNC((void *) TARGET); \
    }


/* FUNCTIONS */
void *copy_value(int value_type, void *value);
int *malloc_int(int i);
float *malloc_float(float f);
double *malloc_double(double d);
char *malloc_string(const char *s);
int cmp_values(int value_type, void *v1, void *v2);
int trim_char(const char c);
char *trim(const char *s);

#endif
