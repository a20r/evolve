CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# hello world example
ADD_EXECUTABLE(
    hello_world

    hello_world.c
    ../example_utils.c
)
TARGET_LINK_LIBRARIES(hello_world ${LIB_EVOLVE})
TARGET_LINK_LIBRARIES(hello_world dstruct)
TARGET_LINK_LIBRARIES(hello_world m)
