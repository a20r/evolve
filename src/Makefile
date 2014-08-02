include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: src libevolve

%.o: %.c
	$(MAKE_OBJ)

subdirs:
	@make -s -C gp

src: subdirs $(OBJS)

libevolve: $(OBJS)
	$(MAKE_STATIC_LIB)

src: subdirs $(OBJS)
