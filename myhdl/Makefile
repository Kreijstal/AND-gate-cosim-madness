# Compiler and flags
IVERILOG = iverilog
VVP = vvp
PYTHON = python3

# MyHDL specifics
MYHDL_REPO = https://github.com/myhdl/myhdl.git
MYHDL_DIR = myhdl
VPI_DIR = $(MYHDL_DIR)/cosimulation/icarus

# Source files
SRC_DIR = ../common
VERILOG_SRCS = $(SRC_DIR)/and_gate.v and_gate_myhdl.v
VERILOG_TB = and_gate_myhdl.v
VERILOG_TB_EXEC = and_gate_myhdl.vvp
VPI_PATH = $(wildcard ./myhdl.vpi)

# Main targets
.PHONY: all clean setup_myhdl run_myhdl

all: setup_myhdl run_myhdl

# MyHDL setup target
setup_myhdl:
	@echo "Setting up MyHDL"
	@if [ ! -d "$(MYHDL_DIR)" ]; then \
		echo "Cloning MyHDL repository"; \
		git clone $(MYHDL_REPO); \
	else \
		echo "MyHDL directory already exists. Skipping clone."; \
	fi
	@if [ ! -f "myhdl.vpi" ]; then \
		echo "Building MyHDL VPI module"; \
		$(MAKE) -C $(VPI_DIR); \
		cp $(VPI_DIR)/myhdl.vpi .; \
	else \
		echo "myhdl.vpi already exists. Skipping build."; \
	fi

# MyHDL run target
run_myhdl: $(VERILOG_TB_EXEC)
	@echo "Running MyHDL simulation"
	PYTHONPATH=$(CURDIR)/$(MYHDL_DIR) $(PYTHON) and_gate_tb.py

# Verilog testbench compilation
$(VERILOG_TB_EXEC): $(VERILOG_SRCS)
	@echo "Compiling Verilog testbench: $@"
	$(IVERILOG) -o $@ $(VERILOG_SRCS)

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -f $(VERILOG_TB_EXEC)
	rm -f *.vvp
	rm -f $(VPI_PATH)

