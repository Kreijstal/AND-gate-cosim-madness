`timescale 1ns / 1ps

module counter_tb;

    logic clk;
    logic rst;
    logic en;
    wire [7:0] count;

    counter uut (
        .clk(clk),
        .rst(rst),
        .en(en),
        .count(count)
    );

    // Clock generation
    initial clk = 0;
    always #5 clk = ~clk;

    initial begin
        // Initialize
        rst = 1;
        en = 0;

        // Wait and release reset
        #20;
        rst = 0;

        // Check initial value
        #10;
        if (count !== 8'd0) begin
            $display("ERROR: Expected count=0, got %d", count);
            $finish;
        end
        $display("PASS: count=%d after reset", count);

        // Enable counting
        en = 1;

        // Count a few cycles
        repeat (5) @(posedge clk);
        #1;

        if (count !== 8'd5) begin
            $display("ERROR: Expected count=5, got %d", count);
            $finish;
        end
        $display("PASS: count=%d after 5 cycles", count);

        // Disable and verify no change
        en = 0;
        repeat (3) @(posedge clk);
        #1;

        if (count !== 8'd5) begin
            $display("ERROR: Expected count=5 (unchanged), got %d", count);
            $finish;
        end
        $display("PASS: count=%d unchanged when disabled", count);

        $display("All tests passed!");
        $finish;
    end

endmodule
