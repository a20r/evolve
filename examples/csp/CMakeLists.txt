CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# closest string example
ADD_EXECUTABLE(
    csp

    csp.c
    ../example_utils.c
)
TARGET_LINK_LIBRARIES(csp ${LIB_EVOLVE})
TARGET_LINK_LIBRARIES(csp dstruct)
TARGET_LINK_LIBRARIES(csp m)
