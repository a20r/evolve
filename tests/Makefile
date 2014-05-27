include $(PWD)/config.mk
FOLDER_NAME = tests

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXES = $(basename $(SRCS))

default: $(EXES)

%.o: %.c
	$(COMPILE_OBJ)

tree_test: tree_test.o
	$(MAKE_EXE)
