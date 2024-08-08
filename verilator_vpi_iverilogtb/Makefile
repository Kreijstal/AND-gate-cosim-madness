VERILATOR = verilator
IVERILOG = iverilog
VVP = vvp

# Detect operating system
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Set Verilator root based on OS
ifeq ($(DETECTED_OS),Windows)
    POSSIBLE_VERILATOR_ROOTS = $(MSYSTEM_PREFIX)/local/share/verilator $(MSYSTEM_PREFIX)/share/verilator
    MINGW_DIR = $(MSYSTEM_PREFIX)
    IVERILOG_DIR = $(MINGW_DIR)
    VPI_INCLUDE_DIR = $(MINGW_DIR)/include/iverilog
else
    POSSIBLE_VERILATOR_ROOTS = /usr/local/share/verilator /usr/share/verilator
    POSSIBLE_IVERILOG_DIRS = /usr /usr/local
    IVERILOG_DIR = $(firstword $(foreach dir,$(POSSIBLE_IVERILOG_DIRS),$(wildcard $(dir)/include/iverilog)))
    VPI_INCLUDE_DIR = $(IVERILOG_DIR)
endif

# Find the first existing Verilator root
VERILATOR_ROOT = $(firstword $(foreach dir,$(POSSIBLE_VERILATOR_ROOTS),$(wildcard $(dir))))

ifeq ($(VERILATOR_ROOT),)
    $(error Verilator root directory not found. Please install Verilator or set VERILATOR_ROOT manually.)
endif

ifeq ($(DETECTED_OS),Linux)
    ifeq ($(IVERILOG_DIR),)
        $(error Icarus Verilog directory not found. Please install Icarus Verilog or set IVERILOG_DIR manually.)
    endif
endif

VERILATOR_FLAGS = -Wall --cc -CFLAGS -fPIC -CFLAGS -DVL_TIME_CONTEXT
CFLAGS = -Wall -g -I$(VERILATOR_ROOT)/include -I$(VPI_INCLUDE_DIR) -Iobj_dir -fPIC -DVL_TIME_CONTEXT
CXXFLAGS = $(CFLAGS) -std=c++14 -faligned-new
LDFLAGS = -L$(VERILATOR_ROOT)/include -L$(IVERILOG_DIR)/../lib -L./obj_dir

VPI_SRC = and_gate_vpi.c
WRAPPER_SRC = and_gate_vpi_wrapper.cpp
VPI_OUT = and_gate_vpi.vpi
VERILOG_SRC = ../common/and_gate.v
VERILOG_PROXY = and_gate_proxy.v
VERILOG_TB = ../common/and_gate_tb.v
VVP_EXEC = and_gate_tb.vvp

# Adjust commands for Windows
ifeq ($(DETECTED_OS),Windows)
    MKDIR = mkdir
    RM = rm -rf
    COPY = cp
    CC = gcc
    CXX = g++
    SHARED = -shared
    VPI_CFLAGS = -I$(VPI_INCLUDE_DIR) 
    VPI_LDFLAGS = -L$(MINGW_DIR)/lib -lvpi -lveriuser
else
    MKDIR = mkdir -p
    RM = rm -rf
    COPY = cp
    CC = gcc
    CXX = g++
    SHARED = -shared
    VPI_CFLAGS = -I$(VPI_INCLUDE_DIR) -fPIC
    VPI_LDFLAGS = -L$(IVERILOG_DIR)/../lib -lvpi -lveriuser
endif

# Debug prints
$(info IVERILOG_DIR = $(IVERILOG_DIR))
$(info VPI_INCLUDE_DIR = $(VPI_INCLUDE_DIR))

# Set default target based on OS
ifeq ($(DETECTED_OS),Windows)
    .DEFAULT_GOAL := build #see https://github.com/steveicarus/iverilog/issues/1157
else
    .DEFAULT_GOAL := run
endif


.PHONY: all clean run build

all: $(.DEFAULT_GOAL)

build: $(VVP_EXEC)

obj_dir:
	$(MKDIR) obj_dir

obj_dir/Vand_gate.cpp: $(VERILOG_SRC) | obj_dir
	$(VERILATOR) $(VERILATOR_FLAGS) $< --Mdir obj_dir

obj_dir/Vand_gate__ALL.o: obj_dir/Vand_gate.cpp
	$(MAKE) -C obj_dir -f Vand_gate.mk

obj_dir/libverilated.a: obj_dir/Vand_gate.cpp
	$(MAKE) -C obj_dir -f Vand_gate.mk

obj_dir/and_gate_vpi_wrapper.o: $(WRAPPER_SRC) and_gate_vpi_wrapper.h obj_dir/Vand_gate.cpp | obj_dir
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj_dir/and_gate_vpi.o: $(VPI_SRC) and_gate_vpi_wrapper.h | obj_dir
	$(CC) $(CFLAGS) $(VPI_CFLAGS) -c $< -o $@

$(VPI_OUT): obj_dir/and_gate_vpi.o obj_dir/and_gate_vpi_wrapper.o obj_dir/Vand_gate__ALL.o obj_dir/libverilated.a
	$(CXX) $(SHARED) -o $@ $^ $(LDFLAGS) $(VPI_LDFLAGS) -lverilated -lpthread -lvpi

$(VVP_EXEC): $(VERILOG_PROXY) $(VERILOG_TB) $(VPI_OUT)
	$(IVERILOG) -o $@ -m./$(VPI_OUT) $(VERILOG_PROXY) $(VERILOG_TB)

run: $(VVP_EXEC)
	$(VVP) -M. -m$(basename $(VPI_OUT)) $<

clean:
	$(RM) obj_dir *.o *.vpi $(VVP_EXEC)
