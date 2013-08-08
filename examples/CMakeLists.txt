cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -DNDEBUG")

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

add_executable(hello_world hello_world.c)
target_link_libraries(hello_world evolve)
target_link_libraries(hello_world dstruct)
target_link_libraries(hello_world m)
