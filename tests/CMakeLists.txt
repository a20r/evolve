CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# evolve_tests
ADD_EXECUTABLE(evolve_tests evolve_tests.c)
TARGET_LINK_LIBRARIES(evolve_tests ${LIBRARY_NAME})


# evolve_monitor_tests
ADD_EXECUTABLE(
    evolve_monitor_tests

    evolve_monitor_tests.c
    test_utils.c
)
TARGET_LINK_LIBRARIES(evolve_monitor_tests ${LIBRARY_NAME})

# population_tests
ADD_EXECUTABLE(
    population_tests

    population_tests.c
    test_utils.c
)
TARGET_LINK_LIBRARIES(population_tests ${LIBRARY_NAME})

# selection_tests
ADD_EXECUTABLE(
    ga_selection_tests

    selection_tests.c
    test_utils.c
)
TARGET_LINK_LIBRARIES(ga_selection_tests ${LIBRARY_NAME})


SUBDIRS(config)
SUBDIRS(ga)
# SUBDIRS(gp)
