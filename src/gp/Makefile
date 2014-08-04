include $(PWD)/config.mk
FOLDER_NAME = src/gp

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: evolve_gp

gp_tree:
	@make -s -C tree

gp_cartesian:
	@make -s -C cartesian

%.o: %.c
	$(MAKE_OBJ)

evolve_gp: gp_tree gp_cartesian $(OBJS)
