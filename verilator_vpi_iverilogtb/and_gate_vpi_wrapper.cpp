#include "and_gate_vpi_wrapper.h"
#include "Vand_gate.h"
#include <verilated.h>

extern "C" {

void* create_and_gate_model() {
    return new Vand_gate;
}

void destroy_and_gate_model(void* model) {
    delete static_cast<Vand_gate*>(model);
}

void set_and_gate_inputs(void* model, int a, int b) {
    Vand_gate* and_gate = static_cast<Vand_gate*>(model);
    and_gate->a = a;
    and_gate->b = b;
    // Force full evaluation
    and_gate->eval_step();
    and_gate->eval_end_step();
}

int get_and_gate_output(void* model) {
    Vand_gate* and_gate = static_cast<Vand_gate*>(model);
    return and_gate->y;
}

}

// Initialize Verilated modules
static struct VerilatedInitialize {
    VerilatedInitialize() {
        Verilated::traceEverOn(true);
    }
} verilated_initialize;
