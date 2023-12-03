DIR = ..

include $(DIR)/makefile

# looking for binaries in the base directory
LIB_PATH += -L$(BIN_DIR)
LIBS += $(BIN_DIR)/wrapper.a

# target for any .cpp file inside this directory
%: %.cpp
	@$(MAKE) --directory=$(DIR)/

	@echo -e "\033[0;33mCompiling executable $@\033[0m"
	$(CXX) $(CFLAGS) $< $(INC_PATH) $(LIB_PATH) $(LIBS) -o $@
