CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# sym_regression example
ADD_EXECUTABLE(
    sym_regression

    sym_regression.c
    ../example_utils.c
)
TARGET_LINK_LIBRARIES(sym_regression ${LIB_EVOLVE})
TARGET_LINK_LIBRARIES(sym_regression dstruct)
TARGET_LINK_LIBRARIES(sym_regression m)
