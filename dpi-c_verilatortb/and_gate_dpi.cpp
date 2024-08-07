#include "svdpi.h"
#include <cstdio>

extern "C" int and_gate_dpi(int a, int b) {
    int y = a & b;
    printf("DPI-C: a=%d, b=%d -> y=%d\n", a, b, y);
    return y;
}
