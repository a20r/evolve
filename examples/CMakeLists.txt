CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# hello world example
ADD_EXECUTABLE(
    hello_world

    example_utils.c
    hello_world.c
)
TARGET_LINK_LIBRARIES(hello_world ${LIBRARY_NAME})
TARGET_LINK_LIBRARIES(hello_world dstruct)
TARGET_LINK_LIBRARIES(hello_world m)

# closest string example
ADD_EXECUTABLE(
    csp

    example_utils.c
    csp.c
)
TARGET_LINK_LIBRARIES(csp ${LIBRARY_NAME})
TARGET_LINK_LIBRARIES(csp dstruct)
TARGET_LINK_LIBRARIES(csp m)
