# data.h
This module is mainly concerned with the training data that Evolutionary
Algorithm will use. It provides data structures and functions to create,
parse and contain training data.

**Contents**:

- Structures
- Functions


## Structures

    struct data {
        int rows;
        int cols;
        char **fields;
        float ***data;
    };



## Functions

    struct data *data_new(int rows, int cols, const char **field_names);
    void data_destroy(void *d);

`data_new()` and `data_destroy` creates and destroys `struct data`. Where
`rows`, `cols` and `field_names` are rows, columns and a string of field names
repsectively.

---

    int data_field_index(struct data *d, const char *field);

`data_field_index()` determins what index the particular field has, and returns
the index as an integer. It requires a `d` pointer to `struct data` which
contains data and its info, and `field` pointer to string that corresponds to
the field where you want the index for.


---

    int csv_num_rows(FILE *f, int header);
    int csv_num_cols(FILE *f, const char *token);

`csv_num_cols()` and `csv_num_rows()` parses the csv file `f` to determine the
number of rows and columns. Where `header` is a boolean to denote whether a
header field exists in the csv file. and `token` denotes the character
delimiter used to separate values (e.g. ',' for comma separated values).


---

    const char **csv_parse_header(FILE *f, int cols, const char *token);

`csv_parse_header()` parses the header of file `f`, with number of columns
`cols`, number of rows `rows` and delimiter character `*token` (e.g. ',' for
comma separated values). The function returns a array of `char` pointers,
where each element is the value of each field header.


---

    struct data *csv_info(FILE *f, int header, const char *token);

`csv_info()` aggregates the number of rows, columns and the value of fields the
csv file `f` has, and records it into a `struct data` and returns a pointer of
it. The function aside from the csv file `f`, requires a boolean value `header`
to denote if a header exists in `f`, and also `token` to denote the character
used to separate values in file `f`.


---

    float **csv_parse_data_row(char *line, int cols, const char *token);

`csv_parse_data_row()` parses a row of data, where `line` is the string of data
to be parsed, `cols` is the number of colums to expect, and `token` is the
delimter that separates between values (e.g. ',' for comma separated values).
Once it finishes parsing it returns an array of `float` pointers containing the
data.


---

    struct data *csv_load_data(const char *fp, int header, const char *token);

`csv_load_data()` loads the file in path `fp` and records it into a `struct
data` and returns a pointer of it. Aside from the file path `fp`, the function
also requires a boolean value `header` to denote whether the data file in `fp`
contains a header line, and also `token` a delimter that separates between data
values (e.g. ',' for comma separated values).
