// A simple cell library for synthesis

// Inverter
module INV(input A, output Y);
  assign Y = ~A;
endmodule

// 2-input AND gate
module AND2(input A, input B, output Y);
  assign Y = A & B;
endmodule

// D-flip-flop
module DFF(input D, input CLK, output Q);
  reg Q_reg;
  assign Q = Q_reg;
  always @(posedge CLK) begin
    Q_reg <= D;
  end
endmodule
