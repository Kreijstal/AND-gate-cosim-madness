#include <iostream>
#include <verilated.h>
#include "Vand_gate.h"

void test_and_gate(Vand_gate* dut, bool a, bool b, bool expected) {
    dut->a = a;
    dut->b = b;
    dut->eval();

    std::cout << "Test: a=" << a << ", b=" << b << ", y=" << (int)dut->y;
    if (dut->y == expected) {
        std::cout << " (PASS)" << std::endl;
    } else {
        std::cout << " (FAIL: expected " << expected << ")" << std::endl;
    }
}

int main(int argc, char** argv) {
    // Initialize Verilator
    Verilated::commandArgs(argc, argv);

    // Create an instance of our module under test
    Vand_gate* and_gate = new Vand_gate;

    // Test all input combinations
    test_and_gate(and_gate, 0, 0, 0);
    test_and_gate(and_gate, 0, 1, 0);
    test_and_gate(and_gate, 1, 0, 0);
    test_and_gate(and_gate, 1, 1, 1);

    // Final model cleanup
    and_gate->final();

    // Destroy model
    delete and_gate;

    // Fin
    exit(0);
}
