CXX := mpicxx
CFLAGS := -std=c++20 -fPIC -fopenmp -pthread

AR := ar
ARFLAGS := rvs

# `filter X, A B` return those of A, B that are equal to X
ifeq ($(VERSION), $(filter $(VERSION), "DEBUG" ""))
CFLAGS += -O0 -ggdb -Wall -Wextra -Wpedantic -Werror
PETSC_ARCH := linux-mpi-debug
endif

ifeq ($(VERSION), RELEASE)
CFLAGS += -O3
PETSC_ARCH := linux-mpi-opt
endif

DIR := $(realpath $(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PETSC_DIR := $(DIR)/external/petsc

INC_PATH := -I$(DIR)/src
INC_PATH += -I$(PETSC_DIR)/include -I$(PETSC_DIR)/$(PETSC_ARCH)/include

LIB_PATH := -L$(PETSC_DIR)/$(PETSC_ARCH)/lib
LIBS := -Wl,-rpath=$(PETSC_DIR)/$(PETSC_ARCH)/lib -lpetsc -lf2clapack -lf2cblas -lm -lX11 # -lquadmath

OBJ_DIR := $(DIR)/bin-int
BIN_DIR := $(DIR)/bin

SRCS :=            \
	src/context.cpp  \
	src/vec.cpp      \
	src/is.cpp       \
	src/mat.cpp      \
	src/ksp.cpp      \
	src/dm.cpp       \
	src/dmda.cpp     \
	src/viewer.cpp   \
	src/binary.cpp   \

SRCS := $(addprefix $(DIR)/, $(SRCS))
OBJS := $(SRCS:$(DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:$(DIR)/%.cpp=$(OBJ_DIR)/%.d)

all: prepare_binaries create_static_library

-include $(DEPS)

# creates a directory for the target if it doesn't exist
MKDIR=@mkdir -p $(@D)

prepare_binaries: $(OBJS)

$(OBJ_DIR)/%.o: $(DIR)/%.cpp message_compiling
	$(MKDIR)
	$(CXX) $(CFLAGS) $(INC_PATH) -c $< -o $@

$(OBJ_DIR)/%.d: $(DIR)/%.cpp
	$(MKDIR)
	@$(CXX) $(CFLAGS) $(INC_PATH) $< -MM -MT $(@:$(OBJ_DIR)/%.d=$(OBJ_DIR)/%.o) >$@

# to prevent multiple messages
.INTERMEDIATE: message_compiling
message_compiling:
	@echo -e "\033[0;33mCompiling files from $(DIR)/src/**\033[0m"


create_static_library: $(BIN_DIR)/wrapper.a

$(BIN_DIR)/wrapper.a: $(OBJS)
	@echo -e "\033[0;33mCompiling static library $(BIN_DIR)/wrapper.a\033[0m"
	$(MKDIR)
	@$(AR) $(ARFLAGS) $(BIN_DIR)/wrapper.a $^


.PHONY: clean
clean:
	@rm -r $(OBJ_DIR) $(BIN_DIR)
