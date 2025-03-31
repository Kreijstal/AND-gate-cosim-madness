# Makefile for Verilator implementation with Icarus Verilog VPI testbench

# --- Tools ---
VERILATOR = verilator
IVERILOG = iverilog
VVP = vvp
CXX = g++
# Use iverilog-vpi to compile VPI code correctly (Removed, using g++ directly again)
# IVERILOG_VPI = iverilog-vpi

# --- Platform Specific ---
# Default MINGW_DIR based on MINGW_PREFIX (for MSYS2)
MINGW_DIR ?= $(MINGW_PREFIX)
# Default VPI include dir
VPI_INCLUDE_DIR ?= $(MINGW_DIR)/include/iverilog

# Detect OS
ifeq ($(OS),Windows_NT)
    IS_WINDOWS := 1
    SHARED = -shared
    VPI_CFLAGS = -I$(VPI_INCLUDE_DIR)
    VPI_LDFLAGS = -L$(MINGW_DIR)/lib -lvpi -lveriuser -lpthread
else
    IS_WINDOWS := 0
    SHARED = -shared -fPIC # Add -fPIC back for non-Windows
    VPI_CFLAGS = -I/usr/include/iverilog # Standard Linux path
    VPI_LDFLAGS = -L/usr/lib -lvpi -lveriuser # Standard Linux libs
endif


# --- Flags ---
# Verilator flags: Create C++ model, specify top module, optimize
# C++ flags: Optimization, Wall, standard, include Verilator headers
# Remove -fPIC for Windows/MinGW shared library build
CXXFLAGS = -Wall -Wextra -O2 -std=c++14
# Icarus Verilog flags: Output file, include directories (if any)
IVERILOG_FLAGS = -Wall -g2012
# VVP flags: Specify VPI module(s)
VVP_FLAGS = -M. -m and_gate_verilator # Module name matches VPI_MODULE_NAME

# --- Source Files ---
COMMON_DIR = ../common
VERILOG_DUT_SRC = $(COMMON_DIR)/and_gate.v
VERILOG_TB_SRCS = and_gate_tb.v and_gate_proxy.v
VPI_SRC = and_gate_vpi.cpp

# --- Output Files ---
VERILATOR_OBJ_DIR = obj_dir_dut # Directory for Verilator generated files for DUT
# Strip potential whitespace from components before concatenating
VERILATOR_MODEL_LIB = $(strip $(VERILATOR_OBJ_DIR))/Vand_gate__ALL.a
VPI_MODULE_NAME = and_gate_verilator # Basename for the VPI shared library
# Strip potential whitespace from components before concatenating
VPI_MODULE = $(strip $(VPI_MODULE_NAME)).vpi
IVERILOG_OUTPUT = and_gate_tb.vvp # Compiled Icarus Verilog executable

# --- Verilator Include Paths ---
# Find Verilator include directory automatically
POSSIBLE_VERILATOR_ROOTS := $(wildcard /usr/local/share/verilator /usr/share/verilator $(MSYSTEM_PREFIX)/share/verilator)
VERILATOR_ROOT := $(firstword $(POSSIBLE_VERILATOR_ROOTS))
ifeq ($(VERILATOR_ROOT),)
    $(error Could not find Verilator installation root. Set VERILATOR_ROOT or install Verilator.)
endif
VERILATOR_INCLUDES = -I$(VERILATOR_ROOT)/include -I$(VERILATOR_ROOT)/include/vltstd
CXXFLAGS += $(VERILATOR_INCLUDES) -I$(VERILATOR_OBJ_DIR) # Add Verilator includes for VPI compilation

# --- Targets ---
.PHONY: all build run clean verilator_build

all: run

# Build target: Depends on Icarus output and VPI module
build: $(IVERILOG_OUTPUT)

# Run target: Depends on build
run: build
	@echo "--- Running Simulation (Icarus Verilog + Verilator VPI) ---"
	$(VVP) $(VVP_FLAGS) $(IVERILOG_OUTPUT)
	@echo "--- Simulation Finished ---"

# Rule to compile Verilog testbench with Icarus Verilog
$(IVERILOG_OUTPUT): $(VERILOG_TB_SRCS) $(VPI_MODULE)
	@echo "--- Compiling Verilog Testbench (Icarus Verilog) ---"
	$(IVERILOG) $(IVERILOG_FLAGS) -o $@ $(VERILOG_TB_SRCS)

# Rule to build the VPI shared library
# Depends on the VPI source and BOTH Verilator-generated static libraries
$(VPI_MODULE): $(VPI_SRC) $(VERILATOR_MODEL_LIB) $(VERILATOR_OBJ_DIR)/libverilated.a
	@echo "--- Building VPI Module ($(VPI_MODULE)) ---"
	# Compile manually using g++, specifying VPI paths/libs directly.
	# Use platform-specific flags defined above.
	$(CXX) $(CXXFLAGS) $(VPI_CFLAGS) $(SHARED) -o $(VPI_MODULE) $(VPI_SRC) $(VERILATOR_MODEL_LIB) $(strip $(VERILATOR_OBJ_DIR))/libverilated.a $(VPI_LDFLAGS)

# Rule to build the Verilator model static library AND runtime library
# Depends on the Verilog DUT source
$(VERILATOR_MODEL_LIB) $(VERILATOR_OBJ_DIR)/libverilated.a: $(VERILOG_DUT_SRC)
	@echo "--- Verilating DUT ($(VERILOG_DUT_SRC)) ---"
	# Verilate the DUT. Use --build to compile sources into static libraries.
	# Output goes to VERILATOR_OBJ_DIR.
	# Use platform-specific CFLAGS for Verilator build (remove -fPIC on Windows)
ifeq ($(IS_WINDOWS), 1)
	$(VERILATOR) --cc $(VERILOG_DUT_SRC) --top-module and_gate \
		-CFLAGS "-std=c++14 -O2 $(VERILATOR_INCLUDES)" \
		--build -j 0 --Mdir $(VERILATOR_OBJ_DIR)
else
	$(VERILATOR) --cc $(VERILOG_DUT_SRC) --top-module and_gate \
		-CFLAGS "-std=c++14 -O2 -fPIC $(VERILATOR_INCLUDES)" \
		--build -j 0 --Mdir $(VERILATOR_OBJ_DIR)
endif
	@echo "--- Verilator Build Complete ---"
	# Touch the targets to ensure Make recognizes they've been updated by the rule
	# Use strip to prevent potential whitespace issues in path construction
	touch $(VERILATOR_MODEL_LIB) $(strip $(VERILATOR_OBJ_DIR))/libverilated.a


# Clean target
clean:
	@echo "--- Cleaning ---"
	rm -rf ${VERILATOR_OBJ_DIR} # Remove Verilator build directory
	rm -f ${VPI_MODULE} # Remove VPI shared library
	rm -f ${IVERILOG_OUTPUT} # Remove Icarus Verilog output
	rm -f *.o # Remove any stray object files
	rm -f wave.vcd # Remove waveform file if generated
	rm -f and_gate_verilator.c # iverilog-vpi intermediate file

# Explicitly declare VERILATOR_MODEL_LIB as not PHONY if it matches a directory name
.SECONDARY: $(VERILATOR_MODEL_LIB)
