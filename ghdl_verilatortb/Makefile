# Makefile for GHDL and Verilator Co-simulation

# --- Variables ---
VERILATOR ?= verilator
GHDL      ?= ghdl
CXX       ?= g++

# Verilator include and library paths
VERILATOR_ROOT ?= /usr/share/verilator
VERILATOR_INC_DIR := $(VERILATOR_ROOT)/include
VERILATOR_LIB_DIR := $(VERILATOR_ROOT)/lib # May not be needed if static linking or if libs are in standard paths

# Source files
VERILOG_SOURCES := and_gate.v
VHDL_PKG        := and_gate_pkg.vhd
VHDL_TB         := and_gate_tb.vhd
WRAPPER_CPP     := and_gate_wrapper.cpp
TOPLEVEL_VHDL   := and_gate_tb # The VHDL entity name for simulation
DESIGN_NAME     := and_gate  # Basename for Verilated files

# Output files and directories
OBJ_DIR      := obj_dir
SIM_EXEC     := $(DESIGN_NAME)_sim
VERILATED_C_MAIN := $(OBJ_DIR)/V$(DESIGN_NAME).cpp # Verilator generated C++ main, not used by us
VERILATED_H      := $(OBJ_DIR)/V$(DESIGN_NAME).h
VERILATED_O      := $(OBJ_DIR)/V$(DESIGN_NAME)__ALL.o # Often an object file or archive
# Verilator also produces other .h and .cpp files in OBJ_DIR
# We will compile them via Verilator's own makefile or by directly compiling sources.

# Compiler and Linker flags
# CFLAGS for Verilator to pass to C++ compiler, DVL_USER_FINI allows for global finalization
VERILATOR_CFLAGS := -DVL_USER_FINI -std=c++11
CXXFLAGS_COMMON  := -I$(VERILATOR_INC_DIR) -I$(OBJ_DIR) -Wall -Wno-parentheses -std=c++11
CXXFLAGS         += $(CXXFLAGS_COMMON)
LDFLAGS          += -L$(VERILATOR_LIB_DIR) # If Verilator provides shared libraries

# GHDL flags
GHDL_ANALYZE_FLAGS  := --std=08 -fsynopsys -fexplicit
GHDL_ELABORATE_FLAGS:= --std=08 -fsynopsys -fexplicit

# --- Targets ---

# Default target
all: $(SIM_EXEC)
	@echo "Running simulation..."
	./$(SIM_EXEC)

# Simulation executable
# Links the elaborated VHDL testbench with all C++ objects
# The -Wl, prefix passes the following comma-separated list as arguments to the linker (ld)
# VERILATOR_COMMON_O are common Verilator runtime objects.
# We need verilated.o and verilated_vpi.o for VHPIDIRECT support with Verilator.
# These are typically compiled from verilated.cpp and verilated_vpi.cpp from VERILATOR_INC_DIR
VERILATOR_SUPPORT_O := $(OBJ_DIR)/verilated.o $(OBJ_DIR)/verilated_vpi.o
$(SIM_EXEC): $(VHDL_PKG) $(VHDL_TB) $(WRAPPER_CPP) $(VERILOG_SOURCES) $(OBJ_DIR)/V$(DESIGN_NAME)__ALL.a $(VERILATOR_SUPPORT_O) and_gate_wrapper.o
	$(GHDL) -a $(GHDL_ANALYZE_FLAGS) $(VHDL_PKG)
	$(GHDL) -a $(GHDL_ANALYZE_FLAGS) $(VHDL_TB)
	$(GHDL) -e $(GHDL_ELABORATE_FLAGS) $(TOPLEVEL_VHDL) -o $(SIM_EXEC) \
		-Wl,and_gate_wrapper.o \
		-Wl,$(OBJ_DIR)/V$(DESIGN_NAME)__ALL.a \
		-Wl,$(VERILATOR_SUPPORT_O) \
		-Wl,-lstdc++ # Link with standard C++ library

# Verilated C++ objects (archive)
# Verilator generates its own makefile (e.g., obj_dir/Vand_gate.mk)
# which is the preferred way to compile its generated sources.
$(OBJ_DIR)/V$(DESIGN_NAME)__ALL.a: $(VERILOG_SOURCES)
	@echo "Verilating $(VERILOG_SOURCES)..."
	$(VERILATOR) --cc --exe $(VERILOG_SOURCES) -CFLAGS "$(VERILATOR_CFLAGS)" --build -j 0 -o ../$(SIM_EXEC) --top-module $(DESIGN_NAME) --Mdir $(OBJ_DIR)
	@echo "Building Verilated sources..."
	$(MAKE) -C $(OBJ_DIR) -f V$(DESIGN_NAME).mk V$(DESIGN_NAME)__ALL.a

# Compile Verilator's support files (verilated.cpp, verilated_vpi.cpp)
# These are needed for VHPIDIRECT/foreign language interface.
$(OBJ_DIR)/verilated.o: $(VERILATOR_INC_DIR)/verilated.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/verilated_vpi.o: $(VERILATOR_INC_DIR)/verilated_vpi.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Wrapper object
and_gate_wrapper.o: $(WRAPPER_CPP) $(VERILATED_H)
	$(CXX) $(CXXFLAGS) -c $(WRAPPER_CPP) -o $@

# Phony target for running the simulation
run: all
	# 'all' target already runs the simulation

# Clean target
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR)
	rm -f $(SIM_EXEC)
	rm -f *.o # Removes and_gate_wrapper.o
	rm -f *.cf # GHDL intermediate files
	rm -f work-obj93.cf # GHDL library file
	rm -f e~*.o # GHDL temporary files if any
	# Add any other generated files here

# Phony targets
.PHONY: all run clean
