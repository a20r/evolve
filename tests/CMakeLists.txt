cmake_minimum_required(VERSION 2.6)

include_directories(../include)
include_directories(/usr/local/include/)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

add_executable(evolve_tests evolve_tests.c)
target_link_libraries(evolve_tests evolve)

add_executable(evolve_utils_tests evolve_utils_tests.c)
target_link_libraries(evolve_utils_tests evolve)

add_executable(utils_tests utils_tests.c)
target_link_libraries(utils_tests evolve)

add_executable(selection_tests selection_tests.c)
target_link_libraries(selection_tests evolve)

add_executable(crossover_tests crossover_tests.c)
target_link_libraries(crossover_tests evolve)
