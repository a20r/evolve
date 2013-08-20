# Crossover
In genetic algorithms, crossover is a genetic operator used to vary the
programming of a chromosome or chromosomes from one generation to the next. It
is analogous to reproduction and biological crossover, upon which genetic
algorithms are based. Cross over is a process of taking more than one parent
solutions and producing a child solution from them.


    int one_pt_crossover(void **chromo_1, void **chromo_2, int index)

A one point single crossover on both chromosomes is selected with
`index`.  All data beyond that point in either chromosome is swapped
between the two.  The result is crossed overd chromosomes.

    Parameters:
        void **chromo_1
            Pointer to pointer pointing at chromosome 1

        void **chromo_2
            Pointer to pointer pointing at chromosome 1

        int index
            Crossover pivot index

    Returns:
        0 for success, -1 for failure


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
            Pointer to pointer pointing at child_1

        void **child_2
            Pointer to pointer pointing at child_2

        int index
            Crossover pivot index

        int (*crossover_func)(void **chromo_1, void **chromo_2, int index)
            Pointer to crossover function

        float crossover_prob
            Crossover probability
