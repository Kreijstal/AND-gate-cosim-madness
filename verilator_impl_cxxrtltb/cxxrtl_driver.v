// CXXRTL-driven testbench: generates input vectors for the Verilator DUT.
module cxxrtl_driver(
    input  wire clk,
    input  wire rst,
    output reg  a,
    output reg  b,
    output reg  expected,
    output reg  done
);
    reg [1:0] idx;

    always @(posedge clk) begin
        if (rst) begin
            idx <= 2'd0;
            a <= 1'b0;
            b <= 1'b0;
            expected <= 1'b0;
            done <= 1'b0;
        end else begin
            case (idx)
                2'd0: begin a <= 1'b0; b <= 1'b0; expected <= 1'b0; end
                2'd1: begin a <= 1'b0; b <= 1'b1; expected <= 1'b0; end
                2'd2: begin a <= 1'b1; b <= 1'b0; expected <= 1'b0; end
                2'd3: begin a <= 1'b1; b <= 1'b1; expected <= 1'b1; end
            endcase
            if (idx == 2'd3) begin
                done <= 1'b1;
            end else begin
                idx <= idx + 2'd1;
            end
        end
    end
endmodule
