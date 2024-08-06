# Makefile for VPI Verilator project

# Verilator and C++ compiler settings
VERILATOR = verilator
CXX = g++
CXXFLAGS = -Wall -O2
VERILATOR_FLAGS = --cc --exe --vpi

# Project files
VERILOG_SRC = and_gate_proxy.v
CPP_SRC = and_gate_sim_main.cpp and_gate_vpi.cpp
TARGET = and_gate_proxy

# Verilated output directory
VOBJ_DIR = obj_dir

# Default target: build and run
.PHONY: run
run: $(VOBJ_DIR)/V$(TARGET)
	@echo "Running simulation..."
	@$(VOBJ_DIR)/V$(TARGET)

# Verilate and compile
$(VOBJ_DIR)/V$(TARGET): $(VERILOG_SRC) $(CPP_SRC)
	@echo "Verilating and compiling..."
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILOG_SRC) $(CPP_SRC)
	@echo "Building..."
	make -j -C $(VOBJ_DIR) -f V$(TARGET).mk V$(TARGET)

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(VOBJ_DIR)

# Help target
.PHONY: help
help:
	@echo "Makefile targets:"
	@echo "  run   - Build and run the simulation (default)"
	@echo "  clean - Remove compiled files"
	@echo "  help  - Display this help message"
