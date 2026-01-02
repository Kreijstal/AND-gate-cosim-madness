# Makefile for CXXRTL implementation with Icarus Verilog VPI testbench

# --- Tools ---
YOSYS = yosys
YOSYS_CONFIG ?= yosys-config
IVERILOG = iverilog
VVP = vvp
CXX = g++

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
    SHARED = -shared -fPIC
    VPI_CFLAGS = -I/usr/include/iverilog
    VPI_LDFLAGS = -L/usr/lib -lvpi -lveriuser
endif

# --- Flags ---
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
IVERILOG_FLAGS = -Wall -g2012
VVP_FLAGS = -M. -m and_gate_cxxrtl

# --- Source Files ---
COMMON_DIR = ../common
VERILOG_DUT_SRC = $(COMMON_DIR)/and_gate.v
VERILOG_TB_SRCS = and_gate_tb.v and_gate_proxy.v
VPI_SRC = and_gate_vpi.cpp
CXXRTL_CPP = and_gate.cpp

# --- Output Files ---
VPI_MODULE_NAME = and_gate_cxxrtl
VPI_MODULE = $(VPI_MODULE_NAME).vpi
IVERILOG_OUTPUT = and_gate_tb.vvp

# --- CXXRTL Include Paths ---
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

# --- Targets ---
.PHONY: all build run clean

all: run

build: $(IVERILOG_OUTPUT)

run: build
	@echo "--- Running Simulation (Icarus Verilog + CXXRTL VPI) ---"
	$(VVP) $(VVP_FLAGS) $(IVERILOG_OUTPUT)
	@echo "--- Simulation Finished ---"

$(IVERILOG_OUTPUT): $(VERILOG_TB_SRCS) $(VPI_MODULE)
	@echo "--- Compiling Verilog Testbench (Icarus Verilog) ---"
	$(IVERILOG) $(IVERILOG_FLAGS) -o $@ $(VERILOG_TB_SRCS)

$(CXXRTL_CPP): $(VERILOG_DUT_SRC)
	@echo "--- Generating CXXRTL model ($(CXXRTL_CPP)) ---"
	$(YOSYS) -p "read_verilog $(VERILOG_DUT_SRC); write_cxxrtl $(CXXRTL_CPP)"

$(VPI_MODULE): $(VPI_SRC) $(CXXRTL_CPP)
	@echo "--- Building VPI Module ($(VPI_MODULE)) ---"
	$(CXX) $(CXXFLAGS) $(CXXRTL_INC) $(VPI_CFLAGS) $(SHARED) -o $(VPI_MODULE) $(VPI_SRC) $(VPI_LDFLAGS)

clean:
	@echo "--- Cleaning ---"
	rm -f $(CXXRTL_CPP)
	rm -f $(VPI_MODULE)
	rm -f $(IVERILOG_OUTPUT)
	rm -f *.o
