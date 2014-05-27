include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: evolve

%.o: %.c
	$(COMPILE_OBJ)

evolve: $(OBJS)
	$(MAKE_STATIC_LIB)
