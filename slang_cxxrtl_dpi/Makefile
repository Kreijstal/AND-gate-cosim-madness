# SV -> Yosys (slang) -> CXXRTL with Verilator DPI-C testbench

VERILATOR ?= $(or $(wildcard /usr/bin/verilator),verilator)
YOSYS = yosys
YOSYS_CONFIG ?= yosys-config

# Source files
SV_DUT = and_gate.sv
SV_PROXY = and_gate_proxy.sv
SV_TB = and_gate_tb.sv
DPI_CPP = and_gate_dpi.cpp
CXXRTL_CPP = and_gate_cxxrtl.cpp

# Verilator settings
TOP_MODULE = and_gate_tb
OBJ_DIR = obj_dir
TARGET = $(OBJ_DIR)/V$(TOP_MODULE)

# CXXRTL include path detection
YOSYS_DATDIR := $(shell $(YOSYS_CONFIG) --datdir 2>/dev/null)
ifeq ($(strip $(YOSYS_DATDIR)),)
  ifneq ($(strip $(MSYSTEM_PREFIX)),)
    YOSYS_DATDIR := $(MSYSTEM_PREFIX)/share/yosys
  else
    YOSYS_DATDIR := /usr/share/yosys
  endif
endif
CXXRTL_SYS_HDR_OLD := $(YOSYS_DATDIR)/include/backends/cxxrtl/cxxrtl.h
CXXRTL_SYS_HDR_NEW := $(YOSYS_DATDIR)/include/backends/cxxrtl/runtime/cxxrtl/cxxrtl.h
ifneq ($(wildcard $(CXXRTL_SYS_HDR_NEW)),)
  CXXRTL_INC := -I$(YOSYS_DATDIR)/include/backends/cxxrtl/runtime
else ifneq ($(wildcard $(CXXRTL_SYS_HDR_OLD)),)
  CXXRTL_INC := -I$(YOSYS_DATDIR)/include
else
  CXXRTL_INC := -I$(YOSYS_DATDIR)/include -I$(YOSYS_DATDIR)/include/backends/cxxrtl/runtime
endif

# Verilator flags
VERILATOR_FLAGS = --cc --exe --main --sv --timing -CFLAGS "-DVL_TIME_CONTEXT $(CXXRTL_INC)"

.PHONY: all run clean

all: run

# Generate CXXRTL model from SystemVerilog using slang frontend
$(CXXRTL_CPP): $(SV_DUT)
	@echo "--- Generating CXXRTL model from SystemVerilog (slang) ---"
	$(YOSYS) -m slang -p "read_slang $(SV_DUT); write_cxxrtl $(CXXRTL_CPP)"

# Generate Verilator makefile
$(OBJ_DIR)/V$(TOP_MODULE).mk: $(SV_PROXY) $(SV_TB) $(DPI_CPP) $(CXXRTL_CPP)
	@echo "--- Running Verilator ---"
	$(VERILATOR) $(VERILATOR_FLAGS) --top-module $(TOP_MODULE) $(SV_PROXY) $(SV_TB) $(DPI_CPP)

# Build the simulation
$(TARGET): $(OBJ_DIR)/V$(TOP_MODULE).mk
	@echo "--- Building simulation ---"
	make -j -C $(OBJ_DIR) -f V$(TOP_MODULE).mk V$(TOP_MODULE)

run: $(TARGET)
	@echo "--- Running simulation ---"
	./$(TARGET)

clean:
	@echo "--- Cleaning ---"
	rm -rf $(OBJ_DIR)
	rm -f $(CXXRTL_CPP)
