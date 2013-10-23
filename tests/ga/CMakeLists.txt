CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# initialize
ADD_EXECUTABLE(ga_initialize_tests initialize_tests.c)
TARGET_LINK_LIBRARIES(ga_initialize_tests ${LIBRARY_NAME})

# crossover
ADD_EXECUTABLE(ga_crossover_tests crossover_tests.c)
TARGET_LINK_LIBRARIES(ga_crossover_tests ${LIBRARY_NAME})

# mutation
ADD_EXECUTABLE(ga_mutation_tests mutation_tests.c)
TARGET_LINK_LIBRARIES(ga_mutation_tests ${LIBRARY_NAME})
