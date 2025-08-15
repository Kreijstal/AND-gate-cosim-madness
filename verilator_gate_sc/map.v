module _and(A, B, Y);
  input A, B;
  output Y;
  AND2 and_gate(.A(A), .B(B), .Y(Y));
endmodule
