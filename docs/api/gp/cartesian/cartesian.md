# cartesian.h

**Contents**:
- Structures
- Functions


## Structures

    struct cartesian_config
    {
        /* graph details */
        int rows;
        int columns;
        int levels_back;
        int max_arity;

        int num_inputs;
        int num_outputs;

        /* data details */
        int *functions;
    };

    struct cartesian
    {
        float *score;

        /* graph details */
        int rows;
        int columns;

        int levels_back;
        int max_arity;

        int num_inputs;
        int num_outputs;

        /* graph elements */
        int *graph;
        int *inputs;
        int *outputs;

        int **address_grid;
    };


## Functions

    struct cartesian_config *cartesian_config_new(void);
    void cartesian_config_destroy(void *cc);

`cartesian_config_new()` creates and returns pointer to a `struct
cartesian_config`, and `cartesian_config_destroy()` destroys it. While the
former does not require input arguments, it is expected that the user will
assign appropriate values later. The `struct cartesian_config` created will
have the following defaults:

    cc->rows = 0;
    cc->columns = 0;
    cc->levels_back = 0;
    cc->max_arity = 0;

    cc->num_inputs = 0;
    cc->num_outputs = 0;


---

    struct cartesian *cartesian_new(struct cartesian_config *cc);
    void cartesian_destroy(void *c);


`cartesian_new()` creates and returns pointer to a `struct cartesian`, and
`cartesian_destroy()` destroys it. The `struct cartesian` created will have the
following defaults:

    c->score = NULL;

    /* graph details */
    c->rows = cc->rows;
    c->columns = cc->columns;
    c->levels_back = cc->levels_back;
    c->max_arity = cc->max_arity;

    c->num_inputs = cc->num_inputs;
    c->num_outputs = cc->num_outputs;

    /* graph elements */
    c->graph = NULL;
    c->inputs = NULL;
    c->outputs = NULL;
    c->address_grid = NULL;

**Note**: The graph elements of the cartesian will have to be assigned by the
user at the later stage.


---

    void cartesian_print(struct cartesian *c);

Prints the string representation of cartesian `c`.

---

    char *cartesian_string(struct cartesian *c);

Returns the string representation of cartesian `c`.


---

    int **caretsian_address_grid(struct cartesian *c);
