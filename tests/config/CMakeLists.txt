CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# config_tests
ADD_EXECUTABLE(config_tests config_tests.c)
TARGET_LINK_LIBRARIES(config_tests ${LIBRARY_NAME})
TARGET_LINK_LIBRARIES(config_tests jansson)
