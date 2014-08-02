include $(PWD)/config.mk
FOLDER_NAME = src/gp

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: intro $(OBJS) done

intro:
	@echo "ENTERING --> $(FOLDER_NAME)"

%.o: %.c
	$(MAKE_OBJ)

done:
	@echo "EXITING <-- $(FOLDER_NAME)"
	@echo ""
