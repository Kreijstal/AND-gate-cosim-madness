# Compiler and flags
IVERILOG = iverilog
VVP = vvp
CFLAGS = -Wall -Wextra -g

# Sources
SRC_DIR = .
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

# spicebind_vpi.vpi takes parameters from the environment.
run_verilog_tb: $(VERILOG_TB_EXEC) spicebind_vpi.vpi
	@echo "Running Verilog testbench"
	export SPICE_NETLIST=and_gate.cir ; \
	export HDL_INSTANCE=and_gate_tb.uut ; \
	export VCC=3.3 ; \
	$(VVP) -m ./spicebind_vpi.vpi $(VERILOG_TB_EXEC)

spicebind_vpi.vpi:
	@echo
	@echo Add a copy '(or link)' of spicebind_vpi.vpi to this directory,
	@echo built from https://github.com/themperek/spicebind.
	@echo Spicebind needs an installed copy of the Ngspice shared library:
	@echo https://sourceforge.net/projects/ngspice/.
	@echo
	@false

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -f $(VERILOG_TB_EXEC) $(VCD_FILE)
	rm -f *.raw *.vcd *~
