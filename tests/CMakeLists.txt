CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g -fprofile-arcs -ftest-coversage")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# evolve
ADD_EXECUTABLE(evolve_tests evolve_tests.c)
TARGET_LINK_LIBRARIES(evolve_tests ${LIBRARY_NAME})

# evolve_monitor
ADD_EXECUTABLE(
    evolve_monitor_tests

    test_utils.c
    evolve_monitor_tests.c
)
TARGET_LINK_LIBRARIES(evolve_monitor_tests ${LIBRARY_NAME})

# population
ADD_EXECUTABLE(
    population_tests

    test_utils.c
    population_tests.c
)
TARGET_LINK_LIBRARIES(population_tests ${LIBRARY_NAME})

# selection
ADD_EXECUTABLE(selection_tests selection_tests.c)
TARGET_LINK_LIBRARIES(selection_tests ${LIBRARY_NAME})

# crossover
ADD_EXECUTABLE(crossover_tests crossover_tests.c)
TARGET_LINK_LIBRARIES(crossover_tests ${LIBRARY_NAME})

# mutation
ADD_EXECUTABLE(mutation_tests mutation_tests.c)
TARGET_LINK_LIBRARIES(mutation_tests ${LIBRARY_NAME})
