// ghdl_verilator_driver/and_gate_c.c
#include <stdio.h> // For printf (optional, for debugging)

// Attribute to ensure the function is not optimized away if unused by other C code
__attribute__((used)) 
char and_gate_c(char a, char b) {
    // Simple AND logic
    char result = (a == '1' && b == '1') ? '1' : '0';
    
    // Optional: print inputs and result for debugging during simulation
    // printf("C (and_gate_c): a='%c', b='%c' -> result='%c'\n", a, b, result);
    // fflush(stdout); // Ensure printf output is flushed immediately
    
    return result;
}
