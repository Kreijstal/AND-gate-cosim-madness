// ghdl_verilator_driver/and_gate_tb.cpp
#include "Vand_gate_proxy.h" // Generated by Verilator
#include "verilated.h"       // Core Verilator routines
#include <iostream>          // For std::cout

// Keep track of simulation time
vluint64_t main_time = 0;
double sc_time_stamp() {
    return main_time;
}

int main(int argc, char** argv) {
    // Initialize Verilators variables
    Verilated::commandArgs(argc, argv);

    // Create an instance of our module
    Vand_gate_proxy* top = new Vand_gate_proxy;

    // Reset the module
    top->a = 0;
    top->b = 0;
    top->eval();

    std::cout << "Starting simulation..." << std::endl;

    // Test case 1: 0 AND 0
    top->a = 0;
    top->b = 0;
    top->eval();
    main_time++;
    std::cout << "Time: " << main_time << " Test: a=0, b=0 -> y=" << (int)top->y << " (Expected: 0)" << std::endl;
    assert(top->y == 0);

    // Test case 2: 0 AND 1
    top->a = 0;
    top->b = 1;
    top->eval();
    main_time++;
    std::cout << "Time: " << main_time << " Test: a=0, b=1 -> y=" << (int)top->y << " (Expected: 0)" << std::endl;
    assert(top->y == 0);

    // Test case 3: 1 AND 0
    top->a = 1;
    top->b = 0;
    top->eval();
    main_time++;
    std::cout << "Time: " << main_time << " Test: a=1, b=0 -> y=" << (int)top->y << " (Expected: 0)" << std::endl;
    assert(top->y == 0);

    // Test case 4: 1 AND 1
    top->a = 1;
    top->b = 1;
    top->eval();
    main_time++;
    std::cout << "Time: " << main_time << " Test: a=1, b=1 -> y=" << (int)top->y << " (Expected: 1)" << std::endl;
    assert(top->y == 1);

    // Final model cleanup
    top->final();
    delete top;

    std::cout << "Simulation finished successfully!" << std::endl;
    return 0;
}
