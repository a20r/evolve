#ifndef __UTILS__
#define __UTILS__

#define INTEGER 0
#define FLOAT 1
#define DOUBLE 2
#define STRING 3

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
