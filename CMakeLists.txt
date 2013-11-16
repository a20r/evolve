CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(evolve)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_BUILD_TYPE DEBUG)
SET(CMAKE_C_FLAGS_DEBUG "-Wall -g")
# SET(CMAKE_C_FLAGS_RELEASE "-Wall -DNDEBUG")
# SET(CLANG_ASAN "-fsanitize=address -fno-omit-frame-pointer")
# SET(CLANG_MSAN "-fsanitize=memory -fsanitize-memory-track-origins")
# SET(CMAKE_C_FLAGS_DEBUG "${CLANG_ASAN} -g")
# SET(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
# SET(CMAKE_SHARED_LINKER_FLAGS "-fsanitize=address")
# SET(CMAKE_STATIC_LINKER_FLAGS "-fsanitize=address")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# uninstall target
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)
ADD_CUSTOM_TARGET(
    uninstall
    COMMAND
    ${CMAKE_COMMAND} -P
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

SUBDIRS(src)
SUBDIRS(tests)
SUBDIRS(examples)
