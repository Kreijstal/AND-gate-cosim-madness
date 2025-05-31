#include <systemc.h>

SC_MODULE(and_gate) {
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> out;

    void process_and() {
        out.write(a.read() && b.read());
    }

    SC_CTOR(and_gate) {
        SC_METHOD(process_and);
        sensitive << a << b;
    }
};
