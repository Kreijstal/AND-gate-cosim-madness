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

    initial begin
        // Initialize Inputs
        a = 0;
        b = 0;

        // Wait 100 ns for global reset to finish
        #100;
        
        // Stimulus here
        #10 a = 0; b = 0;
        #10 a = 0; b = 1;
        #10 a = 1; b = 0;
        #10 a = 1; b = 1;
        
        // Add some delay before finishing simulation
        #10;
        
        $finish;
    end
      
    // Monitor changes
    initial begin
        $monitor("Time = %0t, a = %b, b = %b, y = %b", $time, a, b, y);
    end

endmodule
