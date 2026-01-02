from amaranth import Elaboratable, Module, Signal
from amaranth.sim import Simulator


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
    sim = Simulator(dut)

    async def bench(ctx):
        for a in (0, 1):
            for b in (0, 1):
                ctx.set(dut.a, a)
                ctx.set(dut.b, b)
                y = ctx.get(dut.y)
                expected = a & b
                if y != expected:
                    raise AssertionError(
                        f"FAIL: a={a} b={b} y={y} expected={expected}"
                    )
                print(f"PASS: a={a} b={b} y={y}")

    sim.add_testbench(bench)
    sim.run()
    print("All tests passed!")


if __name__ == "__main__":
    main()
