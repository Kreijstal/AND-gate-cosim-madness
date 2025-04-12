# Makefile for GHDL VHDL project with Verilated AND gate (Windows & Linux friendly)

# --- Tools ---
GHDL = ghdl
VERILATOR = verilator
CXX = g++

# --- OS Detection and Platform-Specific Flags ---
ifeq ($(OS),Windows_NT)
    IS_WINDOWS := 1
    SHARED =
    F_PIC =
    # Default Verilator install location for MSYS2/MINGW
    MINGW_DIR ?= $(MINGW_PREFIX)
    VERILATOR_ROOT ?= $(firstword $(wildcard $(MINGW_DIR)/local/share/verilator $(MINGW_DIR)/share/verilator))
    VERILATOR_LIB ?= obj_dir
    VERILATOR_LDFLAGS = -L$(VERILATOR_LIB) -lverilated -lstdc++ -lpthread
else
    IS_WINDOWS := 0
    SHARED =
    F_PIC = -fPIC
    # Auto-detect Verilator root
    VERILATOR_ROOT ?= $(firstword $(wildcard /usr/local/share/verilator /usr/share/verilator))
    VERILATOR_LIB ?= obj_dir
    VERILATOR_LDFLAGS = -L$(VERILATOR_LIB) -lverilated -lstdc++ -lpthread
endif

# --- Verilator Include Paths ---
VERILATOR_INC ?= $(VERILATOR_ROOT)/include
VERILATOR_VLTSTD ?= $(VERILATOR_ROOT)/include/vltstd

# --- Flags ---
GHDL_FLAGS = --std=08
VERILATOR_FLAGS = --cc --Mdir obj_dir --compiler gcc
CXXFLAGS = -std=c++14 $(F_PIC) -Iobj_dir -I$(VERILATOR_INC) -I$(VERILATOR_VLTSTD)

# --- Source files ---
VHDL_SRC = and_gate_pkg.vhd and_gate.vhd and_gate_tb.vhd
VERILOG_SRC = ../common/and_gate.v
WRAPPER_SRC = and_gate_verilated.cpp

# --- Object files ---
VHDL_OBJ = work-obj08.cf
VERILATED_OBJ = obj_dir/Vand_gate__ALL.a
WRAPPER_OBJ = and_gate_verilated.o

# --- Top-level entity ---
TOP_ENTITY = and_gate_tb

# --- Targets ---
.PHONY: all run clean build

all: run
build: $(TOP_ENTITY)

# Run Verilator to generate C++ model
$(VERILATED_OBJ): $(VERILOG_SRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $<

# Compile C++ wrapper
$(WRAPPER_OBJ): $(WRAPPER_SRC) $(VERILATED_OBJ)
	$(CXX) $(CXXFLAGS) -c $(WRAPPER_SRC) -o $(WRAPPER_OBJ)

# Analyze VHDL files
$(VHDL_OBJ): $(VHDL_SRC)
	$(GHDL) -a $(GHDL_FLAGS) $(VHDL_SRC)

# Elaborate and link with Verilated model and wrapper
$(TOP_ENTITY): $(VHDL_OBJ) $(WRAPPER_OBJ) $(VERILATED_OBJ)
	$(GHDL) -e $(GHDL_FLAGS) -Wl,$(WRAPPER_OBJ) -Wl,$(VERILATED_OBJ) -Wl,$(VERILATOR_LDFLAGS) $(TOP_ENTITY)

# Run
run: $(TOP_ENTITY)
	./$(TOP_ENTITY)

# Clean rule
clean:
	$(GHDL) --remove
	rm -f *.o *.cf *.ghw $(TOP_ENTITY)
	rm -rf obj_dir
