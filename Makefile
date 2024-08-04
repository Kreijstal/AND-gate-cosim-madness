# Variables for Verilog/VPI simulation
VPI_OBJS = and_gate_tb.o
VPI_SHARED_LIB = and_gate_tb.vpi
VERILOG_SRCS = and_gate.v and_gate_tb_wrapper.v
VVP_EXEC = and_gate.vvp
VCD_FILE = and_gate.vcd

# Variables for Verilog testbench
VERILOG_TB = and_gate_tb.v
VERILOG_TB_EXEC = and_gate_tb.vvp

# Variables for Verilator
VERILATOR = verilator
VERILATOR_FLAGS = --main --binary -Wall
VERILATOR_TB = and_gate_tb
VERILATOR_EXEC = obj_dir/V$(VERILATOR_TB)

# Variables for MyHDL simulation
MYHDL_REPO = https://github.com/myhdl/myhdl.git
MYHDL_DIR = myhdl
PYTHON = python3
VPI_DIR = $(MYHDL_DIR)/cosimulation/icarus

# Compiler and flags
IVERILOG_VPI = iverilog-vpi
IVERILOG = iverilog
VVP = vvp

# Main targets
.PHONY: all verilog myhdl verilog_tb verilator clean deep_clean setup_myhdl

all: verilog myhdl verilog_tb verilator

# Verilog/VPI simulation target
verilog: run_verilog

# MyHDL simulation target
myhdl: setup_myhdl run_myhdl

# Verilog testbench target
verilog_tb: run_verilog_tb

# Verilator target
verilator: run_verilator

# Verilog/VPI specific targets
$(VPI_SHARED_LIB): and_gate_tb.c
	$(IVERILOG_VPI) and_gate_tb.c

$(VVP_EXEC): $(VERILOG_SRCS)
	$(IVERILOG) -o $@ $^

run_verilog: $(VPI_SHARED_LIB) $(VVP_EXEC)
	$(VVP) -M. -mand_gate_tb $(VVP_EXEC)

# Verilog testbench specific targets
$(VERILOG_TB_EXEC): and_gate.v $(VERILOG_TB)
	$(IVERILOG) -o $@ $^

run_verilog_tb: $(VERILOG_TB_EXEC)
	$(VVP) $(VERILOG_TB_EXEC)

# Verilator specific targets
$(VERILATOR_EXEC): and_gate.v $(VERILOG_TB)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILOG_TB) and_gate.v

run_verilator: $(VERILATOR_EXEC)
	$(VERILATOR_EXEC)

# MyHDL setup target
setup_myhdl:
	@if [ ! -d "$(MYHDL_DIR)" ]; then \
		git clone $(MYHDL_REPO); \
	else \
		echo "MyHDL directory already exists. Skipping clone."; \
	fi
	@if [ ! -f "myhdl.vpi" ]; then \
		$(MAKE) -C $(VPI_DIR); \
		cp $(VPI_DIR)/myhdl.vpi .; \
	else \
		echo "myhdl.vpi already exists. Skipping build."; \
	fi

# MyHDL run target
run_myhdl:
	PYTHONPATH=$(CURDIR)/$(MYHDL_DIR) $(PYTHON) and_gate_tb.py

# Clean target
clean:
	rm -f $(VPI_OBJS) $(VPI_SHARED_LIB) $(VVP_EXEC) $(VCD_FILE)
	rm -f *.vvp
	rm -rf obj_dir

# Deep clean (including MyHDL directory and VPI)
deep_clean: clean
	rm -rf $(MYHDL_DIR) myhdl.vpi
