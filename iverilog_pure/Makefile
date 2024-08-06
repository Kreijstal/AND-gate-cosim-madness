# Compiler and flags
IVERILOG = iverilog
VVP = vvp
CFLAGS = -Wall -Wextra -g

# Sources
SRC_DIR = ../common
VERILOG_SRCS = $(SRC_DIR)/and_gate.v
VERILOG_TB = $(SRC_DIR)/and_gate_tb.v
VERILOG_TB_EXEC = and_gate_tb.vvp
VCD_FILE = and_gate.vcd

# Main target
.PHONY: all clean

all: run_verilog_tb

# Verilog testbench specific targets
$(VERILOG_TB_EXEC): $(VERILOG_SRCS) $(VERILOG_TB)
	@echo "Compiling Verilog testbench: $@"
	$(IVERILOG) -o $@ $^

run_verilog_tb: $(VERILOG_TB_EXEC)
	@echo "Running Verilog testbench"
	$(VVP) $(VERILOG_TB_EXEC)

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -f $(VERILOG_TB_EXEC) $(VCD_FILE)

