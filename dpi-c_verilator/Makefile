# Makefile for Verilator DPI-C project

# Verilator command
VERILATOR = verilator

# Verilator flags
VERILATOR_FLAGS = --cc --exe --sv

# Source files
VERILOG_SRC = and_gate_proxy.sv
CPP_SRC = and_gate_tb.cpp and_gate_dpi.cpp

# Output directory
OBJ_DIR = obj_dir

# Target executable
TARGET = $(OBJ_DIR)/Vand_gate_proxy

# Default target: build and run
all: run

# Verilator generation rule
$(OBJ_DIR)/Vand_gate_proxy.mk: $(VERILOG_SRC) $(CPP_SRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILOG_SRC) $(CPP_SRC)

# Compilation rule
$(TARGET): $(OBJ_DIR)/Vand_gate_proxy.mk
	make -j -C $(OBJ_DIR) -f Vand_gate_proxy.mk Vand_gate_proxy

# Build rule
build: $(TARGET)

# Clean rule
clean:
	rm -rf $(OBJ_DIR)

# Run rule
run: $(TARGET)
	$(TARGET)

.PHONY: all build clean run
