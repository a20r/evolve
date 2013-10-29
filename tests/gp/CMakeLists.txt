CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# gp_initialize_tests
ADD_EXECUTABLE(gp_initialize_tests intialize_tests.c)
TARGET_LINK_LIBRARIES(gp_initialize_tests ${LIBRARY_NAME})
