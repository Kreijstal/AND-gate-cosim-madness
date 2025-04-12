# Makefile for GHDL VHDL project with Verilated AND gate (Windows & Linux friendly)

# --- Tools ---
GHDL = ghdl
VERILATOR = verilator
CXX = g++
# MAKE = make # No longer needed explicitly for Verilator build

# --- OS Detection and Platform-Specific Flags ---
ifeq ($(OS),Windows_NT)
    IS_WINDOWS := 1
    SHARED =
    F_PIC =
    MINGW_DIR ?= $(MINGW_PREFIX)
    VERILATOR_ROOT ?= $(firstword $(wildcard $(MINGW_DIR)/local/share/verilator $(MINGW_DIR)/share/verilator))
    VERILATOR_LIB_LOC ?= $(firstword $(wildcard $(MINGW_DIR)/lib $(VERILATOR_ROOT)/lib/mingw))
    VERILATOR_LDFLAGS_PARTS = -Lobj_dir $(if $(VERILATOR_LIB_LOC),-L$(VERILATOR_LIB_LOC)) -lverilated -lstdc++ -lpthread
else # Linux/Unix
    IS_WINDOWS := 0
    SHARED =
    F_PIC = -fPIC
    VERILATOR_ROOT ?= $(firstword $(wildcard /usr/local/share/verilator /usr/share/verilator))
    VERILATOR_LIB_LOC ?= $(firstword $(wildcard \
                            /usr/lib64/verilator \
                            /usr/lib/verilator \
                            /usr/local/lib/verilator \
                            $(VERILATOR_ROOT)/lib \
                            /usr/lib64 \
                            /usr/lib \
                            /usr/local/lib \
                         ))
    VERILATOR_LDFLAGS_PARTS = -Lobj_dir $(if $(VERILATOR_LIB_LOC),-L$(VERILATOR_LIB_LOC)) -lverilated -lstdc++ -lpthread
endif

# --- Verilator Include Paths ---
VERILATOR_INC ?= $(VERILATOR_ROOT)/include
VERILATOR_VLTSTD ?= $(VERILATOR_ROOT)/include/vltstd

# --- Flags ---
GHDL_FLAGS = --std=08
# *** Use --build to automatically compile the generated C++ code ***
VERILATOR_FLAGS = --cc --Mdir obj_dir --compiler gcc --build
CXXFLAGS = -std=c++14 $(F_PIC) -Iobj_dir -I$(VERILATOR_INC) -I$(VERILATOR_VLTSTD)

# --- Source files ---
VHDL_SRC = and_gate_pkg.vhd and_gate.vhd and_gate_tb.vhd
VERILOG_SRC = ../common/and_gate.v
WRAPPER_SRC = and_gate_verilated.cpp

# --- Object files ---
VHDL_OBJ = work-obj08.cf
VERILATED_LIB_NAME = Vand_gate__ALL.a # Base name of the archive
VERILATED_OBJ = obj_dir/$(VERILATED_LIB_NAME)
# VERILATOR_MAKEFILE = obj_dir/Vand_gate.mk # No longer needed directly in this rule
WRAPPER_OBJ = and_gate_verilated.o

# --- Top-level entity ---
TOP_ENTITY = and_gate_tb

# --- Targets ---
.PHONY: all run clean build

all: run
build: $(TOP_ENTITY)

# Step 1 & 2 Combined: Run Verilator to generate C++ sources AND build them
$(VERILATED_OBJ): $(VERILOG_SRC)
	@echo "--- Verilating and Building $(VERILOG_SRC) ---"
	$(VERILATOR) $(VERILATOR_FLAGS) $<
	# Ensure the target file actually exists after the build
	@test -f $@ || (echo "Error: Verilator build failed to produce $@"; exit 1)


# Compile C++ wrapper (depends on Verilated headers existing, which $(VERILATED_OBJ) ensures)
$(WRAPPER_OBJ): $(WRAPPER_SRC) $(VERILATED_OBJ)
	@echo "--- Compiling Wrapper $(WRAPPER_SRC) ---"
	$(CXX) $(CXXFLAGS) -c $(WRAPPER_SRC) -o $(WRAPPER_OBJ)

# Analyze VHDL files
$(VHDL_OBJ): $(VHDL_SRC)
	@echo "--- Analyzing VHDL ---"
	$(GHDL) -a $(GHDL_FLAGS) $(VHDL_SRC)

# Elaborate and link with Verilated model and wrapper
# Pass linker flags individually using -Wl,
$(TOP_ENTITY): $(VHDL_OBJ) $(WRAPPER_OBJ) $(VERILATED_OBJ)
	@echo "--- Elaborating and Linking $(TOP_ENTITY) ---"
	$(GHDL) -e $(GHDL_FLAGS) \
		-Wl,$(WRAPPER_OBJ) \
		-Wl,$(VERILATED_OBJ) \
		$(foreach flag,$(VERILATOR_LDFLAGS_PARTS),-Wl,$(flag)) \
		$(TOP_ENTITY)

# Run
run: $(TOP_ENTITY)
	@echo "--- Running Simulation ---"
	./$(TOP_ENTITY)

# Clean rule
clean:
	@echo "--- Cleaning ---"
	$(GHDL) --remove || true # Ignore errors if work dir doesn't exist
	rm -f *.o *.cf *.ghw $(TOP_ENTITY) $(WRAPPER_OBJ)
	rm -rf obj_dir