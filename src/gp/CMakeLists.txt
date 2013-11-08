CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve_gp)

# evolve_gp library
ADD_LIBRARY(
   ${LIBRARY_NAME}

   initialize.c
   mutation.c
   crossover.c
   function_set.c
   terminal_set.c
   tree_parser.c
   tree_evaluator.c
   tree_validator.c
)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} m)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} al)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} dstruct)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} evolve)
