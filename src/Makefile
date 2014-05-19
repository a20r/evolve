include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: dstruct

%.o: %.c
	$(COMPILE_OBJ)

dstruct: $(OBJS)
	$(MAKE_STATIC_LIB)
