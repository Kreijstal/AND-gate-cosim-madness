// File: and_gate_tb_wrapper.v
module and_gate_tb;
    reg a, b;
    wire y;

    and_gate dut (.a(a), .b(b), .y(y));

    initial begin
        $dumpfile("and_gate.vcd");
        $dumpvars(0, and_gate_tb);
        a = 0;
        b = 0;
        $run_test;
        #10;

        #10;

        #10;

        #10;
    end
endmodule
