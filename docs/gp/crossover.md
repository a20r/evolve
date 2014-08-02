# `crossover.h`

# Functions
- int *random_indicies_new(struct tree *t1, struct tree *t2)
- void random_indicies_destory(int **indicies)

`random_indicies_new()` and `random_indicies_destroy()` creates an integer
array and destroys them. The random indices created are co-ordinates for
crossover.


- int point_crossover(void *t1, void *t2)

`point_crossover()` performs point crossover with `t1` and `t2`, always
returns 0.
