# SystemC AND Gate Example

This directory contains a SystemC implementation of a simple 2-input AND gate and a testbench to verify its functionality.

## Files

- `and_gate.cpp`: Defines the `and_gate` SystemC module.
- `and_gate_tb.cpp`: Contains the SystemC testbench that instantiates and tests the `and_gate`. This file also includes the `sc_main` function to run the simulation.
- `Makefile`: Used to compile the SystemC code and run the simulation.

## Usage

To compile and run the simulation:

1.  Ensure you have SystemC installed and configured on your system.
2.  From this directory (`systemc`), run the following commands:

    ```bash
    make
    ```
    This command compiles the `and_gate_tb.cpp` (which includes `and_gate.cpp`) and creates an executable named `and_gate_sim`.

    ```bash
    make run
    ```
    This command executes the compiled simulation `./and_gate_sim`. The testbench will output messages indicating the results of the test cases.

To clean up compiled files:
    ```bash
    make clean
    ```
    This will remove the `and_gate_sim` executable and any object files.
