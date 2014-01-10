CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(/usr/local/lib)

# evolve library
ADD_LIBRARY(
    ${LIBRARY_NAME}

    # evolve.c
    population.c
    # selection.c
)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} config)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} evolve_ga)

SUBDIRS(config)
SUBDIRS(ga)
