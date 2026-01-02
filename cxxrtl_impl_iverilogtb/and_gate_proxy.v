`timescale 1ns / 1ps

module and_gate_proxy(
    input wire a,
    input wire b,
    output wire y
);

    // Declare the VPI system function
    // It takes inputs a, b and returns the result y
    function integer cxxrtl_and_gate;
        input a, b;
        begin
            cxxrtl_and_gate = $cxxrtl_and_gate(a, b);
        end
    endfunction

    // Assign the output based on the VPI function call
    assign y = cxxrtl_and_gate(a, b);

endmodule
