# Makefile for GHDL VHDL project with C function

# GHDL command
GHDL = ghdl

# GHDL flags
GHDL_FLAGS = --std=08

# C compiler
CC = gcc

# C compiler flags
CFLAGS = -c -fPIC

# Source files
VHDL_SRC = and_gate_pkg.vhd and_gate.vhd and_gate_tb.vhd
C_SRC = and_gate_c.c

# Object files
C_OBJ = and_gate_c.o
VHDL_OBJ = work-obj08.cf

# Top-level entity
TOP_ENTITY = and_gate_tb

# Default target: analyze, elaborate, run
all: run
build: $(TOP_ENTITY)

# Compile C source
$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Analyze VHDL files
$(VHDL_OBJ): $(VHDL_SRC) $(C_OBJ)
	$(GHDL) -a $(GHDL_FLAGS) $(VHDL_SRC)

# Elaborate
$(TOP_ENTITY): $(VHDL_OBJ) $(C_OBJ)
	$(GHDL) -e $(GHDL_FLAGS) $(TOP_ENTITY) ./$(C_OBJ)

# Run
run: $(TOP_ENTITY)
	./$(TOP_ENTITY)

# Clean rule
clean:
	$(GHDL) --remove
	rm -f *.o *.cf *.ghw $(TOP_ENTITY)

.PHONY: all run clean
