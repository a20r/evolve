include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: evolve_gp intro $(OBJS) libevolve done

evolve_gp:
	@make -s -C gp

intro:
	@echo "ENTERING --> $(FOLDER_NAME)"

%.o: %.c
	$(MAKE_OBJ)

libevolve: evolve_gp
	$(MAKE_STATIC_LIB)

done:
	@echo "EXITING <-- $(FOLDER_NAME)"
	@echo ""
