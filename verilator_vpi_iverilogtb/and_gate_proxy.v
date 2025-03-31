`timescale 1ns / 1ps

module and_gate(
    input wire a,
    input wire b,
    output reg y // Changed to reg
);

    // Use the external VPI task for the AND gate
    always @* begin
        $and_gate(a, b, y); // Call as task with output argument
    end
    // assign y = a & b; // Keep commented out

endmodule

