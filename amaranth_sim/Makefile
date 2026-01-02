# Amaranth pure simulation

PYTHON := $(shell command -v python3 >/dev/null 2>&1 && echo python3 || echo python)

.PHONY: all run clean

all: run

run:
	@echo "Running Amaranth pure simulation"
	$(PYTHON) and_gate_sim.py

clean:
	@echo "No generated files to clean"
