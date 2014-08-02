# DIRS
OBJ_DIR = $(PWD)/obj
BIN_DIR = $(PWD)/bin
LIB_DIR = $(PWD)/lib

# INCLUDE AND LIBRARY PATHS
INCLUDES = -I/usr/include -I/usr/local/include -I$(PWD)/include
# LIBS = -L/usr/lib -L/usr/local/lib -L$(LIB_DIR) -lm -levolve -lOpenCL
LIBS = -L/usr/lib -L/usr/local/lib -L$(LIB_DIR) -lm -levolve

# C COMPILER
CC = clang
STANDARD = -std=gnu99
DEBUG_FLAGS = -g
WARN_FLAGS = -Weverything \
			 -Wno-padded \
			 -Wno-gnu-zero-variadic-macro-arguments \
			 -Wno-empty-translation-unit
CFLAGS = $(INCLUDES) $(DEBUG_FLAGS) $(WARN_FLAGS)

# ARCHIVER
AR = ar
ARFLAGS = rvs

# COMMANDS
MAKE_OBJ = \
	@echo "CC [$<]"; \
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $@);

MAKE_TEST = \
	echo "CC [$@.c]"; \
	$(CC) $(CFLAGS) -c $@.c -o $(addprefix $(OBJ_DIR)/, $@.o); \
	$(CC) $(addprefix $(OBJ_DIR)/, $@.o) -o $(addprefix $(BIN_DIR)/, $@) $(LIBS);

MAKE_STATIC_LIB = \
	echo "AR [$@]"; \
	$(AR) $(ARFLAGS) $(LIB_DIR)/$@.a $(wildcard $(OBJ_DIR)/*.o);
