# ghdl_verilator_driver/Makefile

# Tool paths (can be overridden)
GHDL ?= ghdl
VERILATOR ?= verilator
CXX ?= g++
LD ?= g++
AR ?= ar

# Verilator flags for building the executable
VERILATOR_BUILD_FLAGS = --cc --exe --build -j --timing --trace 
# VERILATOR_BUILD_FLAGS += -CFLAGS "-DVERILATOR_VCD_ENABLE" # Enable VCD trace if needed

# GHDL flags for analysis
GHDL_ANALYSIS_FLAGS = --std=08
# GHDL flags for elaboration & shared library creation
# -fPIC is important for shared libraries
# The -eunit_name is not strictly needed here as we are not running a GHDL simulation directly
# but it's good practice. We'll make a shared object from the C file later.
GHDL_ELAB_FLAGS = -fPIC
GHDL_SHARED_LIB_FLAGS = --vpi-compile-only -fPIC

# C compiler flags for the C interface
# -fPIC is crucial for the shared object that GHDL/Verilator will link against
CFLAGS = -fPIC -Wall -Wextra -g
# CXX flags for Verilator generated code and our testbench
CXXFLAGS = -std=c++14 -Wall -Wextra -g

# Source files
VHDL_SOURCES = and_gate_pkg.vhd and_gate.vhd
C_INTERFACE_SOURCE = and_gate_c.c
C_INTERFACE_OBJ = $(C_INTERFACE_SOURCE:.c=.o)
SV_PROXY_SOURCE = and_gate_proxy.sv
TB_SOURCE = and_gate_tb.cpp

# Output names
GHDL_LIB_NAME_PLAIN = andgatec
SHARED_LIB_ARCHIVE = lib$(GHDL_LIB_NAME_PLAIN).a # Static library archive
# Set GHDL VPI specific linker flags
# Attempt to get GHDL VPI library path using the modern command
VPI_LIB_DIR_DETECTED = $(shell $(GHDL) --print-vpi-library-dir 2>/dev/null)

# Common default for GHDL mcode (Debian/Ubuntu style). This might need adjustment for other systems/distros.
COMMON_DEFAULT_VPI_LIB_DIR = /usr/lib/ghdl/mcode

# Use detected path if available, otherwise let user override or use common default.
# The `?=` ensures that if VPI_LIB_DIR is passed via `make VPI_LIB_DIR=/custom/path`, it's used first.
# If VPI_LIB_DIR is not passed from environment, it then considers VPI_LIB_DIR_DETECTED.
# If VPI_LIB_DIR_DETECTED is empty, it falls back to COMMON_DEFAULT_VPI_LIB_DIR.
VPI_LIB_DIR ?= $(firstword $(VPI_LIB_DIR_DETECTED) $(COMMON_DEFAULT_VPI_LIB_DIR))

# Issue warnings or info messages based on how VPI_LIB_DIR was determined
ifeq ($(strip $(VPI_LIB_DIR_DETECTED)),)
  # --print-vpi-library-dir failed or returned empty
  ifneq ($(strip $(VPI_LIB_DIR)), $(strip $(COMMON_DEFAULT_VPI_LIB_DIR)))
    # User provided a VPI_LIB_DIR, and it's different from the common default. This is good.
    $(info GHDL VPI library path auto-detection failed. Using user-provided VPI_LIB_DIR: $(VPI_LIB_DIR))
  else
    # Auto-detection failed, and VPI_LIB_DIR is the COMMON_DEFAULT_VPI_LIB_DIR (meaning user didn't override to something else).
    $(warning GHDL VPI library path auto-detection with '--print-vpi-library-dir' failed.)
    $(warning Using default VPI_LIB_DIR: $(VPI_LIB_DIR). If this is incorrect for your system, please set VPI_LIB_DIR manually.)
    $(warning For example: make VPI_LIB_DIR=/path/to/your/ghdl_backend/vpi)
  endif
else
  # --print-vpi-library-dir succeeded
  ifeq ($(strip $(VPI_LIB_DIR)), $(strip $(VPI_LIB_DIR_DETECTED)))
      # We are using the auto-detected path (either VPI_LIB_DIR was not set by user, or user set it to the same detected path).
      $(info GHDL VPI library path detected by --print-vpi-library-dir: $(VPI_LIB_DIR))
  else
      # Auto-detection worked, but user provided a different VPI_LIB_DIR. User's choice takes precedence.
      $(info GHDL VPI library path detected as "$(VPI_LIB_DIR_DETECTED)", but using user-provided VPI_LIB_DIR: "$(VPI_LIB_DIR)")
  endif
endif

# Ensure GHDL_VPI_LDFLAGS uses this VPI_LIB_DIR. This definition should already exist and be correct.
# GHDL_VPI_LDFLAGS = -L$(VPI_LIB_DIR) -lghdlvpi -Wl,-rpath,$(VPI_LIB_DIR)
GHDL_VPI_LDFLAGS = -L$(VPI_LIB_DIR) -lghdlvpi -Wl,-rpath,$(VPI_LIB_DIR)
VERILATED_DIR = obj_dir
TOP_MODULE = and_gate_proxy # Verilator top module (matches SV filename usually)
EXECUTABLE = $(VERILATED_DIR)/V$(TOP_MODULE)

# Default target
all: $(EXECUTABLE)

# --- GHDL Compilation ---
# Analyze VHDL files
# GHDL analysis needs to happen before C compilation if there are dependencies,
# but here C code is standalone for VHPIDIRECT.
# WORK_VHDL_FILES = $(patsubst %.vhd,$(CURDIR)/%.o,$(VHDL_SOURCES)) # Not directly used as object files
GHDL_ANALYZE_TARGETS = $(VHDL_SOURCES:=.analyzed)
$(GHDL_ANALYZE_TARGETS): %.analyzed: %
	$(GHDL) -a $(GHDL_ANALYSIS_FLAGS) $(patsubst %.analyzed,%,$@)

# Compile C interface into an object file
$(C_INTERFACE_OBJ): $(C_INTERFACE_SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@

# Create a static library from the C object file for linking with Verilator
$(SHARED_LIB_ARCHIVE): $(C_INTERFACE_OBJ)
	$(AR) rcs $(SHARED_LIB_ARCHIVE) $(C_INTERFACE_OBJ)

# --- Main Executable Compilation using Verilator ---
# This command will:
# 1. Verilate $(SV_PROXY_SOURCE) into C++ files in $(VERILATED_DIR).
# 2. Compile those generated C++ files.
# 3. Compile our C++ testbench $(TB_SOURCE).
# 4. Link everything together into $(EXECUTABLE), using the specified LDFLAGS.
# Dependencies include the SV source, C++ testbench, our C library, and GHDL analyzed files.
$(EXECUTABLE): $(SV_PROXY_SOURCE) $(TB_SOURCE) $(SHARED_LIB_ARCHIVE) $(GHDL_ANALYZE_TARGETS)
	$(VERILATOR) $(VERILATOR_BUILD_FLAGS) \
		--top-module $(TOP_MODULE) $(SV_PROXY_SOURCE) \
		--exe $(TB_SOURCE) \
		--Mdir $(VERILATED_DIR) \
		-CFLAGS "$(CXXFLAGS) -I$(VERILATED_DIR)" \
		-LDFLAGS "-Wl,-rpath,$(CURDIR) -L$(CURDIR) -l$(GHDL_LIB_NAME_PLAIN) $(GHDL_VPI_LDFLAGS)"

# --- Run target ---
run: $(EXECUTABLE)
	@echo "Running simulation..."
	@$^
	@echo "Simulation finished."

# --- Clean target ---
clean:
	rm -rf $(VERILATED_DIR)
	rm -f $(C_INTERFACE_OBJ) $(SHARED_LIB_ARCHIVE)
	rm -f $(VHDL_SOURCES:=.analyzed) # Clean GHDL analysis markers
	rm -f work-obj08.cf # GHDL 0.37 and earlier
	rm -f *.o *.ghw *.cf lib*.so # General GHDL clean (might be too broad, but covers .o from old makefile)
	rm -f common_ghdl_compile.sh # If used by some GHDL versions
	@echo "Cleaned."

.PHONY: all run clean $(GHDL_ANALYZE_TARGETS)
