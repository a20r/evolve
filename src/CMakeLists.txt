cmake_minimum_required(VERSION 2.6)

set(LIB_NAME evolve)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -DNDEBUG")

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

# evolve library
add_library(
    ${LIB_NAME}

    population.c
    selection.c
    crossover.c
    mutation.c
    utils.c

    evolve.c
    evolve_monitor.c
)
target_link_libraries(evolve m)
target_link_libraries(evolve al)
target_link_libraries(evolve dstruct)
