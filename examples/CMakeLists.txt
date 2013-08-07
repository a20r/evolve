cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_COMPILER gcc)

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

add_executable(hello_world hello_world.c)
target_link_libraries(hello_world evolve)
target_link_libraries(hello_world dstruct)
target_link_libraries(hello_world m)
