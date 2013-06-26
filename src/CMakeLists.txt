cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_COMPILER gcc)

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

# evolve library
add_library(
    evolve

    evolve.c
    selection.c
    crossover.c
    mutate.c
    utils.c
)
target_link_libraries(evolve dstruct)
target_link_libraries(evolve al)

subdirs(examples)
