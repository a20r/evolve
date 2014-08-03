# regression.h

**Contents**:
- Structures
- Functions


## Structures

    struct regression_stack
    {
        struct node **stack;
        int end;
    };



## Functions

    int regression_func_input(
        struct node *n,
        struct data *d,
        float **func_input,
        int nth_arity
    )

    void regression_free_inputs(
        int in1_type,
        struct node *in1,
        int in2_type,
        struct node *in2
    )

Destroys node `in1` and `in2`, of node type `in1_type` and `in2_type`.


---

    int regression_check(void)

Checks `errno` to see if an error occured, if an error occured the function
returns `-1` else `0`.


---

    int regression_traverse(
        int index,
        int end,
        struct node **chromosome,
        struct stack *stack,
        float **func_input,
        struct data *d
    )

Recusively evaluate tree nodes in depth first order. Where:

    - `index` denotes the depth first order index
    - `end` is the last index of the tree
    - `chromosome` is the tree chromosome to be evaluated
    - `stack` is the regression stack used for performing the evaluation


---

    void regression_clear_stack(struct stack *s)

Destroys regression stack `s`.


---

    int regression_evaluate(
        struct tree *t,
        float **func_input,
        struct data *d,
        char *resp
    )

Evaluate tree `t` with a malloced 2d float array `func_input`, training data
`d` and the response variable `resp` for evaluation.

**Example**:

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* evaluate population */
    struct tree *t = p->individuals[0];
    regression_evaluate(t, func_input, d, (char *) "y");


---

    int regression_evaluate_population(struct population *p, struct data *d)

Evaluate population `p` of trees with data `d`. Returns `0`.


---

    void regression_print_traverse(struct node *n)

Recursively print tree from node `n` as mathematical equation.


---

    void regression_print(void *target)

Prints `target` as mathematical equation.
