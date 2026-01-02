# Amaranth + Cocotb testbench

PYTHON := $(shell command -v python3 >/dev/null 2>&1 && echo python3 || echo python)
SIM ?= icarus
ifneq ($(filter iverilog,$(SIM)),)
  SIM := icarus
endif

TOPLEVEL_LANG = verilog
TOPLEVEL = and_gate
MODULE = test_and_gate

AMARANTH_SRC = and_gate_amaranth.py
VERILOG_OUT = and_gate_amaranth.v
VERILOG_SOURCES = $(CURDIR)/$(VERILOG_OUT)

COCOTB_MAKEFILES := $(shell cocotb-config --makefiles 2>/dev/null)
ifeq ($(strip $(COCOTB_MAKEFILES)),)
  COCOTB_MAKEFILES := $(shell $(PYTHON) -c "import cocotb_tools, os; print(os.path.join(os.path.dirname(cocotb_tools.__file__), 'makefiles'))" 2>/dev/null)
endif
COCOTB_MAKEFILE := $(COCOTB_MAKEFILES)/Makefile.sim

.PHONY: all clean

all: $(VERILOG_OUT)
	@$(MAKE) sim

$(VERILOG_OUT): $(AMARANTH_SRC)
	@echo "Generating Verilog from Amaranth"
	$(PYTHON) $(AMARANTH_SRC)

include $(COCOTB_MAKEFILE)

clean::
	@echo "Cleaning up generated files"
	rm -f $(VERILOG_OUT) results.xml
	rm -rf sim_build
