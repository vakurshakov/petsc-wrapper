CXX    := mpicxx
CFLAGS := -std=c++20 -fPIC -fopenmp -pthread

# `filter X, A B` return those of A, B that are equal to X
ifeq ($(VERSION), $(filter $(VERSION), "DEBUG" ""))
CFLAGS += -O0 -ggdb -Wall -Wextra -Wpedantic -Wno-unused-parameter -Werror
PETSC_ARCH := linux-mpi-debug
endif

ifeq ($(VERSION), RELEASE)
CFLAGS += -O3
PETSC_ARCH := linux-mpi-opt
endif

PETSC_DIR  := $(PWD)/external/petsc

INC_PATH := -I$(PWD)/src
INC_PATH += -I$(PETSC_DIR)/include -I$(PETSC_DIR)/$(PETSC_ARCH)/include

LIB_PATH := -L$(PETSC_DIR)/$(PETSC_ARCH)/lib

LIBS := -Wl,-rpath=$(PETSC_DIR)/$(PETSC_ARCH)/lib -lpetsc

EXECUTABLE := a.out

RESDIR := ./
OBJDIR := ./bin-int

SRCS :=            \
	main.cpp         \
	src/context.cpp  \
	src/vec.cpp      \
	src/mat.cpp      \
	src/ksp.cpp      \
	src/dm.cpp       \
	src/dmda.cpp     \


OBJS := $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS := $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: $(RESDIR)/$(EXECUTABLE)

-include $(DEPS)

# creates a directory for the target if it doesn't exist
MKDIR=@mkdir -p $(@D)

$(RESDIR)/$(EXECUTABLE): $(OBJS)
	@echo -e "\033[0;33m\nCreating the resulting binary.\033[0m"
	$(MKDIR)
	$(CXX) $(CFLAGS) $(LIB_PATH) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: %.cpp message_compiling
	$(MKDIR)
	$(CXX) $(CFLAGS) $(INC_PATH) -c $< -o $@

$(OBJDIR)/%.d: %.cpp
	$(MKDIR)
	@$(CXX) $(CFLAGS) $(INC_PATH) $< -MM -MT $(@:$(OBJDIR)/%.d=$(OBJDIR)/%.o) >$@

.PHONY: clean
clean:
	@rm $(DEPS) $(OBJS) $(RESDIR)/$(EXECUTABLE)

# to prevent multiple messages
.INTERMEDIATE: message_compiling
message_compiling:
	@echo -e "\033[0;33m\nCompiling other files from src/**.\033[0m"
