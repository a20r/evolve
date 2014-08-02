include $(PWD)/config.mk
FOLDER_NAME = src/gp/cartesian

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: $(OBJS)

%.o: %.c
	$(MAKE_OBJ)
