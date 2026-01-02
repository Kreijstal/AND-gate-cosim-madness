# CXXRTL simulation for AND gate

CXX = g++
CXXFLAGS = -std=c++17 -O2 -g -Wall -Wextra
YOSYS = yosys

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
	$(CXX) $(CXXFLAGS) -I "$(shell yosys-config --datdir)/include/backends/cxxrtl/runtime" $(TB_SRC) -o $(TB_BIN)

run: $(TB_BIN)
	@echo "Running CXXRTL testbench"
	./$(TB_BIN)

clean:
	@echo "Cleaning up generated files"
	rm -f $(CXXRTL_SRC) $(TB_BIN)
