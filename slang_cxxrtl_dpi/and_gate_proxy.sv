`timescale 1ns / 1ps

// Auto-generated CXXRTL DPI proxy for module 'and_gate'
module and_gate(
    input wire a,
    input wire b,
    output wire y
);

    import "DPI-C" function void cxxrtl_and_gate(input int a_i, input int b_i, output int y_o);

    wire [31:0] a_ext = {{31{1'b0}}, a};
    wire [31:0] b_ext = {{31{1'b0}}, b};
    logic [31:0] y_ext;

    always_comb begin
        cxxrtl_and_gate(a_ext, b_ext, y_ext);
    end

    assign y = y_ext[0];

endmodule
