#include "Vand_gate_proxy.h"
#include "verilated.h"
#include "verilated_vpi.h"

vluint64_t main_time = 0;

double sc_time_stamp() {
    return main_time;
}

void and_gate_vpi() {
    vpiHandle a_handle = vpi_handle_by_name((PLI_BYTE8*)"TOP.and_gate_proxy.a", NULL);
    vpiHandle b_handle = vpi_handle_by_name((PLI_BYTE8*)"TOP.and_gate_proxy.b", NULL);
    vpiHandle y_handle = vpi_handle_by_name((PLI_BYTE8*)"TOP.and_gate_proxy.y", NULL);

    if (!a_handle || !b_handle || !y_handle) {
        vpi_printf("Error: Could not get handles to a, b, or y\n");
        return;
    }

    s_vpi_value a_val, b_val, y_val;
    a_val.format = b_val.format = y_val.format = vpiIntVal;

    vpi_get_value(a_handle, &a_val);
    vpi_get_value(b_handle, &b_val);

    int a = a_val.value.integer;
    int b = b_val.value.integer;
    int y = a & b;

    y_val.value.integer = y;
    vpi_put_value(y_handle, &y_val, NULL, vpiNoDelay);

    vpi_printf("VPI: a=%d, b=%d -> y=%d\n", a, b, y);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    const std::unique_ptr<Vand_gate_proxy> top{new Vand_gate_proxy{contextp.get()}};
    
    printf("Starting simulation\n");

    while (!contextp->gotFinish() && main_time < 10) {
        contextp->timeInc(1);
        
        // Change input values
        top->a = main_time % 2;
        top->b = (main_time / 2) % 2;

        top->eval();
        VerilatedVpi::callValueCbs();
        
        and_gate_vpi();  // Call our VPI function directly
        
        if (VerilatedVpi::evalNeeded()) {
            top->eval();
            VerilatedVpi::clearEvalNeeded();
        }

        printf("Time %ld: a=%d, b=%d, y=%d\n", main_time, top->a, top->b, top->y);
        
        main_time++;
    }

    top->final();
    printf("Simulation finished\n");
    return 0;
}
