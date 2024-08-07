`timescale 1ns / 1ps

module and_gate(
    input wire a,
    input wire b,
    output wire y
);

    import "DPI-C" function int and_gate_dpi(input int a, input int b);

    wire [31:0] a_extended = {{31{1'b0}}, a};
    wire [31:0] b_extended = {{31{1'b0}}, b};
    wire [31:0] y_extended;

    assign y_extended = and_gate_dpi(a_extended, b_extended);
    assign y = y_extended[0];

endmodule
