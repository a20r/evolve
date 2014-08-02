include $(PWD)/config.mk
FOLDER_NAME = tests

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(basename $(SRCS))

default: intro $(EXES) done gp_tests

intro:
	@echo "ENTERING --> $(FOLDER_NAME)"

%.o: %.c
	$(MAKE_OBJ)

config_test: config_test.o
	$(MAKE_EXE)

data_test: data_test.o
	$(MAKE_EXE)

tree_test: tree_test.o
	$(MAKE_EXE)

population_test: population_test.o
	$(MAKE_EXE)

selection_test: selection_test.o
	$(MAKE_EXE)

crossover_test: crossover_test.o
	$(MAKE_EXE)

mutation_test: mutation_test.o
	$(MAKE_EXE)

utils_test: utils_test.o
	$(MAKE_EXE)

csv_test: csv_test.o
	$(MAKE_EXE)

regression_test: regression_test.o
	$(MAKE_EXE)

stack_test: stack_test.o
	$(MAKE_EXE)

evolve_test: evolve_test.o
	$(MAKE_EXE)

parallel_test: parallel_test.o
	# $(MAKE_EXE)

done:
	@echo "EXITING <-- $(FOLDER_NAME)"
	@echo ""

gp_tests:
	make -s -C gp
