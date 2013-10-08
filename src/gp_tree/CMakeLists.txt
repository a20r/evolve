cmake_minimum_required(VERSION 2.6)

set(LIBRARY_NAME evolve_gp)

include_directories(../../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

# evolve genetic programming library
add_library(
    ${LIBRARY_NAME}

    population.c
    crossover.c
    mutation.c
)
target_link_libraries(evolve m)
target_link_libraries(evolve al)
target_link_libraries(evolve dstruct)
