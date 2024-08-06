import os
from myhdl import *

def find_myhdl_vpi():
    possible_paths = [
        "./myhdl.vpi",
        "/usr/lib/python3.12/site-packages/myhdl/conversion/myhdl.vpi",
        "/usr/local/lib/python3.12/dist-packages/myhdl/conversion/myhdl.vpi"
    ]
    for path in possible_paths:
        if os.path.exists(path):
            return path
    raise FileNotFoundError("myhdl.vpi not found. Please run 'python -m myhdl.conversion.iverilog' to generate it.")

def and_gate_verilog(a, b, y):
    cmd = "iverilog -o and_gate_myhdl.vvp ../common/and_gate.v and_gate_myhdl.v"
    if os.system(cmd) != 0:
        raise RuntimeError("Verilog compilation failed")

    vpi_path = find_myhdl_vpi()
    return Cosimulation(f"vvp -m {vpi_path} and_gate_myhdl.vvp", a=a, b=b, y=y)

def and_gate_python(a, b, y):
    @always_comb
    def logic():
        y.next = a and b
    return logic

def tb_and_gate():
    a, b = [Signal(bool(0)) for i in range(2)]
    y_verilog = Signal(bool(0))
    y_python = Signal(bool(0))

    and_gate_verilog_inst = and_gate_verilog(a, b, y_verilog)
    and_gate_python_inst = and_gate_python(a, b, y_python)

    @instance
    def stimulus():
        print("a b | y_verilog y_python")
        print("-------------------------")
        for i in range(4):
            a.next, b.next = bool(i & 2), bool(i & 1)
            yield delay(10)
            print(f"{int(a)} {int(b)} | {int(y_verilog)}        {int(y_python)}")
            assert y_verilog == y_python, f"Mismatch at a={int(a)}, b={int(b)}: y_verilog={int(y_verilog)}, y_python={int(y_python)}"

    return and_gate_verilog_inst, and_gate_python_inst, stimulus

if __name__ == "__main__":
    tb = tb_and_gate()
    sim = Simulation(tb)
    sim.run()
