# Variables
VPI_SRC = and_gate_tb.c
VPI_OUT = and_gate_tb.vpi
VERILOG_SRCS = ../common/and_gate.v and_gate_tb_wrapper.v
VVP_EXEC = and_gate_tb.vvp
VCD_FILE = and_gate.vcd

# Compiler and flags
IVERILOG_VPI = iverilog-vpi
IVERILOG = iverilog
VVP = vvp
CFLAGS = -Wall -Wextra -g

# Main target
.PHONY: all clean build run

all: run

# Build VPI shared library
$(VPI_OUT): $(VPI_SRC)
	@echo "Building VPI shared library: $@"
	$(IVERILOG_VPI) $(CFLAGS) $<

# Compile Verilog sources
$(VVP_EXEC): $(VERILOG_SRCS) $(VPI_OUT)
	@echo "Compiling Verilog sources: $@"
	$(IVERILOG) -o $@ $(VERILOG_SRCS)

# Build target
build: $(VVP_EXEC)

# Run simulation
run: $(VVP_EXEC)
	@echo "Running VPI simulation"
	$(VVP) -M. -m$(basename $(VPI_OUT)) $<

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -f $(VPI_OUT) $(VVP_EXEC) $(VCD_FILE) *.o
