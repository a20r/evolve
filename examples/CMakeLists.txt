CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# hello world example
ADD_EXECUTABLE(hello_world hello_world.c)
TARGET_LINK_LIBRARIES(hello_world evolve)
TARGET_LINK_LIBRARIES(hello_world dstruct)
TARGET_LINK_LIBRARIES(hello_world m)

# closest string example
ADD_EXECUTABLE(csp csp.c)
TARGET_LINK_LIBRARIES(csp evolve)
TARGET_LINK_LIBRARIES(csp dstruct)
TARGET_LINK_LIBRARIES(csp m)
