# Makefile for Verilator DPI-C project with Verilog testbench

# Verilator command
VERILATOR ?= $(or $(wildcard /usr/bin/verilator),verilator)
# Verilator flags
VERILATOR_FLAGS = --cc --exe --main --sv --timing -CFLAGS -DVL_TIME_CONTEXT

# Source files
VERILOG_SRC = and_gate_proxy.sv ../common/and_gate_tb.v
CPP_SRC = and_gate_dpi.cpp

# Verilator top module
TOP_MODULE = and_gate_tb

# Output directory
OBJ_DIR = obj_dir

# Target executable
TARGET = $(OBJ_DIR)/V$(TOP_MODULE)

# Default target: build and run
all: run

# Verilator generation rule
$(OBJ_DIR)/V$(TOP_MODULE).mk: $(VERILOG_SRC) $(CPP_SRC)
	$(VERILATOR) $(VERILATOR_FLAGS) --top-module $(TOP_MODULE) $(VERILOG_SRC) $(CPP_SRC)

# Compilation rule
$(TARGET): $(OBJ_DIR)/V$(TOP_MODULE).mk
	make -j -C $(OBJ_DIR) -f V$(TOP_MODULE).mk V$(TOP_MODULE)

# Build rule
build: $(TARGET)

# Clean rule
clean:
	rm -rf $(OBJ_DIR)

# Run rule
run: $(TARGET)
	$(TARGET)

.PHONY: all build clean run
