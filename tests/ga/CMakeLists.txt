CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# crossover
ADD_EXECUTABLE(ga_crossover_tests crossover_tests.c)
TARGET_LINK_LIBRARIES(ga_crossover_tests ${LIBRARY_NAME})

# mutation
ADD_EXECUTABLE(ga_mutation_tests mutation_tests.c)
TARGET_LINK_LIBRARIES(ga_mutation_tests ${LIBRARY_NAME})
