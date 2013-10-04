CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# evolve library
ADD_LIBRARY(
    ${LIBRARY_NAME}

    population.c
    selection.c
    crossover.c
    mutation.c

    evolve.c
    evolve_monitor.c
)
TARGET_LINK_LIBRARIES(evolve m)
TARGET_LINK_LIBRARIES(evolve al)
TARGET_LINK_LIBRARIES(evolve dstruct)
