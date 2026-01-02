from amaranth import Elaboratable, Module, Signal
from amaranth.back import verilog


class AndGate(Elaboratable):
    def __init__(self):
        self.a = Signal()
        self.b = Signal()
        self.y = Signal()

    def elaborate(self, platform):
        m = Module()
        m.d.comb += self.y.eq(self.a & self.b)
        return m


def main():
    dut = AndGate()
    ports = [dut.a, dut.b, dut.y]
    verilog_src = verilog.convert(dut, ports=ports, name="and_gate")
    with open("and_gate_amaranth.v", "w", encoding="ascii") as f:
        f.write(verilog_src)


if __name__ == "__main__":
    main()
