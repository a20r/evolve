include $(PWD)/config.mk
FOLDER_NAME = tests

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(basename $(SRCS))

default: $(EXES)

%.o: %.c
	$(COMPILE_OBJ)

data_test: data_test.o
	$(MAKE_EXE)

tree_test: tree_test.o
	$(MAKE_EXE)

population_test: population_test.o
	$(MAKE_EXE)

crossover_test: crossover_test.o
	$(MAKE_EXE)

mutation_test: mutation_test.o
	$(MAKE_EXE)
