# Crossover
In genetic algorithms, crossover is a genetic operator used to vary the
programming of a chromosome or chromosomes from one generation to the next. It
is analogous to reproduction and biological crossover, upon which genetic
algorithms are based. Cross over is a process of taking more than one parent
solutions and producing a child solution from them.



## Functions

    int one_pt_crossover(void **chromo_1, void **chromo_2, int index)

A single point is selected on both chromosomes, the chromosomes are then
crossed from the point to form two pairs of chromosomes. All chromosomes beyond
that point are swapped between the two adjacent chromosomes. The result is
two pairs of mixed chromosomes from a single point.

Parameters:

    void **chromo_1
        Pointer to pointer pointing at chromosome 1

    void **chromo_2
        Pointer to pointer pointing at chromosome 1

    int index
        Crossover pivot index

Returns:

    0 for success, -1 on failure

------------------------------------------------------------------------------

    int two_pt_crossover(void **chromo_1, void **chromo_2, int index)

Two points are selected on both chromosomes, the chromosomes are then crossed
from the point to form two pairs of chromosomes. All chromosomes beyond both
points are swapped between the two adjacent chromosomes. Essentially similar to
a one point crossover but repeated twice. The result is two pairs of mixed
chromosomes from two points.

Parameters:

    void **chromo_1
        Pointer to pointer pointing at chromosome 1

    void **chromo_2
        Pointer to pointer pointing at chromosome 1

    int index
        Crossover pivot index

Returns:

    0 for success, -1 on failure


------------------------------------------------------------------------------

    void crossover(
            void **child_1,
            void **child_2,
            int index,
            int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
            float crossover_prob
    )

Main crossover wrapper for different crossover methods.

Parameters:

    void **child_1
        Child 1 to be crossed over

    void **child_2
        Child 2 to be crossed over

    int index
        Crossover pivot index

    int (*crossover_func)(void **chromo_1, void **chromo_2, int index)
        Crossover function

    float crossover_prob
        Crossover probability

------------------------------------------------------------------------------
