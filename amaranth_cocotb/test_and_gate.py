import cocotb
from cocotb.triggers import Timer


@cocotb.test()
async def and_gate_basic(dut):
    tests = [
        (0, 0, 0),
        (0, 1, 0),
        (1, 0, 0),
        (1, 1, 1),
    ]
    for a, b, y in tests:
        dut.a.value = a
        dut.b.value = b
        await Timer(1, units="ns")
        assert int(dut.y.value) == y, f"a={a} b={b} y={int(dut.y.value)}"
