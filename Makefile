# Root Makefile for AND Gate Project

# Subdirectories
SUBDIRS = iverilog_pure iverilog_vpitb myhdl verilator verilator_manualtb verilator_vpi_iverilogtb vpi_iverilogtb

# Phony targets
.PHONY: all clean $(SUBDIRS) build run

# Default target
all: build

# Build target
build: $(SUBDIRS)

# Build each subdirectory
$(SUBDIRS):
	@echo "Building $@..."
	@$(MAKE) -C $@

# Clean all subdirectories
clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean; \
	done

# Run targets for each implementation
run_iverilog_pure:
	@$(MAKE) -C iverilog_pure

run_iverilog_vpitb:
	@$(MAKE) -C iverilog_vpitb

run_myhdl:
	@$(MAKE) -C myhdl

run_verilator:
	@$(MAKE) -C verilator

run_verilator_manualtb:
	@$(MAKE) -C verilator_manualtb

run_verilator_vpi_iverilogtb:
	@$(MAKE) -C verilator_vpi_iverilogtb

run_vpi_iverilogtb:
	@$(MAKE) -C vpi_iverilogtb

# Run all implementations
run: run_iverilog_pure run_iverilog_vpitb run_myhdl run_verilator run_verilator_manualtb run_verilator_vpi_iverilogtb run_vpi_iverilogtb

# Help target
help:
	@echo "Available targets:"
	@echo "  all                      - Alias for 'build'"
	@echo "  build                    - Build all implementations"
	@echo "  clean                    - Clean all implementations"
	@echo "  run                      - Run all implementations"
	@echo "  run_iverilog_pure        - Run Icarus Verilog pure implementation"
	@echo "  run_iverilog_vpitb       - Run Icarus Verilog with VPI testbench"
	@echo "  run_myhdl                - Run MyHDL implementation"
	@echo "  run_verilator            - Run Verilator implementation"
	@echo "  run_verilator_manualtb   - Run Verilator with manual testbench"
	@echo "  run_verilator_vpi_iverilogtb - Run Verilator with VPI and Icarus Verilog testbench"
	@echo "  run_vpi_iverilogtb       - Run VPI with Icarus Verilog testbench"
	@echo "  help                     - Show this help message"
