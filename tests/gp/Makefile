include $(PWD)/config.mk
FOLDER_NAME = tests/gp

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(basename $(SRCS))

default: intro $(EXES) done

intro:
	@echo "ENTERING --> $(FOLDER_NAME)"

%.o: %.c
	$(COMPILE_OBJ)

tree_test: tree_test.o
	$(MAKE_EXE)

crossover_test: crossover_test.o
	$(MAKE_EXE)

mutation_test: mutation_test.o
	$(MAKE_EXE)

regression_test: regression_test.o
	$(MAKE_EXE)

done:
	@echo "EXITING <-- $(FOLDER_NAME)"
	@echo ""
