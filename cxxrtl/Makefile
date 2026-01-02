# CXXRTL simulation for AND gate

CXX = g++
CXXFLAGS = -std=c++17 -O2 -g -Wall -Wextra
YOSYS = yosys
YOSYS_CONFIG ?= yosys-config

YOSYS_DATDIR := $(shell $(YOSYS_CONFIG) --datdir 2>/dev/null)
ifeq ($(strip $(YOSYS_DATDIR)),)
  ifneq ($(strip $(MSYSTEM_PREFIX)),)
    YOSYS_DATDIR := $(MSYSTEM_PREFIX)/share/yosys
  else
    YOSYS_DATDIR := /usr/share/yosys
  endif
endif
ROOT_DIR := $(abspath ..)
LOCAL_CXXRTL_DIR := $(ROOT_DIR)/cxxrtl_runtime
CXXRTL_INC := -I$(YOSYS_DATDIR)/include -I$(YOSYS_DATDIR)/include/backends/cxxrtl/runtime
ifneq ($(wildcard $(LOCAL_CXXRTL_DIR)/cxxrtl/cxxrtl.h),)
  CXXRTL_INC := -I$(LOCAL_CXXRTL_DIR) $(CXXRTL_INC)
endif

SRC_DIR = ../common
VERILOG_SRC = $(SRC_DIR)/and_gate.v
CXXRTL_SRC = and_gate.cpp
TB_SRC = main.cpp
TB_BIN = and_gate_cxxrtl

.PHONY: all clean run

all: run

$(CXXRTL_SRC): $(VERILOG_SRC)
	@echo "Generating CXXRTL model: $@"
	$(YOSYS) -p "read_verilog $(VERILOG_SRC); write_cxxrtl $(CXXRTL_SRC)"

$(TB_BIN): $(CXXRTL_SRC) $(TB_SRC)
	@echo "Compiling CXXRTL testbench: $@"
	$(CXX) $(CXXFLAGS) $(CXXRTL_INC) $(TB_SRC) -o $(TB_BIN)

run: $(TB_BIN)
	@echo "Running CXXRTL testbench"
	./$(TB_BIN)

clean:
	@echo "Cleaning up generated files"
	rm -f $(CXXRTL_SRC) $(TB_BIN)
