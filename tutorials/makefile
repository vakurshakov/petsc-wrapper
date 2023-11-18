BASE_DIR = ..

include $(BASE_DIR)/makefile

# looking for binaries in the base directory
OBJS := $(OBJS:%.o=$(BASE_DIR)/%.o)
DEPS := $(DEPS:%.d=$(BASE_DIR)/%.d)

# target for any .cpp file inside this directory
%: %.cpp
	@$(MAKE) --directory=$(BASE_DIR)/

	@echo -e "\033[0;33mCompiling executable $@.\033[0m"
	$(CXX) $(CFLAGS) $(INC_PATH) $(LIB_PATH) $(LIBS) $(OBJS) $< -o $@
