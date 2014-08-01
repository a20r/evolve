# `selection.h`


# Functions
- struct population *tournament_selection(
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
