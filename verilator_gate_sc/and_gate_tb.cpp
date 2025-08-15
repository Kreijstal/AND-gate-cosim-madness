#include <systemc.h>
#include "Vand_gate.h"

SC_MODULE(Testbench) {
    Vand_gate* dut;
    sc_signal<bool> a, b, y;

    void test_and_gate(bool val_a, bool val_b, bool expected) {
        a.write(val_a);
        b.write(val_b);
        wait(1, SC_NS); // wait for the model to evaluate
        cout << "Test: a=" << val_a << ", b=" << val_b << ", y=" << y.read();
        if (y.read() == expected) {
            cout << " (PASS)" << endl;
        } else {
            cout << " (FAIL: expected " << expected << ")" << endl;
        }
    }

    void run_tests() {
        // Test all input combinations
        test_and_gate(0, 0, 0);
        test_and_gate(0, 1, 0);
        test_and_gate(1, 0, 0);
        test_and_gate(1, 1, 1);

        sc_stop();
    }

    SC_CTOR(Testbench) {
        dut = new Vand_gate("dut");
        dut->a(a);
        dut->b(b);
        dut->y(y);
        SC_THREAD(run_tests);
    }
};

int sc_main(int argc, char** argv) {
    Testbench tb("tb");
    sc_start();
    return 0;
}
