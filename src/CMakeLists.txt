cmake_minimum_required(VERSION 2.6)

include_directories(../include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)

add_library(evolve evolve.c crossover.c mutate.c)
target_link_libraries(evolve dstruct)

subdirs(examples)
