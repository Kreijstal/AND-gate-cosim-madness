#include <systemc.h>
#include "and_gate.cpp" // Include the AND gate module

SC_MODULE(testbench) {
    sc_signal<bool> input_a;
    sc_signal<bool> input_b;
    sc_signal<bool> output_z;

    and_gate uut; // Instantiate the AND gate

    void stimulus() {
        // Test case 1: 0 & 0 = 0
        input_a = false;
        input_b = false;
        wait(1, SC_NS);
        sc_assert(output_z == false);

        // Test case 2: 0 & 1 = 0
        input_a = false;
        input_b = true;
        wait(1, SC_NS);
        sc_assert(output_z == false);

        // Test case 3: 1 & 0 = 0
        input_a = true;
        input_b = false;
        wait(1, SC_NS);
        sc_assert(output_z == false);

        // Test case 4: 1 & 1 = 1
        input_a = true;
        input_b = true;
        wait(1, SC_NS);
        sc_assert(output_z == true);

        SC_REPORT_INFO("Testbench", "All test cases passed!");
        sc_stop(); // End simulation
    }

    SC_CTOR(testbench) : uut("uut") { // Name the instance of and_gate
        uut.a(input_a);
        uut.b(input_b);
        uut.out(output_z);

        SC_THREAD(stimulus);
    }
};

int sc_main(int argc, char* argv[]) {
    testbench tb("tb"); // Instantiate the testbench
    sc_start();        // Run the simulation
    return 0;
}
