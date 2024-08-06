`timescale 1ns / 1ps

module and_gate_tb;

    // Inputs
    reg a;
    reg b;

    // Outputs
    wire y;

    // Instantiate the Unit Under Test (UUT)
    and_gate uut (
        .a(a),
        .b(b),
        .y(y)
    );

    // Task for checking the AND gate output
    task check_and_gate;
        input a_in, b_in, expected_y;
        begin
            a = a_in;
            b = b_in;
            #1; // Allow some time for the output to settle
            if (y !== expected_y) begin
                $display("ERROR: a=%b, b=%b, y=%b (expected %b)", a, b, y, expected_y);
                $finish;
            end else begin
                $display("PASS: a=%b, b=%b, y=%b", a, b, y);
            end
        end
    endtask

    initial begin
        // Initialize Inputs
        a = 0;
        b = 0;

        // Wait 100 ns for global reset to finish
        #100;

        // Stimulus and assertions
        check_and_gate(0, 0, 0);
        check_and_gate(0, 1, 0);
        check_and_gate(1, 0, 0);
        check_and_gate(1, 1, 1);

        // Add some delay before finishing simulation
        #10;

        $display("All tests passed!");
        $finish;
    end

endmodule
