# DIRS
OBJ_DIR = $(PWD)/obj
BIN_DIR = $(PWD)/bin
LIB_DIR = $(PWD)/lib

# INCLUDE AND LIBRARY PATHS
INCLUDES = -I/usr/include -I/usr/local/include -I$(PWD)/include
LIBS = -L/usr/lib -L/usr/local/lib -L$(LIB_DIR) -ldstruct -lal

# C COMPILER
CC = clang
STANDARD = -std=gnu99
DEBUG_FLAGS = -g
WARN_FLAGS = -Weverything -Wno-padded
CFLAGS = $(INCLUDES) $(DEBUG_FLAGS) $(WARN_FLAGS)

COMPILE_OBJ = \
	echo "COMPILING [$<]"; \
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $@);

MAKE_EXE = \
	echo "LINKING [$<]"; \
	$(CC) $(addprefix $(OBJ_DIR)/, $<) -o $(addprefix $(BIN_DIR)/, $@) $(LIBS);

MAKE_STATIC_LIB = \
	echo "LINK STATIC LIBRARY[$@]"; \
	$(AR) $(ARFLAGS) $(LIB_DIR)/libdstruct.a $(addprefix $(OBJ_DIR)/, $(OBJS)); \


# ARCHIVER
AR = ar
ARFLAGS = rvs
