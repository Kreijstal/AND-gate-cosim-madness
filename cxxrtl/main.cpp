#include <iostream>
#include "and_gate.cpp"

int main() {
    cxxrtl_design::p_and__gate top;

    struct Vec {
        bool a;
        bool b;
        bool y;
    } vectors[] = {
        {false, false, false},
        {false, true,  false},
        {true,  false, false},
        {true,  true,  true},
    };

    bool all_ok = true;
    for (const auto &v : vectors) {
        top.p_a.set<bool>(v.a);
        top.p_b.set<bool>(v.b);
        top.step();

        bool y = top.p_y.get<bool>();
        if (y != v.y) {
            std::cout << "FAIL: a=" << v.a << " b=" << v.b
                      << " y=" << y << " expected=" << v.y << '\n';
            all_ok = false;
        } else {
            std::cout << "PASS: a=" << v.a << " b=" << v.b
                      << " y=" << y << '\n';
        }
    }

    if (!all_ok) {
        return 1;
    }

    std::cout << "All tests passed!" << '\n';
    return 0;
}
