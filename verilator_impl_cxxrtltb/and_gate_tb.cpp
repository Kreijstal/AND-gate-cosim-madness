#include <iostream>
#include <verilated.h>
#include "Vand_gate.h"
#include "cxxrtl_driver.cpp"

static void tick_driver(cxxrtl_design::p_cxxrtl__driver &driver) {
    driver.p_clk.set<bool>(false);
    driver.step();
    driver.p_clk.set<bool>(true);
    driver.step();
    driver.p_clk.set<bool>(false);
    driver.step();
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    Vand_gate verilator_dut;
    cxxrtl_design::p_cxxrtl__driver cxxrtl_tb;

    bool all_ok = true;
    cxxrtl_tb.p_rst.set<bool>(true);
    tick_driver(cxxrtl_tb);
    cxxrtl_tb.p_rst.set<bool>(false);

    while (true) {
        tick_driver(cxxrtl_tb);
        bool a = cxxrtl_tb.p_a.get<bool>();
        bool b = cxxrtl_tb.p_b.get<bool>();
        bool expected = cxxrtl_tb.p_expected.get<bool>();
        bool done = cxxrtl_tb.p_done.get<bool>();

        verilator_dut.a = a;
        verilator_dut.b = b;
        verilator_dut.eval();
        bool y_ver = verilator_dut.y;

        if (y_ver != expected) {
            std::cout << "FAIL: a=" << a << " b=" << b
                      << " verilator=" << y_ver
                      << " expected=" << expected << '\n';
            all_ok = false;
        } else {
            std::cout << "PASS: a=" << a << " b=" << b
                      << " y=" << y_ver << '\n';
        }

        if (done) {
            break;
        }
    }

    verilator_dut.final();

    if (!all_ok) {
        return 1;
    }

    std::cout << "All tests passed!" << '\n';
    return 0;
}
