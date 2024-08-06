`timescale 1ns / 1ps

module and_gate(
    input wire a,
    input wire b,
    output wire y
);

    // Use the external VPI function for the AND gate
    // If you want to use a Verilog-based AND gate instead, uncomment the assign statement below
    assign y = $and_gate(a, b);
    // assign y = a & b;

endmodule

