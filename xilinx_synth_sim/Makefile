# Simulate Yosys Xilinx synth netlist with Icarus Verilog

YOSYS = yosys
YOSYS_CONFIG ?= yosys-config
IVERILOG = iverilog
VVP = vvp

TOP = and_gate
COMMON_DIR = ../common
SRC = $(COMMON_DIR)/and_gate.v
TB = $(COMMON_DIR)/and_gate_tb.v
SYNTH_V = and_gate_synth.v
VVP_OUT = and_gate_tb_synth.vvp

YOSYS_DATDIR := $(shell $(YOSYS_CONFIG) --datdir 2>/dev/null)
ifeq ($(strip $(YOSYS_DATDIR)),)
  ifneq ($(strip $(MSYSTEM_PREFIX)),)
    YOSYS_DATDIR := $(MSYSTEM_PREFIX)/share/yosys
  else
    YOSYS_DATDIR := /usr/share/yosys
  endif
endif
XILINX_CELLS_SIM := $(YOSYS_DATDIR)/xilinx/cells_sim.v

.PHONY: all run clean

all: run

$(SYNTH_V): $(SRC)
	@echo "Generating Xilinx synth netlist for $(TOP)"
	$(YOSYS) -p "read_verilog $(SRC); synth_xilinx -top $(TOP); write_verilog -noattr $(SYNTH_V)"

$(VVP_OUT): $(SYNTH_V) $(TB)
	@echo "Compiling synth netlist with Icarus Verilog"
	$(IVERILOG) -g2012 -o $(VVP_OUT) $(TB) $(SYNTH_V) $(XILINX_CELLS_SIM)

run: $(VVP_OUT)
	@echo "Running synth netlist simulation"
	$(VVP) $(VVP_OUT)

clean:
	@echo "Cleaning synth sim outputs"
	rm -f $(SYNTH_V) $(VVP_OUT)
