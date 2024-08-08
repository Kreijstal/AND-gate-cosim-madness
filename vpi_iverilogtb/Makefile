# Variables
VPI_SRC = and_gate_vpi.c
VPI_OUT = and_gate_vpi.vpi
VERILOG_SRCS = and_gate_proxy.v ../common/and_gate_tb.v
VVP_EXEC = and_gate_tb.vvp
VCD_FILE = and_gate_tb.vcd

# Detect OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Compiler and flags
IVERILOG_VPI = iverilog-vpi
IVERILOG = iverilog
VVP = vvp
CFLAGS = -Wall -Wextra -g -fPIC
IVERILOG_FLAGS = -Wall -g2012

# OS-specific settings
ifeq ($(DETECTED_OS),Windows)
    MINGW_DIR = $(MSYSTEM_PREFIX)
    VPI_CMD = $(IVERILOG_VPI) -mingw=$(MINGW_DIR) $(VPI_SRC)
    IVERILOG_FLAGS += -m ./$(basename $(VPI_OUT))
else
    VPI_CMD = $(IVERILOG_VPI) $(CFLAGS) $(VPI_SRC)
    IVERILOG_FLAGS += -m ./$(VPI_OUT)
endif

# Main target
.PHONY: all clean build run

all: run

# Build VPI shared library
$(VPI_OUT): $(VPI_SRC)
	@echo "Building VPI shared library: $@"
	$(VPI_CMD)

# Compile Verilog sources
$(VVP_EXEC): $(VERILOG_SRCS) $(VPI_OUT)
	@echo "Compiling Verilog sources: $@"
	$(IVERILOG) $(IVERILOG_FLAGS) -o $@ $(VERILOG_SRCS)

# Build target
build: $(VVP_EXEC)

# Run simulation
run: $(VVP_EXEC)
	@echo "Running VPI simulation"
	$(VVP) -M. -m$(basename $(VPI_OUT)) $<

# Clean target
clean:
	@echo "Cleaning up generated files"
	rm -f $(VPI_OUT) $(VVP_EXEC) $(VCD_FILE) *.o
