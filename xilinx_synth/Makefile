# Yosys synth-only flow for Xilinx

YOSYS = yosys
TOP = and_gate
COMMON_DIR = ../common
SRC = $(COMMON_DIR)/and_gate.v

.PHONY: all run clean

all: run

run: $(TOP).edif $(TOP).json
	@echo "Xilinx synth complete"

$(TOP).edif $(TOP).json: $(SRC)
	@echo "Running Yosys synth_xilinx for $(TOP)"
	$(YOSYS) -p "read_verilog $(SRC); synth_xilinx -top $(TOP); write_edif $(TOP).edif; write_json $(TOP).json"

clean:
	@echo "Cleaning synth outputs"
	rm -f $(TOP).edif $(TOP).json
