# Verilator Example with GHDL Testbench

This example demonstrates how to use a GHDL testbench to verify a Verilog module that has been Verilated (converted to C++). It utilizes GHDL's VHPIDIRECT foreign language interface to communicate between the VHDL testbench and the C++ Verilated model.

## Files

Here's a breakdown of the key files in this example:

*   `and_gate.v`: The Verilog Design Under Test (DUT). It's a simple 2-input AND gate.
*   `and_gate_tb.vhd`: The GHDL testbench written in VHDL. It drives inputs to the `and_gate` and checks its outputs.
*   `and_gate_pkg.vhd`: A VHDL package containing the foreign procedure and function declarations (using `VHPIDIRECT`) that map to C++ functions. This allows the VHDL testbench to call C++ functions.
*   `and_gate_wrapper.cpp`: A C++ wrapper file. It contains functions callable from VHDL (via `VHPIDIRECT`) that interact with the Verilated `and_gate` model (e.g., setting inputs, evaluating the model, getting outputs).
*   `Makefile`: The makefile used to automate the entire process: Verilating the Verilog code, compiling the C++ wrapper and Verilator-generated code, compiling the VHDL code, linking everything together, and running the simulation.

## Prerequisites

To build and run this example, you will need the following software installed:

*   **Verilator**: A tool for converting Verilog/SystemVerilog to C++/SystemC. Ensure it's in your PATH.
*   **GHDL**: A VHDL simulator. It must be compiled with VHPIDIRECT support (often enabled by default or with specific build flags like `--enable-vhpidirect`). Ensure it's in your PATH.
*   **C++ Compiler**: A C++ compiler compatible with Verilator's output (e.g., g++, clang++).
*   **Make**: The GNU Make utility to process the Makefile.

## How to Run

1.  Navigate to this directory:
    ```bash
    cd ghdl_verilatortb
    ```

2.  Build and run the simulation:
    ```bash
    make
    ```
    Alternatively, you can explicitly call `make all`.

    This command will:
    *   Verilate `and_gate.v` into C++ code (in the `obj_dir` directory).
    *   Compile the Verilator-generated C++ code and the `and_gate_wrapper.cpp`.
    *   Analyze the VHDL files (`and_gate_pkg.vhd`, `and_gate_tb.vhd`) using GHDL.
    *   Elaborate and link the VHDL testbench with the compiled C++ code, creating a simulation executable (e.g., `and_gate_sim`).
    *   Run the simulation executable.

3.  **Expected Output**:
    You should see messages from GHDL and Verilator during the build process. If successful, the simulation run should output:
    ```
    ghdl_verilatortb/and_gate_tb.vhd:49:15:@20ns:(assertion error): Assertion failed for 00
    ghdl_verilatortb/and_gate_tb.vhd:57:15:@30ns:(assertion error): Assertion failed for 01
    ghdl_verilatortb/and_gate_tb.vhd:65:15:@40ns:(assertion error): Assertion failed for 10
    ghdl_verilatortb/and_gate_tb.vhd:73:15:@50ns:(assertion error): Assertion failed for 11
    ghdl_verilatortb/and_gate_tb.vhd:76:8:@50ns:(report note): Testbench finished successfully.
    ```
    *(Note: The example testbench has intentional assertion failures to demonstrate output. You would normally expect all assertions to pass in a real test.)*
    Actually, the assertions in `and_gate_tb.vhd` should pass. The expected output should be:
    ```
    ./and_gate_sim
    ghdl_verilatortb/and_gate_tb.vhd:76:8:@50ns:(report note): Testbench finished successfully.
    ```

## Clean

To remove all generated files (object files, Verilator output, simulation executable, GHDL intermediate files):

```bash
make clean
```

This helps in keeping the directory tidy and ensures a fresh build next time.
