CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# selection
ADD_EXECUTABLE(ga_selection_tests selection_tests.c)
TARGET_LINK_LIBRARIES(ga_selection_tests ${LIBRARY_NAME})

# crossover
ADD_EXECUTABLE(ga_crossover_tests crossover_tests.c)
TARGET_LINK_LIBRARIES(ga_crossover_tests ${LIBRARY_NAME})

# mutation
ADD_EXECUTABLE(ga_mutation_tests mutation_tests.c)
TARGET_LINK_LIBRARIES(ga_mutation_tests ${LIBRARY_NAME})
