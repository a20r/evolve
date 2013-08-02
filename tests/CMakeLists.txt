cmake_minimum_required(VERSION 2.6)

set(LIBRARY_NAME evolve)

include_directories(../include)
include_directories(/usr/local/include/)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

# evolve
add_executable(evolve_tests evolve_tests.c)
target_link_libraries(evolve_tests ${LIBRARY_NAME})

# evolve_utils
add_executable(
    evolve_utils_tests

    test_utils.c
    evolve_utils_tests.c
)
target_link_libraries(evolve_utils_tests ${LIBRARY_NAME})

# evolve_monitor
add_executable(
    evolve_monitor_tests

    test_utils.c
    evolve_monitor_tests.c
)
target_link_libraries(evolve_monitor_tests ${LIBRARY_NAME})

# utils
add_executable(utils_tests utils_tests.c)
target_link_libraries(utils_tests ${LIBRARY_NAME})

# selection
add_executable(selection_tests selection_tests.c)
target_link_libraries(selection_tests ${LIBRARY_NAME})

# crossover
add_executable(crossover_tests crossover_tests.c)
target_link_libraries(crossover_tests ${LIBRARY_NAME})
