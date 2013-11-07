CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# gp_initialize_tests
ADD_EXECUTABLE(gp_initialize_tests intialize_tests.c)
TARGET_LINK_LIBRARIES(gp_initialize_tests ${LIBRARY_NAME})

# gp_tree_parser_tests
ADD_EXECUTABLE(gp_tree_parser_tests tree_parser_tests.c)
TARGET_LINK_LIBRARIES(gp_tree_parser_tests ${LIBRARY_NAME})

# gp_tree_evaluator_tests
ADD_EXECUTABLE(gp_tree_evaluator_tests tree_evaluator_tests.c)
TARGET_LINK_LIBRARIES(gp_tree_evaluator_tests ${LIBRARY_NAME})
