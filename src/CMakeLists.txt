CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME evolve)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(/usr/local/include/)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib)

# evolve library
ADD_LIBRARY(
    ${LIBRARY_NAME}

    evolve.c
    evolve_monitor.c
    population.c
    selection.c
)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} evolve_ga)

SUBDIRS(ga)
# SUBDIRS(gp)
