BUILD_DIR=build
SRC_DIR=src
LIB_DIR=$(SRC_DIR)/lib
UTIL_DIR=util

BOBJ_DIR=$(BUILD_DIR)/bobj
BOBJ_COMPILER=../assembler/compiler.py
BOBJ_FLAGS=
#BOBJ=$(foreach fname,$(BASM_LIST),$(BOBJ_DIR)/$(basename $(fname)).bobj)

BEXE_DIR=$(BUILD_DIR)/bexe
BEXE_COMPILER=../assembler/linker.py
BEXE_FLAGS=

BF_SRC_DIR=$(BUILD_DIR)/bf
BVM_COMPILER=$(UTIL_DIR)/interpreter
BVM_FLAGS=
BF_COMPILER=$(UTIL_DIR)/bf2c
BF_FLAGS=

C_SRC_DIR=$(BUILD_DIR)/c_src
C_COMPILER=gcc
C_FLAGS=-O0

TARGETS=$(patsubst $(SRC_DIR)/%.basm,$(BUILD_DIR)/%,$(wildcard $(SRC_DIR)/test_*.basm)) $(BUILD_DIR)/ctf
LIB_SOURCES=$(wildcard $(LIB_DIR)/*.basm)
LIB_OBJECTS=$(LIB_SOURCES:$(LIB_DIR)/%.basm=$(BOBJ_DIR)/lib/%.bobj)

DIRS=$(BUILD_DIR) $(BOBJ_DIR) $(BOBJ_DIR)/lib $(BEXE_DIR) $(BF_SRC_DIR) $(C_SRC_DIR) $(UTIL_DIR)
BUILDABLE_COMPILERS=$(BF_COMPILER) $(BVM_COMPILER)

.DEFAULT_GOAL := all

.PHONY: clean all


all: $(TARGETS)


$(BUILDABLE_COMPILERS): %: | cmake_preprocess
	$(MAKE) -C $(UTIL_DIR) $(notdir $@)


cmake_preprocess: | $(UTIL_DIR)
	cd $(UTIL_DIR) && cmake ../../codegen

# $(TARGETS): $(BUILD_DIR)/%: $(C_SRC_DIR)/%.c
$(BUILD_DIR)/%: $(C_SRC_DIR)/%.c | $(BUILD_DIR)
		$(C_COMPILER) $(C_FLAGS) -o $@ $<


$(C_SRC_DIR)/%.c: $(BF_SRC_DIR)/%.bf | $(BF_COMPILER) $(C_SRC_DIR)
		$(BF_COMPILER) $(BF_FLAGS) < $< > $@


$(BF_SRC_DIR)/%.bf: $(BEXE_DIR)/%.bexe | $(BVM_COMPILER) $(BF_SRC_DIR)
		$(BVM_COMPILER) $(BVM_FLAGS) -o $@ $<


$(BEXE_DIR)/%.bexe: $(BOBJ_DIR)/%.bobj $(LIB_OBJECTS) | $(BEXE_DIR)
		$(BEXE_COMPILER) $(BEXE_FLAGS) -o $@ $^


$(BOBJ_DIR)/%.bobj: $(SRC_DIR)/%.basm | $(BOBJ_DIR) $(BOBJ_DIR)/lib
		$(BOBJ_COMPILER) -o $(dir $@) $<

$(DIRS):
		mkdir -p $@

clean:
		rm -rfv $(DIRS)
