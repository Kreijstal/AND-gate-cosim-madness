#include <stdint.h>
#include <stdio.h>

__attribute__((used)) 
char and_gate_c(char a, char b) {
    char result = (a == '1' && b == '1') ? '1' : '0';
    printf("DEBUG C: a='%c', b='%c', result='%c'\n", a, b, result);
    fflush(stdout);  // Ensure the output is printed immediately
    return result;
}
