# Makefile for Verilator DUT with GHDL (VHDL) Testbench via DPI-C

# --- Tools ---
VERILATOR = verilator
GHDL = ghdl
CXX = g++

# --- Platform Specific ---
# Detect OS for shared library flags and paths
ifeq ($(OS),Windows_NT)
    IS_WINDOWS := 1
    SHARED_EXT = .dll
    SHARED_LDFLAG = -shared
    # Assume MSYS2/MinGW environment for Windows paths
    MINGW_DIR ?= $(MINGW_PREFIX)
    VERILATOR_DEFAULT_ROOT = $(MSYSTEM_PREFIX)/share/verilator
    CXXFLAGS_PLATFORM =
    GHDL_LINK_PREFIX = # GHDL handles DLL linking differently on Windows
else
    IS_WINDOWS := 0
    SHARED_EXT = .so
    SHARED_LDFLAG = -shared -fPIC
    VERILATOR_DEFAULT_ROOT = /usr/share/verilator # Common Linux path
    CXXFLAGS_PLATFORM = -fPIC # Needed for shared libraries on Linux
    GHDL_LINK_PREFIX = -Wl, # Prefix for linker flags passed via GHDL
endif

# --- Verilator Setup ---
# Find Verilator include directory automatically
POSSIBLE_VERILATOR_ROOTS := $(wildcard /usr/local/share/verilator $(VERILATOR_DEFAULT_ROOT))
VERILATOR_ROOT := $(firstword $(POSSIBLE_VERILATOR_ROOTS))
ifeq ($(VERILATOR_ROOT),)
    $(error Could not find Verilator installation root. Set VERILATOR_ROOT or install Verilator.)
endif
VERILATOR_INCLUDES = -I$(VERILATOR_ROOT)/include -I$(VERILATOR_ROOT)/include/vltstd

# --- Flags ---
CXXFLAGS = -Wall -Wextra -O2 -std=c++14 $(CXXFLAGS_PLATFORM) $(VERILATOR_INCLUDES)
GHDL_FLAGS = --std=08

# --- Source Files ---
COMMON_DIR = ../common
VERILOG_DUT_SRC = $(COMMON_DIR)/and_gate.v
VHDL_TB_SRCS = and_gate_tb.vhd # Add pkg file if needed later
CXX_GLUE_SRC = verilator_dpi_glue.cpp

# --- Output Files ---
VERILATOR_OBJ_DIR = obj_dir_dut # Directory for Verilator generated files
# Strip potential whitespace from components before concatenating
VERILATOR_MODEL_LIB = $(strip $(VERILATOR_OBJ_DIR))/Vand_gate__ALL.a
VERILATOR_DPI_LIB = $(strip $(VERILATOR_OBJ_DIR))/verilated_dpi.a
VERILATOR_RUNTIME_LIB = $(strip $(VERILATOR_OBJ_DIR))/verilated.a

SHARED_LIB_NAME = libverilator_dpi_glue$(SHARED_EXT)
GHDL_EXE_NAME = and_gate_tb_ghdl

# --- Targets ---
.PHONY: all build run clean verilator_build

all: run

build: $(GHDL_EXE_NAME)

run: build
	@echo "--- Running Simulation (GHDL Testbench + Verilator DUT via DPI-C) ---"
	./$(GHDL_EXE_NAME)
	@echo "--- Simulation Finished ---"

# Rule to build the GHDL executable, linking the shared C++ library
$(GHDL_EXE_NAME): $(VHDL_TB_SRCS) $(SHARED_LIB_NAME)
	@echo "--- Analyzing VHDL (GHDL) ---"
	$(GHDL) -a $(GHDL_FLAGS) $(VHDL_TB_SRCS)
	@echo "--- Elaborating VHDL & Linking C++ Library (GHDL) ---"
	# Pass the shared library to the linker via GHDL
	$(GHDL) -e $(GHDL_FLAGS) $(GHDL_LINK_PREFIX)./$(SHARED_LIB_NAME) and_gate_tb -o $(GHDL_EXE_NAME)

# Rule to build the C++ DPI glue shared library
# Depends on the C++ glue source and the Verilator-generated static libraries
$(SHARED_LIB_NAME): $(CXX_GLUE_SRC) $(VERILATOR_MODEL_LIB) $(VERILATOR_DPI_LIB) $(VERILATOR_RUNTIME_LIB)
	@echo "--- Building C++ DPI Glue Shared Library ($@) ---"
	$(CXX) $(CXXFLAGS) $(SHARED_LDFLAG) -o $@ $(CXX_GLUE_SRC) \
		$(VERILATOR_MODEL_LIB) $(VERILATOR_DPI_LIB) $(VERILATOR_RUNTIME_LIB) \
		-I$(VERILATOR_OBJ_DIR) # Include obj_dir for Vand_gate.h etc.

# Rule to build the Verilator model static libraries (DUT model, DPI runtime, main runtime)
# Depends on the Verilog DUT source
$(VERILATOR_MODEL_LIB) $(VERILATOR_DPI_LIB) $(VERILATOR_RUNTIME_LIB): $(VERILOG_DUT_SRC)
	@echo "--- Verilating DUT ($(VERILOG_DUT_SRC)) ---"
	# Verilate the DUT. Use --build to compile sources into static libraries.
	# Enable DPI export generation.
	$(VERILATOR) --cc --exe $(VERILOG_DUT_SRC) --top-module and_gate \
		-CFLAGS "$(CXXFLAGS)" \
		--build -j 0 --Mdir $(VERILATOR_OBJ_DIR) \
		--public-flat-rw --main # Use --main temporarily to force DPI lib generation? Check Verilator docs. Or maybe just --cc is enough if DPI glue links it. Let's try without --main first.
# Re-evaluate Verilator flags: --cc should be sufficient for library generation. --exe implies a top-level C++ file which we don't have here.
# Let's use --cc and ensure DPI is handled. Verilator >= 4.100 includes DPI automatically with --cc.
	$(VERILATOR) --cc $(VERILOG_DUT_SRC) --top-module and_gate \
		-CFLAGS "$(CXXFLAGS)" \
		--build -j 0 --Mdir $(VERILATOR_OBJ_DIR) --public-flat-rw

	@echo "--- Verilator Build Complete ---"
	# Touch the targets to ensure Make recognizes they've been updated
	touch $(VERILATOR_MODEL_LIB) $(VERILATOR_DPI_LIB) $(VERILATOR_RUNTIME_LIB)

# Clean target
clean:
	@echo "--- Cleaning ---"
	rm -rf $(VERILATOR_OBJ_DIR) # Remove Verilator build directory
	rm -f $(SHARED_LIB_NAME)   # Remove C++ shared library
	rm -f $(GHDL_EXE_NAME)     # Remove GHDL executable
	rm -f *.o                  # Remove any stray object files
	rm -f wave.vcd             # Remove waveform file if generated
	# GHDL clean
	$(GHDL) --remove || true   # Ignore errors if nothing to remove
	rm -f work-obj*.cf         # Remove GHDL library file

# Explicitly declare Verilator libs as not PHONY if they match a directory name
.SECONDARY: $(VERILATOR_MODEL_LIB) $(VERILATOR_DPI_LIB) $(VERILATOR_RUNTIME_LIB)