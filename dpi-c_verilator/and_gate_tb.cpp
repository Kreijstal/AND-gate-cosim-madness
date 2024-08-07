#include "Vand_gate_proxy.h"
#include "verilated.h"
#include <iostream>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    
    Vand_gate_proxy* top = new Vand_gate_proxy;
    
    // Test cases
    top->a = 0; top->b = 0; top->eval(); std::cout << "0 AND 0 = " << (int)top->y << std::endl;
    top->a = 0; top->b = 1; top->eval(); std::cout << "0 AND 1 = " << (int)top->y << std::endl;
    top->a = 1; top->b = 0; top->eval(); std::cout << "1 AND 0 = " << (int)top->y << std::endl;
    top->a = 1; top->b = 1; top->eval(); std::cout << "1 AND 1 = " << (int)top->y << std::endl;
    
    delete top;
    return 0;
}
