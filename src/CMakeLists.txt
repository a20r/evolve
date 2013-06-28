cmake_minimum_required(VERSION 2.6)

set(LIB_NAME evolve)
set(CMAKE_C_COMPILER gcc)

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

# evolve library
add_library(
    ${LIB_NAME}

    evolve.c
    selection.c
    crossover.c
    mutate.c
    utils.c
)
target_link_libraries(evolve m)
target_link_libraries(evolve dstruct)
target_link_libraries(evolve al)
