# selection.h

**Contents**:
- Structures
- Functions


## Structures

    struct selection_config
    {
        int method;
        struct population *(*select_func)(
            struct population *,
            struct config *
        );

        int tournament_size;
    };


## Functions

    struct selection_config *selection_config_new(int method);
    void selection_config_destroy(void *config);

`selection_config_new()` creates and returns pointer to a `struct
selection_config`, while `selection_config_destroy()` destroys it.

`selection_config_new()` accepts an integer argument detailing which selection
method to use.


---

    struct population *tournament_selection(
        struct population *p,
        struct config *c
    )

`tournament_selection()` performs tournament selection on population `p` with
additional configuration in `c`. The function expects configuraiton `c` to
contain:

- `c->cmp`
- `c->copy`
- `c->get_score`
- `c->selection->tournament_size`

See docs for `config.h` for more info.
