#include "Vand_gate.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>

// Persistent model pointer
static Vand_gate* and_gate_model = nullptr;

// Initialization function (call once)
extern "C" void and_gate_verilated_init() {
    if (!and_gate_model) {
        Verilated::commandArgs(0, (char**)nullptr);
        and_gate_model = new Vand_gate;
    }
}

// The function to be called from VHDL
extern "C" char and_gate_verilated(char a, char b) {
    if (!and_gate_model) {
        and_gate_verilated_init();
    }
    // Set inputs (assuming 1-bit logic: '0' or '1')
    and_gate_model->a = (a == '1') ? 1 : 0;
    and_gate_model->b = (b == '1') ? 1 : 0;
    and_gate_model->eval();
    char result = (and_gate_model->y) ? '1' : '0';
    printf("VERILATED: a='%c', b='%c', result='%c'\n", a, b, result);
    fflush(stdout);
    return result;
}