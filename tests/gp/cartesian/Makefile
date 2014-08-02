include $(PWD)/config.mk
FOLDER_NAME = tests/gp/cartesian

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TESTS = $(basename $(SRCS))

default: $(TESTS)

%: %.c
	$(MAKE_EXE)
