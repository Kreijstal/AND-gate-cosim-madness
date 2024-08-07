# Makefile for GHDL VHDL project

# GHDL command
GHDL = ghdl

# GHDL flags
GHDL_FLAGS = --std=08

# Source files
VHDL_SRC = and_gate.vhd and_gate_tb.vhd

# Top-level entity
TOP_ENTITY = and_gate_tb

# Output directory
WORK_DIR = work

# Default target: analyze, elaborate, run
all: run

# Analyze rule
analyze: $(VHDL_SRC)
	$(GHDL) -a $(GHDL_FLAGS) $(VHDL_SRC)

# Elaborate rule
elaborate: analyze
	$(GHDL) -e $(GHDL_FLAGS) $(TOP_ENTITY)

# Run rule
run: elaborate
	$(GHDL) -r $(GHDL_FLAGS) $(TOP_ENTITY) --wave=wave.ghw

# Clean rule
clean:
	$(GHDL) --remove
	rm -f *.o *.cf *.ghw

.PHONY: all analyze elaborate run clean
