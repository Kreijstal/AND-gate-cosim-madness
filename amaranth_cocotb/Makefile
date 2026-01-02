# Amaranth + Cocotb testbench

PYTHON ?= python3
SIM ?= iverilog

TOPLEVEL_LANG = verilog
TOPLEVEL = and_gate
MODULE = test_and_gate

AMARANTH_SRC = and_gate_amaranth.py
VERILOG_OUT = and_gate_amaranth.v
VERILOG_SOURCES = $(CURDIR)/$(VERILOG_OUT)

COCOTB_MAKEFILE := $(shell $(PYTHON) -c "import cocotb, os; print(os.path.join(os.path.dirname(cocotb.__file__), 'share', 'makefiles', 'Makefile.sim'))")

.PHONY: all clean

all: $(VERILOG_OUT)
	@$(MAKE) sim

$(VERILOG_OUT): $(AMARANTH_SRC)
	@echo "Generating Verilog from Amaranth"
	$(PYTHON) $(AMARANTH_SRC)

include $(COCOTB_MAKEFILE)

clean:
	@echo "Cleaning up generated files"
	rm -f $(VERILOG_OUT) results.xml
	rm -rf sim_build
