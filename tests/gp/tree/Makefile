include $(PWD)/config.mk
FOLDER_NAME = tests/gp/tree

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TESTS = $(basename $(SRCS))

default: $(TESTS)

%: %.c
	$(MAKE_TEST)
