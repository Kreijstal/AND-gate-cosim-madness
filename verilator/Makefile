# Variables for Verilator
VERILATOR = verilator
VERILATOR_FLAGS = --main --binary -Wall -Wno-TIMESCALEMOD

# Sources
SRC_DIR = ../common
VERILOG_SRCS = $(SRC_DIR)/and_gate.v
VERILOG_TB = $(SRC_DIR)/and_gate_tb.v
VERILATOR_EXEC = Vand_gate_tb

# Main target
.PHONY: all clean run_verilator

all: run_verilator

# Verilator specific targets
$(VERILATOR_EXEC): $(VERILOG_SRCS) $(VERILOG_TB)
	@echo "Compiling with Verilator: $@"
	$(VERILATOR) $(VERILATOR_FLAGS) -y $(SRC_DIR) $(VERILOG_TB)

run_verilator: $(VERILATOR_EXEC)
	@echo "Running Verilator simulation"
	./obj_dir/$(VERILATOR_EXEC)

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -rf obj_dir
