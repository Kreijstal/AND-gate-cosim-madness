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

    Vand_gate verilator_dut;
    cxxrtl_design::p_and__gate cxxrtl_ref;

    Vec vectors[] = {
        {false, false, false},
        {false, true,  false},
        {true,  false, false},
        {true,  true,  true},
    };

    bool all_ok = true;
    for (const auto &v : vectors) {
        verilator_dut.a = v.a;
        verilator_dut.b = v.b;
        verilator_dut.eval();
        bool y_ver = verilator_dut.y;

        cxxrtl_ref.p_a.set<bool>(v.a);
        cxxrtl_ref.p_b.set<bool>(v.b);
        cxxrtl_ref.step();
        bool y_cxx = cxxrtl_ref.p_y.get<bool>();

        if (y_ver != v.y || y_cxx != v.y) {
            std::cout << "FAIL: a=" << v.a << " b=" << v.b
                      << " verilator=" << y_ver
                      << " cxxrtl=" << y_cxx
                      << " expected=" << v.y << '\n';
            all_ok = false;
        } else {
            std::cout << "PASS: a=" << v.a << " b=" << v.b
                      << " y=" << y_ver << '\n';
        }
    }

    verilator_dut.final();

    if (!all_ok) {
        return 1;
    }

    std::cout << "All tests passed!" << '\n';
    return 0;
}
