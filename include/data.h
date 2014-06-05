#ifndef __DATA__
#define __DATA__

struct data {
    int rows;
    int cols;
    char **fields;
    float ***data;
};

/* FUNCTIONS */
struct data *data_new(int rows, int cols, const char **field_names);
int data_destroy(struct data *d);
int data_field_index(struct data *d, const char *field);

#endif
