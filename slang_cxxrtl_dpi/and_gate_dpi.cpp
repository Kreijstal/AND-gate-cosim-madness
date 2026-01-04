#include "svdpi.h"
#include <cstdint>
#include "and_gate_cxxrtl.cpp"

// Static CXXRTL instance
static cxxrtl_design::p_and__gate cxxrtl_dut;

extern "C" void cxxrtl_and_gate(int a_i, int b_i, int* y_o) {
    // Set inputs
    cxxrtl_dut.p_a.set<bool>(a_i != 0);
    cxxrtl_dut.p_b.set<bool>(b_i != 0);

    // Step simulation - CXXRTL handles edge detection internally
    cxxrtl_dut.step();

    // Read outputs
    *y_o = cxxrtl_dut.p_y.get<bool>() ? 1 : 0;
}
