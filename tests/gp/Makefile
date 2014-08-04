include $(PWD)/config.mk
FOLDER_NAME = tests/gp

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TESTS = $(basename $(SRCS))

default: subdirs $(TESTS)

subdirs:
	@make -s -C tree
	@make -s -C cartesian

%: %.c
	$(MAKE_TEST)
