#include <iostream>
#include <verilated.h>
#include "Vand_gate.h"
#include "and_gate.cpp"

struct Vec {
    bool a;
    bool b;
    bool y;
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    cxxrtl_design::p_and__gate cxxrtl_dut;
    Vand_gate verilator_ref;

    Vec vectors[] = {
        {false, false, false},
        {false, true,  false},
        {true,  false, false},
        {true,  true,  true},
    };

    bool all_ok = true;
    for (const auto &v : vectors) {
        cxxrtl_dut.p_a.set<bool>(v.a);
        cxxrtl_dut.p_b.set<bool>(v.b);
        cxxrtl_dut.step();
        bool y_cxx = cxxrtl_dut.p_y.get<bool>();

        verilator_ref.a = v.a;
        verilator_ref.b = v.b;
        verilator_ref.eval();
        bool y_ver = verilator_ref.y;

        if (y_cxx != v.y || y_ver != v.y) {
            std::cout << "FAIL: a=" << v.a << " b=" << v.b
                      << " cxxrtl=" << y_cxx
                      << " verilator=" << y_ver
                      << " expected=" << v.y << '\n';
            all_ok = false;
        } else {
            std::cout << "PASS: a=" << v.a << " b=" << v.b
                      << " y=" << y_cxx << '\n';
        }
    }

    verilator_ref.final();

    if (!all_ok) {
        return 1;
    }

    std::cout << "All tests passed!" << '\n';
    return 0;
}
