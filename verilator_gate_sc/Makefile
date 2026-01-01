VERILATOR = verilator
VERILATOR_FLAGS = -Wall --trace --sc --exe -CFLAGS -DVL_TIME_CONTEXT
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS =

# Determine the operating system
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2/MinGW)
    POSSIBLE_VERILATOR_ROOTS = $(MSYSTEM_PREFIX)/local/share/verilator $(MSYSTEM_PREFIX)/share/verilator
    SYSTEMC_LIBDIR ?= $(MSYSTEM_PREFIX)/local/systemc/lib-mingw64
    SYSTEMC_INCDIR ?= $(MSYSTEM_PREFIX)/local/systemc/include
else
    # Linux/macOS
    POSSIBLE_VERILATOR_ROOTS = /usr/local/share/verilator /usr/share/verilator
    SYSTEMC_LIBDIR ?= /usr/local/systemc/lib
    SYSTEMC_INCDIR ?= /usr/local/systemc/include
endif

# Find the first existing Verilator root
VERILATOR_ROOT = $(firstword $(foreach dir,$(POSSIBLE_VERILATOR_ROOTS),$(wildcard $(dir))))

ifeq ($(VERILATOR_ROOT),)
    $(error Could not find Verilator installation. Please install Verilator or check your installation path.)
endif

CXXFLAGS += -I$(VERILATOR_ROOT)/include -I$(VERILATOR_ROOT)/include/vltstd -Iobj_dir -I$(SYSTEMC_INCDIR)
LDFLAGS += -L$(VERILATOR_ROOT)/include -L$(SYSTEMC_LIBDIR) -lsystemc

TARGET = and_gate_tb
VERILOG_SOURCES = and_gate.v
CPP_SOURCES = and_gate_tb.cpp

.PHONY: all clean run

all: run

$(TARGET): $(VERILOG_SOURCES) $(CPP_SOURCES)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILOG_SOURCES) --top-module and_gate --exe $(CPP_SOURCES)
	make -C obj_dir -f Vand_gate.mk Vand_gate

build: $(TARGET)

run: $(TARGET)
	obj_dir/Vand_gate

clean:
	rm -rf obj_dir
	rm -f *.o $(TARGET)
