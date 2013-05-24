cmake_minimum_required(VERSION 2.6)

include_directories(../include)
include_directories(/usr/local/include/)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

add_executable(darray_tests darray_tests.c)
