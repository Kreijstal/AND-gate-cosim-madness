`timescale 1ns / 1ps

// Auto-generated CXXRTL DPI proxy for module 'counter'
module counter(
    input wire clk,
    input wire rst,
    input wire en,
    output wire [7:0] count
);

    import "DPI-C" function void cxxrtl_counter(input int clk_i, input int rst_i, input int en_i, output int count_o);

    wire [31:0] clk_ext = {{31{1'b0}}, clk};
    wire [31:0] rst_ext = {{31{1'b0}}, rst};
    wire [31:0] en_ext = {{31{1'b0}}, en};
    logic [31:0] count_ext;

    always_comb begin
        cxxrtl_counter(clk_ext, rst_ext, en_ext, count_ext);
    end

    assign count = count_ext[7:0];

endmodule
