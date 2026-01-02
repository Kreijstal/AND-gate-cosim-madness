# Verilator DUT with CXXRTL-driven testbench (C++ cosim)

VERILATOR ?= $(or $(wildcard /usr/bin/verilator),verilator)
YOSYS = yosys
YOSYS_CONFIG ?= yosys-config

COMMON_DIR = ../common
VERILOG_DUT_SRC = $(COMMON_DIR)/and_gate.v
CXXRTL_SRC = cxxrtl_driver.v
CXXRTL_CPP = cxxrtl_driver.cpp
TB_CPP = and_gate_tb.cpp
OBJ_DIR = obj_dir
TB_BIN = $(OBJ_DIR)/Vand_gate

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

.PHONY: all run clean

all: run

$(CXXRTL_CPP): $(CXXRTL_SRC)
	@echo "--- Generating CXXRTL driver ($(CXXRTL_CPP)) ---"
	$(YOSYS) -p "read_verilog $(CXXRTL_SRC); write_cxxrtl $(CXXRTL_CPP)"

$(TB_BIN): $(VERILOG_DUT_SRC) $(TB_CPP) $(CXXRTL_CPP)
	@echo "--- Building Verilator model with CXXRTL TB ---"
	$(VERILATOR) --cc $(VERILOG_DUT_SRC) --top-module and_gate \
		--exe $(TB_CPP) --build -j 0 --Mdir $(OBJ_DIR) \
		-CFLAGS "-std=c++17 -O2 -DVL_TIME_CONTEXT $(CXXRTL_INC)"

run: $(TB_BIN)
	@echo "--- Running Verilator DUT with CXXRTL-driven TB ---"
	./$(TB_BIN)

clean:
	@echo "--- Cleaning ---"
	rm -rf $(OBJ_DIR)
	rm -f $(CXXRTL_CPP)
