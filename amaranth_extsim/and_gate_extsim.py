from amaranth import Elaboratable, Module, Signal, ClockDomain
from amaranth.sim import Simulator

from external_sim import ExternalBackend, ExternalSimulator


class AndGateProxy(Elaboratable):
    def __init__(self):
        self.a = Signal()
        self.b = Signal()
        self.y = Signal()

    def elaborate(self, platform):
        m = Module()
        m.domains.sync = ClockDomain()
        return m


class MockAndBackend(ExternalBackend):
    def eval(self, inputs):
        a = inputs.get("a", 0)
        b = inputs.get("b", 0)
        return {"y": a & b}


async def testbench(ctx, dut):
    for a in (0, 1):
        for b in (0, 1):
            ctx.set(dut.a, a)
            ctx.set(dut.b, b)
            await ctx.tick()
            y = ctx.get(dut.y)
            expected = a & b
            if y != expected:
                raise AssertionError(
                    f"FAIL: a={a} b={b} y={y} expected={expected}"
                )
            print(f"PASS: a={a} b={b} y={y}")


def main():
    dut = AndGateProxy()
    backend = MockAndBackend()

    sim = ExternalSimulator(
        dut,
        backend,
        inputs={"a": dut.a, "b": dut.b},
        outputs={"y": dut.y},
    )
    sim.add_clock(1e-6)
    async def tb(ctx):
        await testbench(ctx, dut)
    sim.add_testbench(tb)
    sim.run()
    print("All tests passed!")


if __name__ == "__main__":
    main()
