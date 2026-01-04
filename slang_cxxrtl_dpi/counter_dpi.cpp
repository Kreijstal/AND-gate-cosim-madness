#include "svdpi.h"
#include <cstdint>
#include "counter_cxxrtl.cpp"

// Static CXXRTL instance
static cxxrtl_design::p_counter cxxrtl_dut;

extern "C" void cxxrtl_counter(int clk_i, int rst_i, int en_i, int* count_o) {
    // Set inputs
    cxxrtl_dut.p_clk.set<bool>(clk_i != 0);
    cxxrtl_dut.p_rst.set<bool>(rst_i != 0);
    cxxrtl_dut.p_en.set<bool>(en_i != 0);

    // Step simulation - CXXRTL handles edge detection internally
    cxxrtl_dut.step();

    // Read outputs
    *count_o = cxxrtl_dut.p_count.get<uint32_t>();
}
