`timescale 1ns/1ps

module and_gate_my_hdl;
    reg a, b;
    wire y;

    initial begin
        $from_myhdl(a, b);
        $to_myhdl(y);
    end

    and_gate dut (.a(a), .b(b), .y(y));

endmodule
