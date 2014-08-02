include $(PWD)/config.mk
FOLDER_NAME = src/gp/tree

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: $(OBJS)

%.o: %.c
	$(MAKE_OBJ)
