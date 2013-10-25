CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(LIBRARY_NAME config)

# config_library
ADD_LIBRARY(
    ${LIBRARY_NAME}

    config.c
    parse.c
)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} jansson)
