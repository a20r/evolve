include $(PWD)/config.mk
FOLDER_NAME = tests

SRCS = $(wildcard *.c)
SRCS := $(filter-out parallel_test.c, $(SRCS))
TESTS = $(SRCS:.c=)

default: $(TESTS) gp_tests

%: %.c
	$(MAKE_TEST)

gp_tests:
	make -s -C gp
